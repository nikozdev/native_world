#include <nw_pch.hpp>
#include "nw_gui_of.h"

#include <ecs/nw_scene.h>
#include <ecs/nw_entity.h>
#include <ecs/nw_entity_cmp.h>

#include <glib/control/nw_draw_engine.h>
#include <glib/control/nw_gapi.h>
#include <glib/control/nw_gcamera_lad.h>
#include <glib/gcontext/nw_gcontext.h>
#include <glib/gcontext/nw_window.h>
#include <glib/gcontext/nw_framebuf.h>
#include <glib/vision/nw_gmaterial.h>
#include <glib/vision/nw_shader.h>
#include <glib/render/nw_texture.h>
#include <glib/render/nw_drawable.h>

#include <core/nw_core_engine.h>
#include <core/nw_core_state.h>
#include <lua/nw_lua_vm.h>

#include <sys/nw_gui_sys.h>
#include <sys/nw_ev_sys.h>
#include <sys/nw_time_sys.h>
#include <sys/nw_io_sys.h>
#include <sys/nw_rand_sys.h>
#include <sys/nw_data_sys.h>
#include <sys/nw_mem_sys.h>
#include <sys/nw_log_sys.h>

#if (NW_GUI & NW_GUI_IMGUI)
#include <imgui.h>
#include <imgui_internal.h>
#include <examples/imgui_impl_opengl3.h>
#include <examples/imgui_impl_glfw.h>
#define GUI_DEFAULT_TREE_FLAGS ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick
#endif	// NW_GUI

#pragma warning(disable : 4312)

// --==<GuiOfGlobal>==--
namespace NW
{
	// --==<GuiOfEngine>==--
	GuiOfEngine::GuiOfEngine() {
		pEState = CoreEngine::Get().GetState();
		pWindow = CoreEngine::Get().GetWindow();
	}
	GuiOfEngine::~GuiOfEngine() { }
	
	void GuiOfEngine::OnDraw() {
		if (!bIsEnabled) return;
		ImGui::Begin("NW_Engine", &bIsEnabled);
		bWindow = ImGui::TreeNodeEx("--==<Window>==--", GUI_DEFAULT_TREE_FLAGS);
		if (bWindow) {
			const WindowInfo& rWindowInfo = pWindow->GetWindowInfo();
			ImGui::Text("Window API: %s",
				rWindowInfo.WApiType == WAPI_GLFW ? "GLFW" : rWindowInfo.WApiType == WAPI_COUT ? "Console" :
				rWindowInfo.WApiType == WAPI_WIN ? "Windows" : "None");
			if (ImGui::InputText("Title", &strWindowTitle[0], 128)) { CoreEngine::Get().GetWindow()->SetTitle(&strWindowTitle[0]); }
			
			ImGui::Text("Size: %dx%d;\nAspect ratio = %f;",
				rWindowInfo.unWidth, rWindowInfo.unHeight, rWindowInfo.nAspectRatio);
			ImGui::Text("Cursor: X = %d; Y = %d;", static_cast<Int32>(IOSys::Cursor.xMove), static_cast<Int32>(IOSys::Cursor.yMove));
			
			bool bVSync = rWindowInfo.bVSync;
			if (ImGui::Checkbox("Vertical synchronization", &bVSync)) { CoreEngine::Get().GetWindow()->SetVSync(bVSync); }

			if (ImGui::BeginCombo("Switch window", &pWindow->GetWindowInfo().strTitle[0])) {
				auto itWindow = CoreEngine::Get().GetWindow();
				if (ImGui::Selectable(&itWindow->GetWindowInfo().strTitle[0])) {
					pWindow = CoreEngine::Get().GetWindow();
					strcpy(&strWindowTitle[0], &pWindow->GetWindowInfo().strTitle[0]);
				} ImGui::EndCombo();
			}
			ImGui::TreePop();
		}
		bAppState = ImGui::TreeNodeEx("--==<Engine_State>==--", GUI_DEFAULT_TREE_FLAGS);
		if (bAppState) {
			if (ImGui::BeginCombo("Switch state", &pEState->GetName()[0])) {
				auto pStates = CoreEngine::Get().GetStates();
				for (auto pEst : pStates) {
					if (ImGui::Selectable(&pEst->GetName()[0])) {
						CoreEngine::Get().SwitchState(&pEst->GetName()[0]);
					} ImGui::EndCombo();
				}
			}
			ImGui::TreePop();
		}
		bLuaVM = ImGui::TreeNodeEx("--==<Lua_Virtual_Machiene>==--", GUI_DEFAULT_TREE_FLAGS);
		if (bLuaVM) {
			if (ImGui::Button("Write info")) {
				LogSys::GetLogOut() << LuaVM::Get().GetInfo();
			}
			ImGui::TreePop();
		}
		ImGui::End();
	}
	// --==</GuiOfEngine>==--

	// --==<GuiOfDataSys>==--
	GuiOfDataSys::GuiOfDataSys() { strcpy(strDir, DataSys::GetDirectory()); }
	GuiOfDataSys::~GuiOfDataSys() { }

	void GuiOfDataSys::OnDraw() {
		if (!bIsEnabled) return;
		ImGui::Begin("Data_System", &bIsEnabled);
		if (ImGui::BeginPopupContextWindow("DataProps", 1, false)) {
			if (ImGui::TreeNodeEx("Create", GUI_DEFAULT_TREE_FLAGS)) {
				if (ImGui::MenuItem("Shader")) { AShader::Create("shd_nameless"); }
				else if (ImGui::MenuItem("Texture2d")) { ATexture2d::Create("tex_2d_nameless"); }
				else if (ImGui::MenuItem("Default 3d")) { MemSys::NewT<GMaterial>("gmt_default_3d"); }
				ImGui::TreePop();
			}
			ImGui::EndPopup();
		}
		
		if (ImGui::Button("file_system_tree")) { system("tree"); }
		else if (ImGui::Button("files_list")) { system("dir"); }
		else if (ImGui::Button("new_directory")) { system("md newdir"); }
		else if (ImGui::Button("delete_directory")) { system("rd newdir"); }
		ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal | ImGuiSeparatorFlags_SpanAllColumns);

		if (ImGui::TreeNodeEx("Data Resources")) {
			DataSys::ADRs& rDrs = DataSys::GetADataResources();
			for (DataSys::ADRs::iterator itDrs = rDrs.begin(); itDrs != rDrs.end(); itDrs++) {
				ImGui::TextColored({0.4f, 0.7f, 0.7f, 1.0f}, "Name: %s; ID: %d", itDrs->second->GetName(), itDrs->first);
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("Textures2d")) {
			auto& Textures = DataSys::GetDataResources<ATexture2d>();
			for (auto& itTex : Textures) {
				ImGui::Text(itTex.second->GetName());
				const ImageInfo& rImgInfo = itTex.second->GetImgInfo();
				if (ImGui::ImageButton(reinterpret_cast<void*>(itTex.second->GetRenderId()),
					ImVec2{ 64.0f * rImgInfo.nWidth / rImgInfo.nHeight, 64.0f })) { GuiOfSpriteEditor::Get().SetContext(itTex.second); }
			} ImGui::Separator();
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("Shaders")) {
			auto& Shaders = DataSys::GetDataResources<AShader>();
			for (auto& pShader : Shaders) {
				if (ImGui::Button(pShader.second->GetName())) { GuiOfCodeEditor::Get().SetContext(pShader.second); }
			} ImGui::Separator();
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("GMaterial")) {
			auto& GMaterials = DataSys::GetDataResources<GMaterial>();
			for (auto& itGMtl : GMaterials) {
				if (ImGui::Button(itGMtl.second->GetName())) { GuiOfGMaterialEditor::Get().SetContext(itGMtl.second); }
			} ImGui::Separator();
			ImGui::TreePop();
		}
		ImGui::End();
	}
	// --==</GuiOfDataSys>==--

	// --==<GuiOfMemSys>==--
	void GuiOfMemSys::OnDraw() {
		if (!bIsEnabled) return;
		ImGui::Begin("Memory_System", &bIsEnabled);
		ImGui::Text("::Allocated memory bytes: %d", MemSys::GetInfo().szAlloc);
		ImGui::Text("::Allocated memory blocks: %d", MemSys::GetInfo().unAlloc);
		ImGui::End();
	}
	// --==</GuiOfMemSys>==--
	
	// --==<GuiOfTimeSys>==--
	void GuiOfTimeSys::OnDraw() {
		if (!bIsEnabled) return;
		ImGui::Begin("Time_System", &bIsEnabled);
		float nAppSpeed = TimeSys::GetAppSpeed();
		ImGui::Text("Work time: %d", static_cast<int>(TimeSys::GetRealTime()));
		ImGui::Text("Application time: %d", static_cast<Int32>(TimeSys::GetAppTime()));
		if (ImGui::DragFloat("Application speed", &nAppSpeed, 0.05f, 0.001f)) { TimeSys::SetAppSpeed(nAppSpeed); }
		ImGui::Text("Frames/second: %d", static_cast<Int32>(1.0f / TimeSys::GetRealDelta()));
		ImGui::Text("Date: %s", strDate);
		if (ImGui::Button("Update date")) { strcpy(strDate, &TimeSys::GetTimeString()[0]); }
		ImGui::End();
	}
	// --==</GuiOfTimeSys>==--

	// --==<GuiOfLogSys>==--
	void GuiOfLogSys::OnDraw() {
		if (!bIsEnabled) return;
		ImGui::Begin("Log_System", &bIsEnabled);
		if (ImGui::Button("Print formatted message")) {
			LogSys::WriteStr("Int: {int};\nFloat: {flt};\nDouble: {flt};\nChar: {chr}\nCString: {str};",
				1, 1.25f, 152.0, 'C', "Format is done!");
		}
		ImGui::End();
	}
	// --==</GuiOfLogSys>==--
	
	// --==<GuiOfRandSys>==--
	void GuiOfRandSys::OnDraw() {
		if (!bIsEnabled) return;
		ImGui::Begin("Random_System", &bIsEnabled);
		if (ImGui::Button("Get random boolean")) {
			bRand = RandSys::GetBool();
		}
		else if (ImGui::Button("Get random byte")) {
			btRand = RandSys::GetByte();
		}
		else if (ImGui::Button("Get random ubyte")) {
			ubtRand = RandSys::GetUByte();
		}
		else if (ImGui::Button("Get random integer")) {
			nRand = RandSys::GetInt();
		}
		else if (ImGui::Button("Get random uinteger")) {
			unRand = RandSys::GetUInt();
		}
		else if (ImGui::Button("Get random float")) {
			fRand = RandSys::GetFloat();
		}
		else if (ImGui::Button("Get random double")) {
			dRand = RandSys::GetDouble();
		}
		ImGui::Text("Bool: %s; Byte: %d; UByte: %d;\nInt: %d; UInt: %d;\nFloat: %f; Double: %f;\n",
			bRand ? "true" : "false", btRand, ubtRand, nRand, unRand, fRand, dRand);
		ImGui::End();
	}
	// --==</GuiOfRandSys>==--

	// --==<GuiOfRenderer>==--
	void GuiOfRender::OnDraw() {
		if (!bIsEnabled) return;

		ImGui::Begin("Graphics", &bIsEnabled);
		bGContext = ImGui::TreeNodeEx("--==<graphics_context>==--", GUI_DEFAULT_TREE_FLAGS);
		if (bGContext) {
			const GContextInfo& rGContextInfo = CoreEngine::Get().GetWindow()->GetGContext()->GetInfo();

			ImGui::Text("--==<Graphics context>==--\nVersion: %s;\nRenderer: %s;"
				"\nVendor: %s;\nShading language: %s"
				"\nMax texture count: %d;\nMax vertex attributes: %d",
				rGContextInfo.strRenderer, rGContextInfo.strVersion,
				rGContextInfo.strVendor, rGContextInfo.strShadingLanguage,
				rGContextInfo.nMaxTextures, rGContextInfo.nMaxVertexAttribs);

			ImGui::TreePop();
		}
		bGApi = ImGui::TreeNodeEx("--==<graphics_api>==--", GUI_DEFAULT_TREE_FLAGS);
		if (bGApi) {
			AGraphicsApi* pGApi = DrawEngine::GetGApi();
			ImGui::Text("Type: %s",
				pGApi->GetType() == GApiTypes::GAPI_OPENGL ? "OpenGL" : pGApi->GetType() == GApiTypes::GAPI_COUT ? "Console" :
				pGApi->GetType() == GApiTypes::GAPI_WIN ? "Windows" :
				"None");
			if (ImGui::DragFloat("Line Width", &nLineW, 0.1f)) DrawEngine::GetGApi()->SetLineWidth(nLineW);
			if (ImGui::DragFloat("Pixel size", &nPixelSz, 0.1f)) DrawEngine::GetGApi()->SetPixelSize(nPixelSz);
			if (ImGui::BeginCombo("DrawMode", &strDrawMode[0])) {
				if (ImGui::Selectable("MD_FILL")) {
					strDrawMode = "MD_FILL";
					DrawEngine::GetGApi()->SetDrawMode(DrawModes::DM_FILL, FacePlanes::FP_FRONT_AND_BACK);
				}
				else if (ImGui::Selectable("MD_LINE")) {
					strDrawMode = "MD_LINE";
					DrawEngine::GetGApi()->SetDrawMode(DrawModes::DM_LINE, FacePlanes::FP_FRONT_AND_BACK);
				}
				ImGui::EndCombo();
			} ImGui::Separator();
			ImGui::TreePop();
		}
		bDrawEngineInfo = ImGui::TreeNodeEx("--==<draw_engine>==--", GUI_DEFAULT_TREE_FLAGS);
		if (bDrawEngineInfo) {
			const DrawEngineInfo& rDInfo = DrawEngine::GetInfo();
			ImGui::Text("Vertex data\n::Count: %d;\n::Size in Bytes: %d/%d;\n",
				rDInfo.unVtx,
				rDInfo.szVtx, rDInfo.szMaxVtx);

			ImGui::InputInt("New MaxVtxSize", &szMaxVtx);
			if (ImGui::Button("Change MaxVtxSize")) DrawEngine::SetMaxVtxSize(szMaxVtx);
			
			szMaxIdx = rDInfo.szMaxIdx;
			ImGui::Text("Index data\n::Count: %d;\n::Size in Bytes: %d/%d;\n",
				rDInfo.unIdx,
				rDInfo.szIdx, rDInfo.szMaxIdx);

			ImGui::InputInt("New MaxIndexSize", &szMaxIdx);
			if (ImGui::Button("Change MaxIndexSize")) DrawEngine::SetMaxIdxSize(szMaxIdx);
			ImGui::Text("Textures\n::Slots: %d/%d;",
				rDInfo.unTex, rDInfo.unMaxTex);

			unTexCount = rDInfo.unMaxTex;
			ImGui::InputInt("New MaxTexCount", &unTexCount);
			if (ImGui::Button("Change MaxTexCount")) DrawEngine::SetMaxTexCount(unTexCount);

			ImGui::Text("Performance\n::Draw calls per frame: %d;",
				rDInfo.unDrawCalls);
			ImGui::Separator();
			ImGui::TreePop();
		}
		ImGui::End();
	}
	// --==</GuiOfRenderer>==--

	// --==<GuiOfConsole>==--
	GuiOfConsole::GuiOfConsole():
		chrCmdBuf(DArray<Char>(128, '\0')) { }
	// --core_methods
	void GuiOfConsole::OnDraw() {
		if (!bIsEnabled) return;
		ImGui::Begin("Console");
		ImGui::Text("cmd_mode: %s", CmdMode == CMD_NATIVE ? "native" : CmdMode == CMD_LUA ? "lua" : "none");
		if (ImGui::InputText("Command:", &chrCmdBuf[0], chrCmdBuf.size())) {
			if (strlen(&chrCmdBuf[0]) > chrCmdBuf.size() - 8) {
				chrCmdBuf.resize(chrCmdBuf.size() * 2);
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Accept") || IOSys::GetKeyReleased(NW_KEY_ENTER_10)) {
			strCmdBuf.push_back(&chrCmdBuf[0]);
			strCmdBuf.push_back("--==<>==--");
			while (strCmdBuf.size() > 24) { strCmdBuf.erase(strCmdBuf.begin()); }
			
			StrStream strStream(*(strCmdBuf.end() - 2));
			while (strStream >> &chrCmdBuf[0]) {
				if (strcmp(&chrCmdBuf[0], "cmd_mode") == 0) {
					strStream >> &chrCmdBuf[0];
					if (strcmp(&chrCmdBuf[0], "native") == 0) { CmdMode = CMD_NATIVE; }
					else if (strcmp(&chrCmdBuf[0], "lua") == 0) { CmdMode = CMD_LUA; }
					else { CmdMode = CMD_NONE; }
				}
				else if (CmdMode == CMD_LUA) {
					LuaVM::Get().RunScriptStr(&strStream.str()[0]);
				}
				else if (CmdMode == CMD_NATIVE) {
					//
				}
			}
			memset(&chrCmdBuf[0], 0, chrCmdBuf.size()); chrCmdBuf.resize(100);
		}
		
		for (Int32 li = strCmdBuf.size() - 1; li >= 0; li--) {
			ImGui::Text("%d: %s", li, &(strCmdBuf[li])[0]);
		}

		ImGui::End();
	}
	// --==</GuiOfConsole>==--
}
// --==</GuiOfGlobal>==--

// --==<GuiOfEditors>==--
namespace NW
{
	// --==<GuiOfCodeEditor>==--
	GuiOfCodeEditor::GuiOfCodeEditor() :
		strCodeBuf(DArray<char>(1024 * 4, 0)) { }
	
	// --setters
	void GuiOfCodeEditor::SetContext(ACodeChunk* pContext) {
		if (this->pContextScr = dynamic_cast<LuaScript*>(pContext)) {
			bIsEnabled = true;
			strcpy(&strCodeBuf[0], &pContextScr->GetCode()[0]);
			pContextShd = nullptr;
		}
		else if (this->pContextShd = dynamic_cast<AShader*>(pContext)) {
			bIsEnabled = true;
			strcpy(&strCodeBuf[0], &pContextShd->GetCode()[0]);
		}
		else {
			memset(&strCodeBuf[0], 0, strCodeBuf.size());
			bIsEnabled = false;
		}
	}
	// --core_methods
	void GuiOfCodeEditor::OnDraw()
	{
		if (!bIsEnabled) return;
		ImGui::Begin("Code_Editor", &bIsEnabled, ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Save code...")) {
					String strFPath = DataSys::FDialog_save("all_files(*.*)\0*.*\0lua_scripts(*.lua)\0*.lua\0opengl_shader(*.glsl)\0(*.glsl)\0\0");
					if (!strFPath.empty()) {
						String strTemp;
						DataSys::SaveF_string(&strFPath[0], &strCodeBuf[0], strCodeBuf.size());
						strCodeBuf.resize(strTemp.size() + 8);
						strcpy(&strCodeBuf[0], &strTemp[0]);
					}
				}
				else if (ImGui::MenuItem("Load code...")) {
					String strFPath = DataSys::FDialog_load("all_files(*.*)\0*.*\0lua_scripts(*.lua)\0*.lua\0opengl_shader(*.glsl)\0(*.glsl)\0\0");
					if (!strFPath.empty()) {
						String strTemp;
						DataSys::LoadF_string(&strFPath[0], strTemp);
						strCodeBuf.resize(strTemp.size() + 8);
						strcpy(&strCodeBuf[0], &strTemp[0]);
					}
				}
				ImGui::EndMenu();
			}
			else if (ImGui::BeginMenu("Context")) {
				if (ImGui::BeginCombo("Set shader", pContextShd == nullptr ? "No shader" : &pContextShd->GetName()[0])) {
					auto& Shaders = DataSys::GetDataResources<AShader>();
					for (auto& pShd : Shaders) {
						if (ImGui::Button(&pShd.second->GetName()[0])) {
							this->SetContext(pShd.second);
						}
					}
					ImGui::EndCombo();
				}
				if (ImGui::MenuItem("Reset")) { SetContext(nullptr); }
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		if (pContextScr != nullptr) {
			ImGui::PushID(pContextScr->GetId());
			ImGui::Text("Lua script: %s;", &pContextScr->GetName()[0]);
			if (ImGui::InputTextMultiline("", &strCodeBuf[0], strCodeBuf.size(),
				{ ImGui::GetContentRegionAvail().x - 32.0f, ImGui::GetContentRegionAvail().y - 64.0f }, ImGuiInputTextFlags_Multiline)) {
				if (strlen(&strCodeBuf[0]) > strCodeBuf.size() - 8) { strCodeBuf.resize(strCodeBuf.size() * 2); }
			}
			else if (ImGui::Button("Edit")) { pContextScr->SetCode(&strCodeBuf[0]); }

			ImGui::PopID();
		}
		else if (pContextShd != nullptr) {
			ImGui::PushID(pContextShd->GetId());
			ImGui::Text("Shader: %s;", &pContextShd->GetName()[0]);
			if (ImGui::InputTextMultiline("", &strCodeBuf[0], strCodeBuf.size(),
				{ ImGui::GetContentRegionAvail().x - 32.0f, ImGui::GetContentRegionAvail().y - 64.0f }, ImGuiInputTextFlags_Multiline)) {
				if (strlen(&strCodeBuf[0]) > strCodeBuf.size() - 8) { strCodeBuf.resize(strCodeBuf.size() * 2); }
			}
			else if (ImGui::Button("Edit")) { pContextShd->SetCode(&strCodeBuf[0]); pContextShd->Compile(); }
			
			if (ImGui::TreeNodeEx("--==<vertex_buffer_layout>==--", GUI_DEFAULT_TREE_FLAGS)) {
				auto& rElems = pContextShd->GetVertexLayout().GetElems();
				for (UInt16 bei = 0; bei < rElems.size(); bei++) {
					auto& rBE = rElems[bei];
					ImGui::Text("%dth attribute:\nName: %s;\tType: %s;\nCount = %d;\tIs%snormalized", bei,
						&rBE.strName[0],
						rBE.sdType == SDT_BOOL ? "boolean" :
						rBE.sdType == SDT_INT8 ? "byte" : rBE.sdType == SDT_UINT8 ? "unsigned byte" :
						rBE.sdType == SDT_INT16 ? "short" : rBE.sdType == SDT_UINT16 ? "unsigned short" :
						rBE.sdType == SDT_INT32 ? "integer" : rBE.sdType == SDT_UINT32 ? "unsigned integer" :
						rBE.sdType == SDT_FLOAT32 ? "float" : rBE.sdType == SDT_FLOAT64 ? "double" :
						"unknown",
						rBE.unCount, rBE.bNormalized ? " " : " not ");
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNodeEx("--==<globals>==--", GUI_DEFAULT_TREE_FLAGS)) {
				for (auto& rPrm : pContextShd->GetGlobals()) { ImGui::Text("%dth global: %s", rPrm.second, rPrm.first); }
				ImGui::TreePop();
			}
			if (ImGui::TreeNodeEx("--==<shader_buffer_layout>==--", GUI_DEFAULT_TREE_FLAGS)) {
				auto& rLayout = pContextShd->GetShdLayout();
				for (auto& rBlk : rLayout.GetBlocks()) {
					ImGui::Text("%dth block\nName: %s\nSize = %d;\tOffset = %d", rBlk.unBindPoint, rBlk.strName, rBlk.szAll, rBlk.szOffset);
					if (ImGui::TreeNodeEx("Elements", GUI_DEFAULT_TREE_FLAGS)) {
						for (UInt16 bei = 0; bei < rBlk.BufElems.size(); bei++) {
							auto& rBE = rBlk.BufElems[bei];
							ImGui::Text("%dth attribute:\nName: %s;\tType: %s;\nCount = %d;\tIs%snormalized", bei,
								&rBE.strName[0], SDType_GetString(rBE.sdType), rBE.unCount, rBE.bNormalized ? " " : " not ");
						}
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
			
			ImGui::PopID();
		}
		ImGui::End();
	}
	// --==</GuiOfCodeEditor>==--
	
	// --==<GuiOfGMaterialEditor>==--
	GuiOfGMaterialEditor::GuiOfGMaterialEditor() :
		pContext(DataSys::GetDataRes<GMaterial>("gmt_batch_3d"))
	{ }
	// --setters
	void GuiOfGMaterialEditor::SetContext(GMaterial* pContext) {
		this->pContext = pContext;
		if (pContext == nullptr) {
			bIsEnabled = false;
		}
		else {
			if (pContext == nullptr) { pContext = DataSys::GetDataRes<GMaterial>("gmt_batch_3d"); }
			bIsEnabled = true;
		}
	}
	// --core_methods
	void GuiOfGMaterialEditor::OnDraw() {
		if (!bIsEnabled) return;
		ImGui::Begin("GMaterial_Editor", &bIsEnabled, ImGuiWindowFlags_MenuBar);
		if (pContext == nullptr) { ImGui::End(); return; }

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Save...")) {
					String strFPath = DataSys::FDialog_save("all_files(*.*)\0*.*\0graphics_material(*.gmt)\0(*.gmt)\0\0");
					if (!strFPath.empty()) { }
				}
				else if (ImGui::MenuItem("Load...")) {
					String strFPath = DataSys::FDialog_load("all_files(*.*)\0*.*\0graphics_material(*.gmt)\0(*.gmt)\0\0");
					if (!strFPath.empty()) { }
				}
				ImGui::EndMenu();
			}
			else if (ImGui::BeginMenu("Context")) {
				if (ImGui::BeginCombo("Set gmaterial", pContext == nullptr ? "No material" : &pContext->GetName()[0])) {
					auto& GMaterials = DataSys::GetDataResources<GMaterial>();
					for (auto& itGMtl : GMaterials) {
						if (ImGui::Button(&itGMtl.second->GetName()[0])) {
							this->SetContext(itGMtl.second);
						}
					}
					ImGui::EndCombo();
				}
				if (ImGui::MenuItem("Reset")) { SetContext(nullptr); }
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		strcpy(strContextName, pContext->GetName());
		if (ImGui::InputText("Name", &strContextName[0], 128)) { pContext->SetName(strContextName); }

		if (ImGui::TreeNodeEx("-- colors", GUI_DEFAULT_TREE_FLAGS)) {
			for (auto itClr = pContext->GetColors().begin(); itClr != pContext->GetColors().end(); itClr++) {
				ImGui::ColorEdit4(&itClr->first[0], &itClr->second[0]);
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("-- textures", GUI_DEFAULT_TREE_FLAGS)) {
			auto itTexMtl = pContext->GetTextures().begin();
			for (Int32 txi = 0; txi < pContext->GetTexCount(); txi++, itTexMtl++) {
				auto pTex = itTexMtl->second;
				ImGui::PushID(&itTexMtl->first[0]);
				const ImageInfo rImgInfo = pTex->GetImgInfo();
				ImGui::Text("%d texture: Name: %s\nRender Id = %d; Slot = %d;",
					txi, &itTexMtl->first[0], pTex->GetRenderId(), pTex->GetTexSlot());
				ImGui::Text("Width = %d; Height = %d; Depth = %d;",
					rImgInfo.nWidth, rImgInfo.nHeight, rImgInfo.nDepth);
				ImGui::Image(reinterpret_cast<void*>(pTex->GetRenderId()),
					ImVec2{ 64.0f * rImgInfo.nWidth / rImgInfo.nHeight, 64.0f });
				if (ImGui::BeginCombo("Change texture", &(pTex->GetName())[0])) {
					auto& Textures = DataSys::GetDataResources<ATexture>();
					for (auto& itTex : Textures) {
						const ImageInfo rImgInfoLoc = itTex.second->GetImgInfo();
						if (ImGui::ImageButton(reinterpret_cast<void*>(itTex.second->GetRenderId()),
							ImVec2{ 64.0f * (rImgInfoLoc.nWidth / rImgInfoLoc.nHeight), 64.0f })) {
							pContext->SetTexture(itTex.second, &itTexMtl->first[0]);
						}
					} ImGui::EndCombo();
				}
				ImGui::PopID();
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx("-- shader", GUI_DEFAULT_TREE_FLAGS)) {
			AShader* pShader = pContext->GetShader();
			ImGui::Text("ID = %d;\nName: %s;", pShader->GetId(), pShader->GetName());
			if (ImGui::Button("Code editor")) { GuiOfCodeEditor::Get().SetContext(pShader); }
			if (ImGui::BeginCombo("Change shader", &pShader->GetName()[0])) {
				auto& Shaders = DataSys::GetDataResources<AShader>();
				for (auto& pShd : Shaders) {
					if (ImGui::Button(&pShd.second->GetName()[0])) { pContext->SetShader(pShd.second); }
				}
				ImGui::EndCombo();
			}
			ImGui::TreePop();
		}

		ImGui::End();
	}
	// --==</GuiOfGMaterialEditor>==--

	// --==<GuiOfSpriteEditor>==--
	GuiOfSpriteEditor::GuiOfSpriteEditor() :
		pVtxBuf(nullptr),
		pIndBuf(nullptr),
		pShader(AShader::Create("shd_sprite_editor"))
	{
		pVtxBuf.reset(AVertexBuf::Create(sizeof(float) * (2 + 2), nullptr));
		UInt32 IndData[] = { 0, 1, 2,	2, 3, 0 };
		pIndBuf.reset(AIndexBuf::Create(sizeof(IndData), &IndData[0]));
		
		pShader->LoadF("D:/dev/native_world/nw_engine/src_glsl/display_img.glsl");
		pVtxBuf->SetLayout(pShader->GetVertexLayout());

		FrameBufInfo fbInfo;
		fbInfo.unWidth = 64;
		fbInfo.unHeight = 64;
		fbInfo.unSamples = 1;
		pFrameBuf = AFrameBuf::Create("fmb_sprite_editor", fbInfo);
	}
	// --setters
	void GuiOfSpriteEditor::SetContext(ATexture2d* pContext) {
		this->pContext = pContext;
		if (pContext == nullptr) {
			bIsEnabled = false;
			pContext = DataSys::GetDataRes<ATexture2d>("tex_white_solid");
		}
		else {
			bIsEnabled = true;
			/*
			V2i xyTexCrd = pContext->GetTexCoord_0_1();
			V2i whTexSize = pContext->GetTexSize_0_1();
			float vtxData[] = {
				-1.0f,	-1.0f,		xyTexCrd.x + 0.0f,	xyTexCrd.y + 0.0f,
				-1.0f,	1.0f,		xyTexCrd.x + 0.0f,	xyTexCrd.y + whTexSize.y,
				1.0f,	1.0f,		xyTexCrd.x + whTexSize.x,	xyTexCrd.y + whTexSize.y,
				1.0f,	-1.0f,		xyTexCrd.x + whTexSize.x,	xyTexCrd.x + 0.0f
			};
			pVtxBuf->SetSubData(sizeof(vtxData), &vtxData[0]);*/
		}
		strcpy(strContextName, pContext->GetName());
		ImgInfo = pContext->GetImgInfo();
		TexInfo = pContext->GetTexInfo();
		nAspectRatio = ImgInfo.nWidth / ImgInfo.nHeight;
	}
	// --core_methods
	void GuiOfSpriteEditor::OnDraw() {
		if (!bIsEnabled) return;
		ImGui::Begin("Sprite_Editor", &bIsEnabled, ImGuiWindowFlags_MenuBar);

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Save...")) {
					String strFPath = DataSys::FDialog_save("all_files\0*.*\0images\0*.png");
					if (!strFPath.empty()) { pContext->SaveF(&strFPath[0]); }
				}
				else if (ImGui::MenuItem("Load...")) {
					String strFPath = DataSys::FDialog_load("all_files\0*.*\0images\0*.png");
					if (!strFPath.empty()) { pContext->LoadF(&strFPath[0]); }
				}
				ImGui::EndMenu();
			}
			else if (ImGui::BeginMenu("Context")) {
				if (ImGui::MenuItem("Set texture")) {
					ImGui::OpenPopup("set_context");
					if (ImGui::BeginPopup("set_context")) {
						auto& Textures = DataSys::GetDataResources<ATexture2d>();
						for (auto& itTex : Textures) {
							ImGui::Text(itTex.second->GetName());
							const ImageInfo& rImgInfo = itTex.second->GetImgInfo();
							if (ImGui::ImageButton(reinterpret_cast<void*>(itTex.second->GetRenderId()),
								ImVec2{ 64.0f * rImgInfo.nWidth / rImgInfo.nHeight, 64.0f })) {
								SetContext(itTex.second);
							}
						}
						ImGui::EndPopup();
					}
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		if (pContext == nullptr) { ImGui::End(); return; }

		if (ImGui::InputText("name", strContextName, 128)) { pContext->SetName(strContextName); }
		ImGui::Text("context id: %d; render id: %d", pContext->GetId(), pContext->GetRenderId());
		ImGui::Text("current: width = %d; height = %d", pContext->GetWidth(), pContext->GetHeight());

		if (ImGui::Button("apply")) { pContext->SetInfo(ImgInfo); pContext->SetInfo(TexInfo); pContext->Remake(); }

		if (ImGui::TreeNodeEx("texture_info", GUI_DEFAULT_TREE_FLAGS)) {
			
			if (ImGui::BeginCombo("filter_min", TexInfo.FilterMin == TC_FILTER_LINEAR ? "linear" :
				TexInfo.FilterMin == TC_FILTER_NEAREST ? "nearest" : "none")) {
				if (ImGui::MenuItem("linear")) { TexInfo.FilterMin = TC_FILTER_LINEAR; }
				else if (ImGui::MenuItem("nearest")) { TexInfo.FilterMin = TC_FILTER_NEAREST; }
				ImGui::EndCombo();
			}
			else if (ImGui::BeginCombo("filter_mag", TexInfo.FilterMag == TC_FILTER_LINEAR ? "linear" :
				TexInfo.FilterMag == TC_FILTER_NEAREST ? "nearest" : "none")) {
				if (ImGui::MenuItem("linear")) { TexInfo.FilterMag = TC_FILTER_LINEAR; }
				else if (ImGui::MenuItem("nearest")) { TexInfo.FilterMag = TC_FILTER_NEAREST; }
				ImGui::EndCombo();
			}

			else if (ImGui::BeginCombo("wrap_s", TexInfo.WrapTypeS == TC_WRAP_REPEAT ? "repeat" : TexInfo.WrapTypeS == TC_WRAP_CLAMP ? "clamp" : "none")) {
				if (ImGui::MenuItem("clamp")) { TexInfo.WrapTypeS = TC_WRAP_CLAMP; }
				else if (ImGui::MenuItem("repeat")) { TexInfo.WrapTypeS = TC_WRAP_REPEAT; }
				ImGui::EndCombo();
			}
			else if (ImGui::BeginCombo("wrap_t", TexInfo.WrapTypeT == TC_WRAP_REPEAT ? "repeat" : TexInfo.WrapTypeT == TC_WRAP_CLAMP ? "clamp" : "none")) {
				if (ImGui::MenuItem("clamp")) { TexInfo.WrapTypeT = TC_WRAP_CLAMP; }
				else if (ImGui::MenuItem("repeat")) { TexInfo.WrapTypeT = TC_WRAP_REPEAT; }
				ImGui::EndCombo();
			}

			else if (ImGui::BeginCombo("format", TexInfo.Format == TC_FORMAT_RGB ? "rgb" : TexInfo.Format == TC_FORMAT_RGBA ? "rgba" :
				TexInfo.Format == TC_FORMAT_RED ? "grayscale" : "none")) {
				if (ImGui::MenuItem("rgb")) { TexInfo.Format = TC_FORMAT_RGB; TexInfo.InterFormat = TC_FORMAT_RGB; }
				else if (ImGui::MenuItem("rgba")) { TexInfo.Format = TC_FORMAT_RGBA; TexInfo.InterFormat = TC_FORMAT_RGBA; }
				ImGui::EndCombo();
			}

			ImGui::TreePop();
		}
		ImGui::Image(reinterpret_cast<void*>(pContext->GetRenderId()),
			ImVec2{ static_cast<float>(64.0f * nAspectRatio), static_cast<float>(64.0f) });
		if (ImGui::TreeNodeEx("Sprites", GUI_DEFAULT_TREE_FLAGS)) {

			/*if (ImGui::DragInt2("Top left", &pContext->xyTexCrd[0], 0.1f, 0.0, pContext->GetOverTexSize().y) ||
				ImGui::DragInt2("Bottom right", &pContext->whTexSize[0], 0.1f, pContext->GetOverTexSize().y, pContext->pOverTex->GetWidth())) {
				V2i xyTexCrd = pContext->GetTexCoord_0_1();
				V2i whTexSize = pContext->GetTexSize_0_1();
				float vtxData[] = {
					-1.0f,	-1.0f,		xyTexCrd.x + 0.0f,	xyTexCrd.y + 0.0f,
					-1.0f,	1.0f,		xyTexCrd.x + 0.0f,	xyTexCrd.y + whTexSize.y,
					1.0f,	1.0f,		xyTexCrd.x + whTexSize.x,	xyTexCrd.y + whTexSize.y,
					1.0f,	-1.0f,		xyTexCrd.x + whTexSize.x,	xyTexCrd.x + 0.0f
				};
				pVtxBuf->SetSubData(sizeof(vtxData), &vtxData[0]);
			}*/

			V2i whDisplaySize = { ImGui::GetContentRegionAvail().x * nAspectRatio, ImGui::GetContentRegionAvail().y - 32.0f };

			const FrameBufInfo& rFBInfo = pFrameBuf->GetInfo();
			if (rFBInfo.unHeight != whDisplaySize.x || rFBInfo.unWidth != whDisplaySize.x) {
				pFrameBuf->SetSizeWH(whDisplaySize.x, whDisplaySize.y);
			}

			pFrameBuf->Bind();
			pFrameBuf->Clear();

			pShader->Enable();
			pContext->Bind(0);
			pShader->SetInt("unf_tex", 0);

			pVtxBuf->Bind();
			pIndBuf->Bind();
			DrawEngine::GetGApi()->DrawIndexed(pIndBuf->GetDataSize() / sizeof(UInt32));
			pIndBuf->Unbind();
			pVtxBuf->Unbind();

			pContext->Unbind();
			pShader->Disable();
			pFrameBuf->Unbind();

			ImGui::Image(reinterpret_cast<void*>(pFrameBuf->GetColorAttachment()->GetRenderId()),
				ImVec2{ static_cast<float>(whDisplaySize.x), static_cast<float>(whDisplaySize.y) });
			ImGui::TreePop();
		}

		ImGui::End();
	}
	// --==</GuiOfSpriteEditor>==--

	// --==<GuiOfEntityEditor>==--
	// -- Components
	/// OnDraw Abstract EntityComponent
	/// Description:
	/// -- Gets any inherited of AEntityCmp component and draws default tree with "settings" button
	/// -- All individual GUI for particular component can be implemented inside callback function
	/// -- Because a callback can be simple function or a lambda expression, I use templated CBType
	/// -- So, to draw GUI of some component, we need to give a component and the callback function
	/// which will be called in the tree node
	template <typename CBType> inline void GuiOfEntityEditor::OnDraw(AEntityCmp* pACmp, CBType& fnCallback) {
		if (pACmp == nullptr) return;
		ImGui::PushID(pACmp->GetCmpId());
		bool bOpened0 = ImGui::TreeNodeEx(pACmp->GetTypeName(), GUI_DEFAULT_TREE_FLAGS);
		bool bEnabled0 = pACmp->IsEnabled();
		if (ImGui::Checkbox("Is enabled", &bEnabled0)) { pACmp->SetEnabled(bEnabled0); }
		if (ImGui::Button("Settings")) {
			ImGui::OpenPopup("SettingsDS");
		} if (ImGui::BeginPopup("SettingsDS")) {
			if (ImGui::MenuItem("Remove")) {
				pACmp->GetEntity()->RemoveAComponent(pACmp->GetTypeId());
				pACmp = nullptr;
			}
			ImGui::EndPopup();
		}
		if (bOpened0 && pACmp != nullptr) {
			ImGui::Text("ID: %d", pACmp->GetCmpId());
			ImGui::Separator();
			fnCallback();
			ImGui::TreePop();
		}
		ImGui::PopID();
	}
	// -- Particular component GUI specializations
	template <> void GuiOfEntityEditor::OnDraw<LuaScriptCmp>(LuaScriptCmp* pLSCmp) {
		static auto cbLSCmp = [&]()->void {
			if (ImGui::Button("Code editor")) {
				GuiOfCodeEditor::Get().SetContext(pLSCmp->GetScript());
			}
		};
		OnDraw(pLSCmp, cbLSCmp);
	}
	template <> void GuiOfEntityEditor::OnDraw<ATransformCmp>(ATransformCmp* pTFCmp) {
		static auto cbTFCmp = [&]()->void {
			V3f xyzCrd = pTFCmp->GetCoord();
			V3f xyzRtn = pTFCmp->GetRotation();
			V3f xyzScl = pTFCmp->GetScale();
			if (ImGui::DragFloat3("Coordinate", &xyzCrd[0], 0.1f))	pTFCmp->SetCoord(xyzCrd);
			if (ImGui::DragFloat3("Rotation", &xyzRtn[0], 0.1f))	pTFCmp->SetRotation(xyzRtn);
			if (ImGui::DragFloat3("Scale", &xyzScl[0], 0.05f))		pTFCmp->SetScale(xyzScl);
		};
		OnDraw(pTFCmp, cbTFCmp);
	}
	template <> void GuiOfEntityEditor::OnDraw<AGraphicsCmp>(AGraphicsCmp* pGCmp) {
		static auto cbGCmp = [&]()->void {
			Int32 nDrawOrder = pGCmp->unDrawOrder;
			if (ImGui::InputInt("Draw order", &nDrawOrder)) { pGCmp->unDrawOrder = nDrawOrder; }
			ImGui::Separator();
			if (ImGui::TreeNodeEx("--==<Material>==--", GUI_DEFAULT_TREE_FLAGS)) {
				GMaterial* pGMtl = pGCmp->GetDrawable()->pGMtl;
				Char cBuffer[128]{ 0 };
				strcpy_s(cBuffer, pGMtl->GetName());
				if (ImGui::InputText("Name", &cBuffer[0], 128)) { pGMtl->SetName(cBuffer); }

				if (ImGui::TreeNodeEx("-- colors", GUI_DEFAULT_TREE_FLAGS)) {
					for (auto itClr = pGMtl->GetColors().begin(); itClr != pGMtl->GetColors().end(); itClr++) {
						ImGui::ColorEdit4(&itClr->first[0], &itClr->second[0]);
					}
					ImGui::TreePop();
				}
				if (ImGui::TreeNodeEx("-- textures", GUI_DEFAULT_TREE_FLAGS)) {
					auto itTexMtl = pGMtl->GetTextures().begin();
					for (Int32 txi = 0; txi < pGMtl->GetTexCount(); txi++, itTexMtl++) {
						auto pTex = itTexMtl->second;
						ImGui::PushID(&itTexMtl->first[0]);
						const ImageInfo rImgInfo = pTex->GetImgInfo();
						ImGui::Text("%d texture: Name: %s\nRender Id = %d; Slot = %d;",
							txi, &itTexMtl->first[0], pTex->GetRenderId(), pTex->GetTexSlot());
						ImGui::Text("Width = %d; Height = %d; Depth = %d;",
							rImgInfo.nWidth, rImgInfo.nHeight, rImgInfo.nDepth);
						ImGui::Image(reinterpret_cast<void*>(pTex->GetRenderId()),
							ImVec2{ 64.0f * rImgInfo.nWidth / rImgInfo.nHeight, 64.0f });
						if (ImGui::BeginCombo("Change texture", &(pTex->GetName())[0])) {
							auto& Textures = DataSys::GetDataResources<ATexture>();
							for (auto& itTex : Textures) {
								const ImageInfo rImgInfoLoc = itTex.second->GetImgInfo();
								if (ImGui::ImageButton(reinterpret_cast<void*>(itTex.second->GetRenderId()),
									ImVec2{ 64.0f * (rImgInfoLoc.nWidth / rImgInfoLoc.nHeight), 64.0f })) {
									pGMtl->SetTexture(itTex.second, &itTexMtl->first[0]);
								}
							} ImGui::EndCombo();
						}
						ImGui::PopID();
					}
					ImGui::TreePop();
				}
				
				if (ImGui::TreeNodeEx("-- shader", GUI_DEFAULT_TREE_FLAGS)) {
					AShader* pShader = pGMtl->GetShader();
					ImGui::Text("ID = %d;\nName: %s;", pShader->GetId(), pShader->GetName());
					if (ImGui::Button("Code editor")) { GuiOfCodeEditor::Get().SetContext(pShader); }
					if (ImGui::BeginCombo("Change shader", &pShader->GetName()[0])) {
						auto& Shaders = DataSys::GetDataResources<AShader>();
						for (auto& pShd : Shaders) {
							if (ImGui::Button(&pShd.second->GetName()[0])) { pGMtl->SetShader(pShd.second); }
						}
						ImGui::EndCombo();
					}
					ImGui::TreePop();
				}
				if (ImGui::BeginCombo("Change material", &pGMtl->GetName()[0])) {
					auto& GMaterials = DataSys::GetDataResources<GMaterial>();
					for (auto& itGMtl : GMaterials) {
						if (ImGui::Button(&itGMtl.second->GetName()[0])) { pGCmp->GetDrawable()->pGMtl = itGMtl.second; }
					}
					ImGui::EndCombo();
				}
				ImGui::TreePop();
			}
		};
		OnDraw(pGCmp, cbGCmp);
	}
	template <> void GuiOfEntityEditor::OnDraw<ACollider2dCmp>(ACollider2dCmp* pC2Cmp) {
		static auto cbC2Cmp = [&]()->void {
			bool bIsCollided = pC2Cmp->GetCollision();
			if (ImGui::Checkbox("Is Collided", &bIsCollided)) { pC2Cmp->SetCollision(bIsCollided); }
			ImGui::Separator();
			if (Rectangle* pRect = dynamic_cast<Rectangle*>(pC2Cmp->GetShape())) {
				ImGui::Text("-- Rectangle");
				ImGui::Separator();
			}
		};
		OnDraw(pC2Cmp, cbC2Cmp);
	}
	// --setters
	void GuiOfEntityEditor::SetContext(AEntity* pContext) {
		this->pContext = pContext;
		if (pContext == nullptr) {
			bIsEnabled = false;
		}
		else {
			bIsEnabled = true;
		}
	}
	// --core_methods
	void GuiOfEntityEditor::OnDraw() {
		if (!bIsEnabled) return;
		ImGui::Begin("Entity_Editor", &bIsEnabled);
		if (pContext == nullptr) { ImGui::End(); return; }

		bool bEnabled0 = pContext->IsEnabled();
		if (ImGui::Checkbox("Is enabled", &bEnabled0)) { pContext->SetEnabled(bEnabled0); }
		char cNameBuf[128]{ 0 };
		strcpy_s(cNameBuf, &pContext->GetName()[0]);
		if (ImGui::InputText("Name", &cNameBuf[0], 128)) { pContext->SetName(cNameBuf); }
		ImGui::Text("ID: %d", pContext->GetId());
		ImGui::Separator();
		OnDraw<LuaScriptCmp>(pContext->GetComponent<LuaScriptCmp>());
		OnDraw<ATransformCmp>(pContext->GetComponent<ATransformCmp>());
		OnDraw<AGraphicsCmp>(pContext->GetComponent<AGraphicsCmp>());
		OnDraw<ACollider2dCmp>(pContext->GetComponent<ACollider2dCmp>());
		ImGui::Separator();
		if (ImGui::Button("Add Component")) {
			ImGui::OpenPopup("Add Component");
		}
		if (ImGui::BeginPopup("Add Component")) {
			if (ImGui::MenuItem("Transform2d Component")) {
				Transform2dCmp* pTF2dCmp = MemSys::NewT<Transform2dCmp>(*pContext);
			}
			else if (ImGui::MenuItem("Transform3d Component")) {
				Transform3dCmp* pTF3dCmp = MemSys::NewT<Transform3dCmp>(*pContext);
			}
			else if (ImGui::MenuItem("Script Component")) {
				LuaScriptCmp* pLSCmp = MemSys::NewT<LuaScriptCmp>(*pContext);
			}
			else if (ImGui::MenuItem("Graphics2d Component")) {
				Graphics2dCmp* pG2dCmp = MemSys::NewT<Graphics2dCmp>(*pContext);
			}
			else if (ImGui::MenuItem("Collider2d Rect Component")) {
				Collider2dRectCmp* pC2dRCmp = MemSys::NewT<Collider2dRectCmp>(*pContext);
			}
			ImGui::EndPopup();
		}
		ImGui::End();
	}
	// --==</GuiOfEntityEditor>==--
}
// --==</GuiOfEditors>==--

// --==<GuiOfSceneEditor>==--
namespace NW
{
	GuiOfSceneEditor::GuiOfSceneEditor() :
		pIcoCamera(ATexture2d::Create("ico_editor_camera"))
	{
		pIcoCamera->LoadF("data_res/graphics/images/ico_eye.png");
	}
	// --core_methods
	void GuiOfSceneEditor::OnDraw() {
		if (!bIsEnabled) { return; }
		ImGui::Begin("Scene_Editor", &bIsEnabled);

		if (ImGui::BeginPopupContextWindow(0, 1, false)) {
			AEntity* pEnt0 = nullptr;
			if (ImGui::MenuItem("Create Entity")) {
				pEnt0 = Scene::Get().CreateEntity();
				Transform2dCmp* pTF2dCmp = MemSys::NewT<Transform2dCmp>(*pEnt0);
				Graphics2dCmp* pG2dCmp = MemSys::NewT<Graphics2dCmp>(*pEnt0);
			} ImGui::EndPopup();
		}
		OnDraw(Scene::Get().GetOverEnts());
		ImGui::End();

		OnDraw(GCameraLad::Get().GetGCamera());
		ImGui::Begin("Scene_Frame");
		if (ImGui::ImageButton(reinterpret_cast<void*>(pIcoCamera->GetRenderId()),
			{ 32.0f * (static_cast<float>(pIcoCamera->GetWidth()) / static_cast<float>(pIcoCamera->GetHeight())), 32.0f })) {
			bIsEnabledCamera = true; }
		ImGui::Separator();
		
		UInt32 unSizeW = ImGui::GetContentRegionAvail().x;
		UInt32 unSizeH = ImGui::GetContentRegionAvail().y - 48.0f;
		AFrameBuf* pFrameBuf = Scene::Get().GetFrameBuf();
		
		ImGui::Image(reinterpret_cast<void*>(pFrameBuf->GetColorAttachment()->GetRenderId()),
			{ static_cast<float>(unSizeW), static_cast<float>(unSizeH) }, { 0.0f, 0.0f }, {1.0f, -1.0f});
		const FrameBufInfo& rfbInfo = pFrameBuf->GetInfo();
		ImGui::Separator();

		if (rfbInfo.unWidth != unSizeW || rfbInfo.unHeight != unSizeH) { Scene::Get().SetViewport(V4f{ 0, 0, unSizeW, unSizeH }); }
		ImGui::End();
	}
	inline void GuiOfSceneEditor::OnDraw(RefEnts& rRefEnts) {
		AEntity* pDestroyEnt = nullptr;
		for (auto itEnt : rRefEnts) {
			AEntity* pEntity = itEnt.second;
			bool bOpened = ImGui::TreeNodeEx(&pEntity->GetName()[0], GUI_DEFAULT_TREE_FLAGS);
			if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) { GuiOfEntityEditor::Get().SetContext(pEntity); }
			if (ImGui::BeginPopupContextItem(&pEntity->GetName()[0])) {
				if (ImGui::MenuItem("Create SubEnt")) {
					AEntity* pSubEnt = Scene::Get().CreateEntity();
					pEntity->AddSubEnt(pSubEnt);
					Transform2dCmp* pTF2dCmp = MemSys::NewT<Transform2dCmp>(*pSubEnt);
					Graphics2dCmp* pG2dCmp = MemSys::NewT<Graphics2dCmp>(*pSubEnt);
				}
				else if (ImGui::MenuItem("Destroy Entity")) {
					pDestroyEnt = pEntity;
				} ImGui::EndPopup();
			}
			if (bOpened) {
				if (pEntity->GetSubEnts().size() > 0) { OnDraw(pEntity->GetSubEnts()); }
				ImGui::TreePop();
			}
			if (pDestroyEnt != nullptr) {
				GuiOfEntityEditor::Get().SetContext(nullptr);
				Scene::Get().DestroyEntity(pDestroyEnt->GetId());
			}
		}
	}
	inline void GuiOfSceneEditor::OnDraw(GCamera* pGCamera) {
		if (!bIsEnabledCamera || pGCamera == nullptr) return;
		ImGui::Begin("Editor_Camera", &bIsEnabledCamera);
		if (ImGui::BeginCombo("Camera type", pGCamera->GetType() == GCamera::GCT_ORTHO ? "Orthographic" : "Perspective")) {
			if (ImGui::Selectable("Orthographic", pGCamera->GetType() == GCamera::GCT_ORTHO)) {
				pGCamera->SetType(GCamera::GCT_ORTHO);
			}
			else if (ImGui::Selectable("Perspective", pGCamera->GetType() == GCamera::GCT_PERSPECT)) {
				pGCamera->SetType(GCamera::GCT_PERSPECT);
			}
			ImGui::EndCombo();
		}
		if (ImGui::BeginCombo("Camera mode", pGCamera->GetMode() == GCamera::GCM_2D ? "2 dimentional" : "3 dimentional")) {
			if (ImGui::Selectable("2 dimentional", pGCamera->GetMode() == GCamera::GCM_2D)) {
				pGCamera->SetMode(GCamera::GCM_2D);
			}
			else if (ImGui::Selectable("3 dimentional", pGCamera->GetMode() == GCamera::GCM_3D)) {
				pGCamera->SetMode(GCamera::GCM_3D);
			}
			ImGui::EndCombo();
		}
		switch (pGCamera->GetMode()) {
		case GCamera::GCM_2D:
			ImGui::DragFloat2("::Coordinates", &pGCamera->xCrd, 0.1f);
			ImGui::DragFloat("::Rotations", &pGCamera->zRtn, 0.1f);
			break;
		case GCamera::GCM_3D:
			ImGui::DragFloat3("::Coordinates", &pGCamera->xCrd, 0.1f);
			ImGui::DragFloat3("::Rotations", &pGCamera->xRtn, 0.1f);
			break;
		}
		ImGui::DragFloat2("::Near and Far clips", &pGCamera->nNearClip, 0.1f);
		ImGui::Text("::Aspect Ratio: %f", pGCamera->nAspectRatio);
		switch (pGCamera->GetType()) {
		case GCamera::GCT_PERSPECT:
			ImGui::DragFloat("::Field Of View degree", &pGCamera->nViewField, 1.0f);
			break;
		case GCamera::GCT_ORTHO:
			ImGui::DragFloat("::ViewScale", &pGCamera->nViewScale, 0.1f);
			break;
		}
		ImGui::End();
	}
}
// --==</GuiOfSceneEditor>==--