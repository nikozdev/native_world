#include <nw_pch.hpp>
#include "nw_gui_of.h"

#include <ecs/nw_scene.h>
#include <ecs/nw_entity.h>
#include <ecs/nw_entity_cmp.h>

#include <glib/control/nw_graph_engine.h>
#include <glib/control/nw_graph_api.h>
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
	// --==<GuiOfCoreEngine>==--
	GuiOfCoreEngine::GuiOfCoreEngine() {
		pCoreState = CoreEngine::Get().GetState();
		pWindow = CoreEngine::Get().GetWindow();
	}
	GuiOfCoreEngine::~GuiOfCoreEngine() { }

	// --core_methods
	void GuiOfCoreEngine::OnDraw() {
		if (!bIsEnabled) return;
		ImGui::Begin("core_engine", &bIsEnabled);
		bWindow = ImGui::TreeNodeEx("--==<window>==--", GUI_DEFAULT_TREE_FLAGS);
		if (bWindow) {
			const WindowInfo& rWindowInfo = pWindow->GetWindowInfo();
			ImGui::Text("window_api: %s",
				rWindowInfo.WApiType == WAPI_GLFW ? "glfw" : rWindowInfo.WApiType == WAPI_COUT ? "console" :
				rWindowInfo.WApiType == WAPI_WIN ? "windows" : "None");
			if (ImGui::InputText("title", &strWindowTitle[0], 128)) { CoreEngine::Get().GetWindow()->SetTitle(&strWindowTitle[0]); }

			ImGui::Text("size: %dx%d;\naspect_ratio = %f;",
				rWindowInfo.unWidth, rWindowInfo.unHeight, rWindowInfo.nAspectRatio);
			ImGui::Text("cursor: x = %d; y = %d;", static_cast<Int32>(IOSys::Cursor.xMove), static_cast<Int32>(IOSys::Cursor.yMove));

			bool bVSync = rWindowInfo.bVSync;
			if (ImGui::Checkbox("vertical synchronization", &bVSync)) { CoreEngine::Get().GetWindow()->SetVSync(bVSync); }

			if (ImGui::BeginCombo("switch window", &pWindow->GetWindowInfo().strTitle[0])) {
				auto itWindow = CoreEngine::Get().GetWindow();
				if (ImGui::Selectable(&itWindow->GetWindowInfo().strTitle[0])) {
					pWindow = CoreEngine::Get().GetWindow();
					strcpy(&strWindowTitle[0], &pWindow->GetWindowInfo().strTitle[0]);
				} ImGui::EndCombo();
			}
			ImGui::TreePop();
		}
		bAppState = ImGui::TreeNodeEx("--==<core_state>==--", GUI_DEFAULT_TREE_FLAGS);
		if (bAppState) {
			if (ImGui::BeginCombo("switch state", &pCoreState->GetName()[0])) {
				auto pStates = CoreEngine::Get().GetStates();
				for (auto pEst : pStates) {
					if (ImGui::Selectable(&pEst->GetName()[0])) {
						CoreEngine::Get().SwitchState(&pEst->GetName()[0]);
					} ImGui::EndCombo();
				}
			}
			ImGui::TreePop();
		}
		bLuaVM = ImGui::TreeNodeEx("--==<lua_virtual_machiene>==--", GUI_DEFAULT_TREE_FLAGS);
		if (bLuaVM) {
			if (ImGui::Button("write info")) {
				LogSys::GetLogOut() << LuaVM::Get().GetInfo();
			}
			ImGui::TreePop();
		}
		ImGui::End();
	}
	// --==</GuiOfCoreEngine>==--
	// --==<GuiOfGraphEngine>==--
	GuiOfGraphEngine::GuiOfGraphEngine() {}
	GuiOfGraphEngine::~GuiOfGraphEngine() {}

	void GuiOfGraphEngine::OnDraw() {
		if (!bIsEnabled) return;

		ImGui::Begin("graph_engine", &bIsEnabled);
		bGContext = ImGui::TreeNodeEx("--==<gcontext>==--", GUI_DEFAULT_TREE_FLAGS);
		if (bGContext) {
			const GContextInfo& rGContextInfo = CoreEngine::Get().GetWindow()->GetGContext()->GetInfo();

			ImGui::Text("version: %s;\nrenderer: %s;"
				"\nvendor: %s;\nshading language: %s"
				"\nmax texture count: %d;\nmax vertex attributes: %d",
				rGContextInfo.strRenderer, rGContextInfo.strVersion,
				rGContextInfo.strVendor, rGContextInfo.strShadingLanguage,
				rGContextInfo.nMaxTextures, rGContextInfo.nMaxVertexAttribs);

			ImGui::TreePop();
		}
		bGApi = ImGui::TreeNodeEx("--==<gapi>==--", GUI_DEFAULT_TREE_FLAGS);
		if (bGApi) {
			AGraphApi* pGApi = GraphEngine::Get().GetGApi();
			ImGui::Text("Type: %s",
				pGApi->GetType() == GApiTypes::GAPI_OPENGL ? "OpenGL" : pGApi->GetType() == GApiTypes::GAPI_COUT ? "Console" :
				pGApi->GetType() == GApiTypes::GAPI_WIN ? "Windows" :
				"None");
			if (ImGui::DragFloat("line width", &nLineW, 0.1f)) GraphEngine::Get().GetGApi()->SetLineWidth(nLineW);
			if (ImGui::DragFloat("pixel size", &nPixelSz, 0.1f)) GraphEngine::Get().GetGApi()->SetPixelSize(nPixelSz);
			if (ImGui::BeginCombo("draw mode", &strDrawMode[0])) {
				if (ImGui::Selectable("MD_FILL")) {
					strDrawMode = "MD_FILL";
					GraphEngine::Get().GetGApi()->SetDrawMode(DrawModes::DM_FILL, FacePlanes::FP_FRONT_AND_BACK);
				}
				else if (ImGui::Selectable("MD_LINE")) {
					strDrawMode = "MD_LINE";
					GraphEngine::Get().GetGApi()->SetDrawMode(DrawModes::DM_LINE, FacePlanes::FP_FRONT_AND_BACK);
				}
				ImGui::EndCombo();
			} ImGui::Separator();
			ImGui::TreePop();
		}
		
		bStates = ImGui::TreeNodeEx("--==<gstates>==--");
		if (bStates) {
			GraphState* pState = GraphEngine::Get().GetStates().begin()->second;
			for (UInt16 dsi = 0; dsi < GraphEngine::Get().GetStates().size(); dsi++) {
				pState++;
				ImGui::Text("%dth state:\nname: %s", dsi, pState->GetName());
			}
			if (ImGui::BeginPopupContextWindow("state_props", ImGuiPopupFlags_MouseButtonRight)) {
				if (ImGui::MenuItem("create state")) {
					ImGui::OpenPopup("state_creation");
					if (ImGui::BeginPopup("state_creation")) {
						ImGui::InputText("state name", strStateName, 256);
						if (ImGui::Button("add state")) { GraphEngine::Get().AddState(MemSys::NewT<GraphState>(strStateName)); }
						ImGui::EndPopup();
					}
				}
				ImGui::EndPopup();
			}
		}

		const GraphEngineInfo& rDInfo = GraphEngine::Get().GetInfo();
		ImGui::Text("vertex data\n::count: %d;\n::size in bytes: %d/%d;\n",
			rDInfo.unVtx,
			rDInfo.szVtx, rDInfo.szMaxVtx);
		ImGui::InputInt("new max vertex data size", &szMaxVtx);
		if (ImGui::Button("change max vertex data size")) GraphEngine::Get().SetMaxVtxSize(szMaxVtx);

		ImGui::Text("index data\n::count: %d;\n::size in bytes: %d/%d;\n",
			rDInfo.unIdx,
			rDInfo.szIdx, rDInfo.szMaxIdx);
		ImGui::InputInt("new max index data size", &szMaxIdx);
		if (ImGui::Button("change max index data size")) GraphEngine::Get().SetMaxIdxSize(szMaxIdx);

		ImGui::Text("shader data\n::size in bytes: %d/%d;\n",
			rDInfo.szShd, rDInfo.szMaxShd);
		ImGui::InputInt("new max shader data size", &szMaxShd);
		if (ImGui::Button("change max shader data size")) GraphEngine::Get().SetMaxShdSize(szMaxShd);

		unMaxTex = rDInfo.unMaxTex;
		ImGui::Text("textures\n::slots: %d/%d;",
			rDInfo.unTex, rDInfo.unMaxTex);
		ImGui::InputInt("new max texture slots", &unMaxTex);
		if (ImGui::Button("change max texture slots")) GraphEngine::Get().SetMaxTexCount(unMaxTex);

		ImGui::Text("draw calls per frame: %d;",
			rDInfo.unDrawCalls);
		ImGui::Separator();

		ImGui::End();
	}
	// --==</GuiOfGraphEngine>==--

	// --==<GuiOfCmdEngine>==--
	GuiOfCmdEngine::GuiOfCmdEngine() :
		chrCmdBuf(DArray<Char>(128, '\0')) { }
	GuiOfCmdEngine::~GuiOfCmdEngine() { }
	// --core_methods
	void GuiOfCmdEngine::OnDraw() {
		if (!bIsEnabled) return;
		ImGui::Begin("cmd_engine");
		ImGui::Text("cmd_mode: %s", CmdMode == CMD_NATIVE ? "native" : CmdMode == CMD_LUA ? "lua" : "none");
		if (ImGui::InputText("command:", &chrCmdBuf[0], chrCmdBuf.size())) {
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
	// --==</GuiOfCmdEngine>==--
}
namespace NW
{
	// --==<GuiOfDataSys>==--
	GuiOfDataSys::GuiOfDataSys() { strcpy(strDir, DataSys::GetDirectory()); }
	GuiOfDataSys::~GuiOfDataSys() { }

	void GuiOfDataSys::OnDraw() {
		if (!bIsEnabled) return;
		ImGui::Begin("data_system", &bIsEnabled);
		if (ImGui::BeginPopupContextWindow("data_props", 1, false)) {
			if (ImGui::TreeNodeEx("create", GUI_DEFAULT_TREE_FLAGS)) {
				if (ImGui::MenuItem("shader")) { AShader::Create("shd_nameless"); }
				else if (ImGui::MenuItem("texture2d")) { ATexture2d::Create("tex_2d_nameless"); }
				else if (ImGui::MenuItem("gmaterial")) { MemSys::NewT<GMaterial>("gmt_3d_nameless"); }
				ImGui::TreePop();
			}
			ImGui::EndPopup();
		}
		
		if (ImGui::Button("file system tree")) { system("tree"); }
		else if (ImGui::Button("files list")) { system("dir"); }
		ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal | ImGuiSeparatorFlags_SpanAllColumns);

		if (ImGui::TreeNodeEx("data resources")) {
			DataSys::ADRs& rDrs = DataSys::GetADataResources();
			for (DataSys::ADRs::iterator itDrs = rDrs.begin(); itDrs != rDrs.end(); itDrs++) {
				ImGui::TextColored({0.4f, 0.7f, 0.7f, 1.0f}, "name: %s; id: %d", itDrs->second->GetName(), itDrs->first);
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("textures")) {
			auto& Textures = DataSys::GetDataResources<ATexture2d>();
			for (auto& itTex : Textures) {
				ImGui::Text(itTex.second->GetName());
				const ImageInfo& rImgInfo = itTex.second->GetImgInfo();
				if (ImGui::ImageButton(reinterpret_cast<void*>(itTex.second->GetRenderId()),
					ImVec2{ 64.0f * rImgInfo.nWidth / rImgInfo.nHeight, 64.0f })) { GuiOfSpriteEditor::Get().SetContext(itTex.second); }
			} ImGui::Separator();
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("shaders")) {
			auto& Shaders = DataSys::GetDataResources<AShader>();
			for (auto& pShader : Shaders) {
				if (ImGui::Button(pShader.second->GetName())) { GuiOfCodeEditor::Get().SetContext(pShader.second); }
			} ImGui::Separator();
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("gmaterials")) {
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
		ImGui::Begin("memory_system", &bIsEnabled);
		ImGui::Text("allocated_bytes: %d", MemSys::GetInfo().szAlloc);
		ImGui::Text("allocated_blocks: %d", MemSys::GetInfo().unAlloc);
		ImGui::End();
	}
	// --==</GuiOfMemSys>==--
	
	// --==<GuiOfTimeSys>==--
	void GuiOfTimeSys::OnDraw() {
		if (!bIsEnabled) return;
		ImGui::Begin("time system", &bIsEnabled);
		float nAppSpeed = TimeSys::GetAppSpeed();
		ImGui::Text("time since start: %d", static_cast<int>(TimeSys::GetRealTime()));
		ImGui::Text("application time: %d", static_cast<Int32>(TimeSys::GetAppTime()));
		if (ImGui::DragFloat("application speed", &nAppSpeed, 0.05f, 0.001f)) { TimeSys::SetAppSpeed(nAppSpeed); }
		ImGui::Text("updates/second: %d", static_cast<Int32>(1.0f / TimeSys::GetRealDelta()));
		ImGui::Text("date: %s", strDate);
		if (ImGui::Button("update date")) { strcpy(strDate, &TimeSys::GetTimeString()[0]); }
		ImGui::End();
	}
	// --==</GuiOfTimeSys>==--

	// --==<GuiOfLogSys>==--
	void GuiOfLogSys::OnDraw() {
		if (!bIsEnabled) return;
		ImGui::Begin("log_system", &bIsEnabled);
		if (ImGui::Button("print formatted message")) {
			LogSys::WriteStr("Int: {int};\nFloat: {flt};\nDouble: {flt};\nChar: {chr}\nCString: {str};",
				1, 1.25f, 152.0, 'C', "Format is done!");
		}
		ImGui::End();
	}
	// --==</GuiOfLogSys>==--
	
	// --==<GuiOfRandSys>==--
	void GuiOfRandSys::OnDraw() {
		if (!bIsEnabled) return;
		ImGui::Begin("random_system", &bIsEnabled);
		if (ImGui::Button("get random boolean")) {
			bRand = RandSys::GetBool();
		}
		else if (ImGui::Button("get random byte")) {
			btRand = RandSys::GetByte();
		}
		else if (ImGui::Button("get random ubyte")) {
			ubtRand = RandSys::GetUByte();
		}
		else if (ImGui::Button("get random integer")) {
			nRand = RandSys::GetInt();
		}
		else if (ImGui::Button("get random uinteger")) {
			unRand = RandSys::GetUInt();
		}
		else if (ImGui::Button("get random float")) {
			fRand = RandSys::GetFloat();
		}
		else if (ImGui::Button("get random double")) {
			dRand = RandSys::GetDouble();
		}
		ImGui::Text("Bool: %s; Byte: %d; UByte: %d;\nInt: %d; UInt: %d;\nFloat: %f; Double: %f;\n",
			bRand ? "true" : "false", btRand, ubtRand, nRand, unRand, fRand, dRand);
		ImGui::End();
	}
	// --==</GuiOfRandSys>==--
}

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
		ImGui::Begin("code_editor", &bIsEnabled, ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("file")) {
				if (ImGui::MenuItem("save...")) {
					String strFPath = DataSys::FDialog_save("all_files(*.*)\0*.*\0lua_scripts(*.lua)\0*.lua\0opengl_shader(*.glsl)\0(*.glsl)\0\0");
					if (!strFPath.empty()) {
						String strTemp;
						DataSys::SaveF_string(&strFPath[0], &strCodeBuf[0], strCodeBuf.size());
						strCodeBuf.resize(strTemp.size() + 8);
						strcpy(&strCodeBuf[0], &strTemp[0]);
					}
				}
				else if (ImGui::MenuItem("load...")) {
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
			else if (ImGui::BeginMenu("context")) {
				if (ImGui::BeginCombo("set shader", pContextShd == nullptr ? "no shader" : &pContextShd->GetName()[0])) {
					auto& Shaders = DataSys::GetDataResources<AShader>();
					for (auto& pShd : Shaders) {
						if (ImGui::Button(&pShd.second->GetName()[0])) {
							this->SetContext(pShd.second);
						}
					}
					ImGui::EndCombo();
				}
				if (ImGui::MenuItem("reset")) { SetContext(nullptr); }
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		if (pContextScr != nullptr) {
			ImGui::PushID(pContextScr->GetId());
			ImGui::Text("lua script: %s;", &pContextScr->GetName()[0]);
			if (ImGui::InputTextMultiline("", &strCodeBuf[0], strCodeBuf.size(),
				{ ImGui::GetContentRegionAvail().x - 32.0f, ImGui::GetContentRegionAvail().y - 64.0f }, ImGuiInputTextFlags_Multiline)) {
				if (strlen(&strCodeBuf[0]) > strCodeBuf.size() - 8) { strCodeBuf.resize(strCodeBuf.size() * 2); }
			}
			else if (ImGui::Button("edit")) { pContextScr->SetCode(&strCodeBuf[0]); }

			ImGui::PopID();
		}
		else if (pContextShd != nullptr) {
			ImGui::PushID(pContextShd->GetId());
			ImGui::Text("shader: %s;", &pContextShd->GetName()[0]);
			if (ImGui::InputTextMultiline("", &strCodeBuf[0], strCodeBuf.size(),
				{ ImGui::GetContentRegionAvail().x - 32.0f, ImGui::GetContentRegionAvail().y - 64.0f }, ImGuiInputTextFlags_Multiline)) {
				if (strlen(&strCodeBuf[0]) > strCodeBuf.size() - 8) { strCodeBuf.resize(strCodeBuf.size() * 2); }
			}
			else if (ImGui::Button("edit")) { pContextShd->SetCode(&strCodeBuf[0]); pContextShd->Compile(); }
			
			if (ImGui::TreeNodeEx("--==<vertex_buffer_layout>==--", GUI_DEFAULT_TREE_FLAGS)) {
				auto& rElems = pContextShd->GetVertexLayout().GetElems();
				for (UInt16 bei = 0; bei < rElems.size(); bei++) {
					auto& rBE = rElems[bei];
					ImGui::Text("%dth attribute:\nname: %s;\ttype: %s;\ncount = %d;\tis%snormalized", bei,
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
					ImGui::Text("%dth block\nname: %s\nsize = %d;\toffset = %d", rBlk.unBindPoint, rBlk.strName, rBlk.szAll, rBlk.szOffset);
					if (ImGui::TreeNodeEx("Elements", GUI_DEFAULT_TREE_FLAGS)) {
						for (UInt16 bei = 0; bei < rBlk.BufElems.size(); bei++) {
							auto& rBE = rBlk.BufElems[bei];
							ImGui::Text("%dth attribute:\nname: %s;\ttype: %s;\ncount = %d;\tis%snormalized", bei,
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
		ImGui::Begin("gmaterial_editor", &bIsEnabled, ImGuiWindowFlags_MenuBar);
		if (pContext == nullptr) { ImGui::End(); return; }

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("file")) {
				if (ImGui::MenuItem("save...")) {
					String strFPath = DataSys::FDialog_save("all_files(*.*)\0*.*\0graphics_material(*.gmt)\0(*.gmt)\0\0");
					if (!strFPath.empty()) { }
				}
				else if (ImGui::MenuItem("load...")) {
					String strFPath = DataSys::FDialog_load("all_files(*.*)\0*.*\0graphics_material(*.gmt)\0(*.gmt)\0\0");
					if (!strFPath.empty()) { }
				}
				ImGui::EndMenu();
			}
			else if (ImGui::BeginMenu("context")) {
				if (ImGui::BeginCombo("set gmaterial", pContext == nullptr ? "no material" : &pContext->GetName()[0])) {
					auto& GMaterials = DataSys::GetDataResources<GMaterial>();
					for (auto& itGMtl : GMaterials) {
						if (ImGui::Button(&itGMtl.second->GetName()[0])) {
							this->SetContext(itGMtl.second);
						}
					}
					ImGui::EndCombo();
				}
				if (ImGui::MenuItem("reset")) { SetContext(nullptr); }
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		strcpy(strContextName, pContext->GetName());
		if (ImGui::InputText("name", &strContextName[0], 128)) { pContext->SetName(strContextName); }

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
				ImGui::Text("%d texture: name: %s\nrender id = %d; slot = %d;",
					txi, &itTexMtl->first[0], pTex->GetRenderId(), pTex->GetTexSlot());
				ImGui::Text("width = %d; height = %d; depth = %d;",
					rImgInfo.nWidth, rImgInfo.nHeight, rImgInfo.nDepth);
				ImGui::Image(reinterpret_cast<void*>(pTex->GetRenderId()),
					ImVec2{ 64.0f * rImgInfo.nWidth / rImgInfo.nHeight, 64.0f });
				if (ImGui::BeginCombo("change texture", &(pTex->GetName())[0])) {
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
			ImGui::Text("id = %d;\nname: %s;", pShader->GetId(), pShader->GetName());
			if (ImGui::Button("code editor")) { GuiOfCodeEditor::Get().SetContext(pShader); }
			if (ImGui::BeginCombo("change shader", &pShader->GetName()[0])) {
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
		
		pShader->LoadF("D:/dev/native_world/nw_engine/src_glsl/shd_2d_display.glsl");
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
		ImGui::Begin("sprite_editor", &bIsEnabled, ImGuiWindowFlags_MenuBar);

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("file")) {
				if (ImGui::MenuItem("save...")) {
					String strFPath = DataSys::FDialog_save("all_files\0*.*\0images\0*.png");
					if (!strFPath.empty()) { pContext->SaveF(&strFPath[0]); }
				}
				else if (ImGui::MenuItem("load...")) {
					String strFPath = DataSys::FDialog_load("all_files\0*.*\0images\0*.png");
					if (!strFPath.empty()) { pContext->LoadF(&strFPath[0]); }
				}
				ImGui::EndMenu();
			}
			else if (ImGui::BeginMenu("context")) {
				if (ImGui::MenuItem("set texture")) {
					ImGui::OpenPopup("set context");
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
		if (ImGui::TreeNodeEx("sprites", GUI_DEFAULT_TREE_FLAGS)) {

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
			GraphEngine::Get().GetGApi()->DrawIndexed(pIndBuf->GetDataSize() / sizeof(UInt32));
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
		if (ImGui::BeginPopupContextItem("settings", ImGuiPopupFlags_MouseButtonRight)) {
			if (ImGui::MenuItem("remove")) {
				pACmp->GetEntity()->RemoveAComponent(pACmp->GetTypeId());
				pACmp = nullptr;
			}
			ImGui::EndPopup();
		}
		if (bOpened0) {
			if (pACmp != nullptr) {
				bool bEnabled0 = pACmp->IsEnabled();
				if (ImGui::Checkbox("is enabled", &bEnabled0)) { pACmp->SetEnabled(bEnabled0); }
				ImGui::Text("id: %d", pACmp->GetCmpId());
				ImGui::Separator();
				fnCallback();
			}
			ImGui::TreePop();
		}
		ImGui::PopID();
	}
	// -- Particular component GUI specializations
	template <> void GuiOfEntityEditor::OnDraw<LuaScriptCmp>(LuaScriptCmp* pLSCmp) {
		static auto cbLSCmp = [&]()->void {
			if (ImGui::Button("code editor")) {
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
			if (ImGui::DragFloat3("coordinate", &xyzCrd[0], 0.1f))	pTFCmp->SetCoord(xyzCrd);
			if (ImGui::DragFloat3("rotation", &xyzRtn[0], 0.1f))	pTFCmp->SetRotation(xyzRtn);
			if (ImGui::DragFloat3("scale", &xyzScl[0], 0.05f))		pTFCmp->SetScale(xyzScl);
		};
		OnDraw(pTFCmp, cbTFCmp);
	}
	template <> void GuiOfEntityEditor::OnDraw<AGraphicsCmp>(AGraphicsCmp* pGCmp) {
		static auto cbGCmp = [&]()->void {
			Int32 nDrawOrder = pGCmp->unDrawOrder;
			if (ImGui::InputInt("draw order", &nDrawOrder)) { pGCmp->unDrawOrder = nDrawOrder; }
			if (ImGui::BeginCombo("graphics state", &pGCmp->pGState->GetName()[0])) {
				auto& rGStates = GraphEngine::Get().GetStates();
				for (auto& itGState : rGStates) {
					if (ImGui::MenuItem(&itGState.first[0])) { pGCmp->pGState = itGState.second; }
				}
				ImGui::EndCombo();
			}
			ImGui::Separator();
			if (ImGui::TreeNodeEx("--==<gmaterial>==--", GUI_DEFAULT_TREE_FLAGS)) {
				GMaterial* pGMtl = pGCmp->GetDrawable()->pGMtl;
				Char cBuffer[128]{ 0 };
				strcpy_s(cBuffer, pGMtl->GetName());
				if (ImGui::InputText("name", &cBuffer[0], 128)) { pGMtl->SetName(cBuffer); }
				if (ImGui::BeginCombo("change gmaterial", &pGMtl->GetName()[0])) {
					auto& GMaterials = DataSys::GetDataResources<GMaterial>();
					for (auto& itGMtl : GMaterials) {
						if (ImGui::Button(&itGMtl.second->GetName()[0])) { pGCmp->GetDrawable()->pGMtl = itGMtl.second; }
					}
					ImGui::EndCombo();
				}
				if (ImGui::Button("gmaterial editor")) { GuiOfGMaterialEditor::Get().SetContext(pGMtl); }
				ImGui::TreePop();
			}
			if (TileMapCmp* ptmCmp = dynamic_cast<TileMapCmp*>(pGCmp)) {
				TileMapSprite* ptmSprite = static_cast<TileMapSprite*>(ptmCmp->GetDrawable());
				ATexture2d* pTileMap = static_cast<ATexture2d*>(ptmSprite->pGMtl->GetTexture());
				V2i whMapSize = { pTileMap->GetWidth(), pTileMap->GetHeight() };

				if (ImGui::DragFloat("tile size", &ptmSprite->whTileSize[0], 0.3f, 0,
					std::max(pTileMap->GetWidth(), pTileMap->GetHeight()))) { }

				else if (ImGui::DragFloat4("tile padding", &ptmSprite->xywhTilePadding[0], 1.0f, 0,
					std::max(whMapSize.x, whMapSize.y))) { }
				
				else if (ImGui::BeginCombo("change texture", pTileMap->GetName())) {
					auto& rTextures = DataSys::GetDataResources<ATexture2d>();
					for (auto& itTex : rTextures) {
						if (ImGui::MenuItem(itTex.second->GetName())) { ptmSprite->pGMtl->SetTexture(itTex.second); }
					}
					ImGui::EndCombo();
				}
				else if (ImGui::Button("update tiles")) {
					ptmSprite->UpdateTileData();
				}
				else if (ImGui::Button("add tile")) {
					SubTexture2d TileSubTex;
					TileSubTex.pOverTex = pTileMap;
					TileSubTex.whTexSize = { pTileMap->GetWidth(), pTileMap->GetHeight() };
					TileSubTex.whOverTexSize = { pTileMap->GetWidth(), pTileMap->GetHeight() };
					TileSubTex.xyTexCrd = { 0.0f, 0.0f };
					ptmSprite->Tiles.push_back(TileSprite{ V2i{0, 0}, TileSubTex });
					ptmSprite->Tiles.push_back(TileSprite{ V2i{1, 1}, TileSubTex });
					ptmSprite->UpdateTileData();
				}
			}
		};
		OnDraw(pGCmp, cbGCmp);
	}
	template <> void GuiOfEntityEditor::OnDraw<ACollider2dCmp>(ACollider2dCmp* pC2Cmp) {
		static auto cbC2Cmp = [&]()->void {
			bool bIsCollided = pC2Cmp->GetCollision();
			if (ImGui::Checkbox("is collided", &bIsCollided)) { pC2Cmp->SetCollision(bIsCollided); }
			ImGui::Separator();
			if (Rectangle* pRect = dynamic_cast<Rectangle*>(pC2Cmp->GetShape())) {
				ImGui::Text("-- rectangle");
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
		ImGui::Begin("entity_editor", &bIsEnabled, ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("file")) {
				if (ImGui::MenuItem("save...")) {
					String strFPath = DataSys::FDialog_save("all_files(*.*)\0*.*\0entity(*.ent)\0*.ent");
					if (!strFPath.empty()) {
					}
				}
				else if (ImGui::MenuItem("load...")) {
					String strFPath = DataSys::FDialog_load("all_files(*.*)\0*.*\0entity(*.ent)\0*.ent");
					if (!strFPath.empty()) {
					}
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		if (pContext == nullptr) { ImGui::End(); return; }

		bool bEnabled0 = pContext->IsEnabled();
		if (ImGui::Checkbox("is enabled", &bEnabled0)) { pContext->SetEnabled(bEnabled0); }
		char cNameBuf[128]{ 0 };
		strcpy_s(cNameBuf, &pContext->GetName()[0]);
		if (ImGui::InputText("name", &cNameBuf[0], 128)) { pContext->SetName(cNameBuf); }
		ImGui::Text("id: %d", pContext->GetId());
		ImGui::Separator();
		OnDraw<LuaScriptCmp>(pContext->GetComponent<LuaScriptCmp>());
		OnDraw<ATransformCmp>(pContext->GetComponent<ATransformCmp>());
		OnDraw<AGraphicsCmp>(pContext->GetComponent<AGraphicsCmp>());
		OnDraw<ACollider2dCmp>(pContext->GetComponent<ACollider2dCmp>());
		ImGui::Separator();
		if (ImGui::Button("add component")) {
			ImGui::OpenPopup("add component");
		}
		if (ImGui::BeginPopup("add component")) {
			if (ImGui::MenuItem("transform2d component")) {
				Transform2dCmp* pTF2dCmp = MemSys::NewT<Transform2dCmp>(*pContext);
			}
			else if (ImGui::MenuItem("transform3d component")) {
				Transform3dCmp* pTF3dCmp = MemSys::NewT<Transform3dCmp>(*pContext);
			}
			else if (ImGui::MenuItem("script component")) {
				LuaScriptCmp* pLSCmp = MemSys::NewT<LuaScriptCmp>(*pContext);
			}
			else if (ImGui::MenuItem("graphics2d component")) {
				Graphics2dCmp* pG2dCmp = MemSys::NewT<Graphics2dCmp>(*pContext);
			}
			else if (ImGui::MenuItem("tilemap component")) {
				TileMapCmp* ptmCmp = MemSys::NewT<TileMapCmp>(*pContext);
			}
			else if (ImGui::MenuItem("collider2d rectangle component")) {
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
		pIcoCamera->LoadF("data/tex/ico_eye.png");
	}
	// --core_methods
	void GuiOfSceneEditor::OnDraw() {
		if (!bIsEnabled) { return; }
		ImGui::Begin("scene_editor", &bIsEnabled);

		if (ImGui::BeginPopupContextWindow(0, 1, false)) {
			AEntity* pEnt0 = nullptr;
			if (ImGui::MenuItem("create entity")) {
				pEnt0 = Scene::Get().CreateEntity();
				Transform2dCmp* pTF2dCmp = MemSys::NewT<Transform2dCmp>(*pEnt0);
				Graphics2dCmp* pG2dCmp = MemSys::NewT<Graphics2dCmp>(*pEnt0);
			}
			else if (ImGui::MenuItem("create 10 entities")) {
				for (UInt8 ei = 0; ei < 10; ei++) {
					pEnt0 = Scene::Get().CreateEntity();
					Transform2dCmp* pTF2dCmp = MemSys::NewT<Transform2dCmp>(*pEnt0);
					Graphics2dCmp* pG2dCmp = MemSys::NewT<Graphics2dCmp>(*pEnt0);
				}
			}
			else if (ImGui::MenuItem("create 50 entities")) {
				for (UInt8 ei = 0; ei < 50; ei++) {
					pEnt0 = Scene::Get().CreateEntity();
					Transform2dCmp* pTF2dCmp = MemSys::NewT<Transform2dCmp>(*pEnt0);
					Graphics2dCmp* pG2dCmp = MemSys::NewT<Graphics2dCmp>(*pEnt0);
				}
			}
			else if (ImGui::MenuItem("destroy 50 entities")) {
				for (UInt8 ei = 0; ei < 50; ei++) { Scene::Get().DestroyEntity(&Scene::Get().GetEnts().begin()->second); }
			}
			ImGui::EndPopup();
		}
		OnDraw(Scene::Get().GetEnts());
		ImGui::End();

		OnDraw(GCameraLad::Get().GetGCamera());
		ImGui::Begin("scene_frame");
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

		if (pDestroyEnt != nullptr) {
			GuiOfEntityEditor::Get().SetContext(nullptr);
			Scene::Get().DestroyEntity(pDestroyEnt->GetId());
			pDestroyEnt = nullptr;
		}
	}

	inline void GuiOfSceneEditor::OnDraw(AEntity* pEnt)
	{

		bool bOpened = ImGui::TreeNodeEx(&pEnt->GetName()[0], GUI_DEFAULT_TREE_FLAGS);
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) { GuiOfEntityEditor::Get().SetContext(pEnt); }

		if (ImGui::BeginPopupContextItem(&pEnt->GetName()[0])) {
			if (ImGui::MenuItem("create subent")) {
				AEntity* pSubEnt = Scene::Get().CreateEntity();
				pEnt->AddSubEnt(pSubEnt);
				Transform2dCmp* pTF2dCmp = MemSys::NewT<Transform2dCmp>(*pSubEnt);
				Graphics2dCmp* pG2dCmp = MemSys::NewT<Graphics2dCmp>(*pSubEnt);
			}
			else if (ImGui::MenuItem("create 10 subents")) {
				for (UInt8 ei = 0; ei < 10; ei++) {
					AEntity* pSubEnt = Scene::Get().CreateEntity();
					pEnt->AddSubEnt(pSubEnt);
					Transform2dCmp* pTF2dCmp = MemSys::NewT<Transform2dCmp>(*pSubEnt);
					Graphics2dCmp* pG2dCmp = MemSys::NewT<Graphics2dCmp>(*pSubEnt);
				}
			}
			else if (ImGui::MenuItem("destroy entity")) { pDestroyEnt = pEnt; }
			ImGui::EndPopup();
		}
		if (bOpened) {
			if (pEnt->GetSubEnts().size() > 0) { OnDraw(pEnt->GetSubEnts()); }
			ImGui::TreePop();
		}
	}
	inline void GuiOfSceneEditor::OnDraw(Ents& rEnts) { for (auto& itEnt : rEnts) { OnDraw(&itEnt.second); } }
	inline void GuiOfSceneEditor::OnDraw(RefEnts& rRefEnts) { for (auto& itEnt : rRefEnts) { OnDraw(itEnt.second); } }
	inline void GuiOfSceneEditor::OnDraw(GCamera* pGCamera) {
		if (!bIsEnabledCamera || pGCamera == nullptr) return;
		ImGui::Begin("editor_camera", &bIsEnabledCamera);
		if (ImGui::BeginCombo("camera type", pGCamera->GetType() == GCamera::GCT_ORTHO ? "orthographic" : "perspective")) {
			if (ImGui::Selectable("orthographic", pGCamera->GetType() == GCamera::GCT_ORTHO)) {
				pGCamera->SetType(GCamera::GCT_ORTHO);
			}
			else if (ImGui::Selectable("perspective", pGCamera->GetType() == GCamera::GCT_PERSPECT)) {
				pGCamera->SetType(GCamera::GCT_PERSPECT);
			}
			ImGui::EndCombo();
		}
		if (ImGui::BeginCombo("camera mode", pGCamera->GetMode() == GCamera::GCM_2D ? "2 dimentional" : "3 dimentional")) {
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
			ImGui::DragFloat2("coordinates", &pGCamera->xCrd, 0.1f);
			ImGui::DragFloat("rotations", &pGCamera->zRtn, 0.1f);
			break;
		case GCamera::GCM_3D:
			ImGui::DragFloat3("coordinates", &pGCamera->xCrd, 0.1f);
			ImGui::DragFloat3("rotations", &pGCamera->xRtn, 0.1f);
			break;
		}
		ImGui::DragFloat2("near and far clips", &pGCamera->nNearClip, 0.1f);
		ImGui::Text("aspect ratio: %f", pGCamera->nAspectRatio);
		switch (pGCamera->GetType()) {
		case GCamera::GCT_PERSPECT:
			ImGui::DragFloat("field of view degree", &pGCamera->nViewField, 1.0f);
			break;
		case GCamera::GCT_ORTHO:
			ImGui::DragFloat("viewScale", &pGCamera->nViewScale, 0.1f);
			break;
		}
		ImGui::End();
	}
}
// --==</GuiOfSceneEditor>==--