#include <nwg_pch.hpp>
#include "nwg_gui_of.h"

#pragma warning(disable : 4312)

// --==<GuiOfGlobal>==--
namespace NWG
{
	// --==<GuiOfCoreEngine>==--
	GuiOfCoreEngine::GuiOfCoreEngine() {
	}
	GuiOfCoreEngine::~GuiOfCoreEngine() { }

	// --core_methods
	void GuiOfCoreEngine::OnDraw() {
		if (!bIsEnabled) return;
		ImGui::Begin("core_engine", &bIsEnabled);
		
		{
			ImGui::Text("time since start: %d", static_cast<int>(TimeSys::GetCurrS()));
			ImGui::Text("updates/second: %d", static_cast<Int32>(1.0f / TimeSys::GetDeltaS()));
		}

		{
			ImGui::Text("global memory:\n\t%d blocks | %d bytes", MemInfo::GetGlobal().unAlloc, MemInfo::GetGlobal().szAlloc);
			ImGui::Text("mem_sys memory:\n\t%d blocks | %d bytes", MemSys::GetInfo().unAlloc, MemSys::GetInfo().szAlloc);
		}

		ImGui::End();
	}
	// --==</GuiOfCoreEngine>==--

	// --==<GuiOfGraphEnigne>==--
	GuiOfGraphEngine::GuiOfGraphEngine() { }
	GuiOfGraphEngine::~GuiOfGraphEngine() { }
	
	// --core_methods
	void GuiOfGraphEngine::OnDraw() {
		if (!bIsEnabled) return;
		ImGui::Begin("graphics_engine", &bIsEnabled);

		GfxApi* pGfx = CoreEngine::Get().GetGfx();

		const GfxInfo& rGInfo = pGfx->GetInfo();
		ImGui::Text("\ncontext version: %s;\nrenderer: %s;"
			"\nvendor: %s;\nshading language: %s"
			"\nmax texture count: %d;\nmax vertex attributes: %d",
			rGInfo.strRenderer, rGInfo.strVersion,
			rGInfo.strVendor, rGInfo.strShdLang,
			rGInfo.nMaxTextures, rGInfo.nMaxVertexAttribs);
		ImGui::Separator();
		ImGui::End();

		ImGui::Begin("framebuffer");

		FrameBuf* pfmBuf = pGfx->GetFrameBuf();
		FrameBufInfo fbInfo = pfmBuf->GetInfo();

		fbInfo.rectViewport = { 0, 0, ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 64.0f };

		if (pfmBuf->GetViewport() != fbInfo.rectViewport) { pfmBuf->SetViewport(fbInfo.rectViewport); pfmBuf->Remake(); }
		ImGui::Image(reinterpret_cast<ImTextureID>(pfmBuf->GetAttachment(0)->GetRenderId()),
			{static_cast<Float32>(fbInfo.GetWidth()), static_cast<Float32>(fbInfo.GetHeight()) }, { 0.0f, 1.0f }, { 1.0f, 0.0f });
		
		ImGui::Text("viewport: %d x %d x %d x %d", fbInfo.rectViewport.x, fbInfo.rectViewport.y, fbInfo.rectViewport.z, fbInfo.rectViewport.w);
		if (ImGui::BeginCombo("change framebuffer", &pfmBuf->GetName()[0])) {
			auto& rFbs = DataSys::GetStorage<FrameBuf>();
			for (auto& itFb : rFbs) {
				if (ImGui::MenuItem(itFb.second->GetName())) { pGfx->SetFrameBuf(itFb.second); }
			}
			ImGui::EndCombo();
		}

		ImGui::End();

	}
	// --==</GuiOfGraphEnigne>==--

	// --==<GuiOfCmdEngine>==--
	GuiOfCmdEngine::GuiOfCmdEngine() { }
	GuiOfCmdEngine::~GuiOfCmdEngine() { }
	// --core_methods
	void GuiOfCmdEngine::OnDraw() {
		if (!bIsEnabled) return;
		ImGui::Begin("nwc_engine");
		ImGui::End();
	}
	// --==</GuiOfCmdEngine>==--
}
namespace NWG
{
	// --==<GuiOfDataSys>==--
	GuiOfDataSys::GuiOfDataSys() { strcpy(strDir, DataSys::GetDirectory()); }
	GuiOfDataSys::~GuiOfDataSys() { }

	void GuiOfDataSys::OnDraw() {
		if (!bIsEnabled) return;
		ImGui::Begin("data_system", &bIsEnabled, ImGuiWindowFlags_MenuBar);

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("files")) {
				if (ImGui::MenuItem("new...", "ctrl+n")) {
				}
				if (ImGui::MenuItem("save_as...", "Ctrl+s")) {
				}
				if (ImGui::MenuItem("load...", "ctrl+l")) {
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		if (ImGui::Button("file system tree")) { system("tree"); }
		else if (ImGui::Button("files list")) { system("dir"); }
		ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal | ImGuiSeparatorFlags_SpanAllColumns);

		if (ImGui::TreeNodeEx("textures")) {
			auto& Textures = DataSys::GetStorage<Texture>();
			for (auto& itTex : Textures) {
				ImGui::Text(itTex.second->GetName());
				const ImageInfo& rImgInfo = itTex.second->GetImgInfo();
				if (ImGui::ImageButton(reinterpret_cast<void*>(itTex.second->GetRenderId()),
					ImVec2{ 64.0f * rImgInfo.nWidth / rImgInfo.nHeight, 64.0f })) {
					GuiOfSpriteEditor::Get().SetContext(*itTex.second);
				}
			}
			ImGui::Separator();
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("code_resoruces")) {
			auto& pCodes = DataSys::GetStorage<ACodeRes>();
			for (auto& pCode : pCodes) {
				if (ImGui::Button(pCode.second->GetName())) { }
			}
			ImGui::Separator();
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("gmaterials")) {
			auto& GfxMaterials = DataSys::GetStorage<GfxMaterial>();
			for (auto& itGMtl : GfxMaterials) {
				if (ImGui::Button(itGMtl.second->GetName())) { GuiOfGfxMaterialEditor::Get().SetContext(*itGMtl.second); }
			}
			ImGui::Separator();
			ImGui::TreePop();
		}
		ImGui::End();
	}
	// --==</GuiOfDataSys>==--
}

// --==<GuiOfEditors>==--
namespace NWG
{
	// --==<GuiOfCodeEditor>==--
	GuiOfCodeEditor::GuiOfCodeEditor() :
		strCodeBuf(DArray<char>(1024 * 4, 0)) { }

	// --setters
	void GuiOfCodeEditor::SetContext(RefKeeper<ACodeRes>& rContext) {
		pContext.SetRef(rContext);
		if (rContext.IsValid()) {
			bIsEnabled = true;
			strcpy(&strCodeBuf[0], &pContext->GetCode()[0]);
		}
		else {
			memset(&strCodeBuf[0], 0, strCodeBuf.size());
			bIsEnabled = false;
		}
	}
	// --core_methods
	void GuiOfCodeEditor::OnDraw()
	{
#if false
		if (!bIsEnabled) return;
		ImGui::Begin("code_editor", &bIsEnabled, ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("file")) {
				if (ImGui::MenuItem("save...")) {
					String strFPath = CoreEngine::Get().FDialogSave("all_files(*.*)\0*.*\0lua_scripts(*.lua)\0*.lua\0opengl_shader(*.glsl)\0(*.glsl)\0\0");
					if (!strFPath.empty()) {
						String strTemp;
						DataSys::SaveFString(&strFPath[0], &strCodeBuf[0], strCodeBuf.size());
						strCodeBuf.resize(strTemp.size() + 8);
						strcpy(&strCodeBuf[0], &strTemp[0]);
					}
				}
				else if (ImGui::MenuItem("load...")) {
					String strFPath = CoreEngine::Get().FDialogLoad("all_files(*.*)\0*.*\0lua_scripts(*.lua)\0*.lua\0opengl_shader(*.glsl)\0(*.glsl)\0\0");
					if (!strFPath.empty()) {
						String strTemp;
						DataSys::LoadFString(&strFPath[0], strTemp);
						strCodeBuf.resize(strTemp.size() + 8);
						strcpy(&strCodeBuf[0], &strTemp[0]);
					}
				}
				ImGui::EndMenu();
			}
			else if (ImGui::BeginMenu("context")) {
				if (ImGui::BeginCombo("set code", pContext == nullptr ? "no code" : &pContext->GetName()[0])) {
					auto& Shaders = DataSys::GetStorage<Shader>();
					for (auto& pShd : Shaders) {
						if (ImGui::Button(&pShd.second->GetName()[0])) { }
					}
					ImGui::EndCombo();
				}
				if (ImGui::MenuItem("reset")) { SetContext(RefKeeper<ACodeRes>()); }
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		if (pContext.IsValid()) {
			ImGui::PushID(pContext->GetId());
			ImGui::Text("shader: %s;", &pContext->GetName()[0]);
			if (ImGui::InputTextMultiline("", &strCodeBuf[0], strCodeBuf.size(),
				{ ImGui::GetContentRegionAvail().x - 32.0f, ImGui::GetContentRegionAvail().y - 64.0f }, ImGuiInputTextFlags_Multiline)) {
				if (strlen(&strCodeBuf[0]) > strCodeBuf.size() - 8) { strCodeBuf.resize(strCodeBuf.size() * 2); }
			}
			else if (ImGui::Button("edit")) { pContext->SetCode(&strCodeBuf[0]); pContext->Compile(); }

			if (ImGui::TreeNodeEx("--==<vertex_buffer_layout>==--", GUI_DEFAULT_TREE_FLAGS)) {
				auto& rElems = pContext->GetVtxLayout().GetElems();
				for (UInt16 bei = 0; bei < rElems.size(); bei++) {
					auto& rBE = rElems[bei];
					ImGui::Text("%dth attribute:\nname: %s;\ttype: %s;\ncount = %d;\tis%snormalized", bei,
						&rBE.strName[0], SdTypeGetStr(rBE.sdType),
						rBE.unCount, rBE.bNormalized ? " " : " not ");
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNodeEx("--==<globals>==--", GUI_DEFAULT_TREE_FLAGS)) {
				for (auto& rPrm : pContext->GetGlobals()) { ImGui::Text("%dth global: %s", rPrm.second, rPrm.first); }
				ImGui::TreePop();
			}
			if (ImGui::TreeNodeEx("--==<shader_buffer_layout>==--", GUI_DEFAULT_TREE_FLAGS)) {
				auto& rLayout = pContext->GetShdLayout();
				for (auto& rBlk : rLayout.GetBlocks()) {
					ImGui::Text("%dth block\nname: %s\nsize = %d;\toffset = %d", rBlk.unBindPoint, rBlk.strName, rBlk.szAll, rBlk.szOffset);
					if (ImGui::TreeNodeEx("Elements", GUI_DEFAULT_TREE_FLAGS)) {
						for (UInt16 bei = 0; bei < rBlk.BufElems.size(); bei++) {
							auto& rBE = rBlk.BufElems[bei];
							ImGui::Text("%dth attribute:\nname: %s;\ttype: %s;\ncount = %d;\tis%snormalized", bei,
								&rBE.strName[0], SdTypeGetStr(rBE.sdType), rBE.unCount, rBE.bNormalized ? " " : " not ");
						}
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
			ImGui::PopID();
		}
		ImGui::End();
#endif
	}
	// --==</GuiOfCodeEditor>==--

	// --==<GuiOfGfxMaterialEditor>==--
	GuiOfGfxMaterialEditor::GuiOfGfxMaterialEditor() :
		pContext(DataSys::GetDR<GfxMaterial>("gmt_batch_3d")) { }
	// --setters
	void GuiOfGfxMaterialEditor::SetContext(GfxMaterial& rContext) {
		this->pContext = &rContext;
		if (pContext == nullptr) {
			bIsEnabled = false;
		}
		else {
			if (pContext == nullptr) { pContext = DataSys::GetDR<GfxMaterial>("gmt_batch_3d"); }
			bIsEnabled = true;
		}
	}
	// --core_methods
	void GuiOfGfxMaterialEditor::OnDraw() {
		if (!bIsEnabled) return;
		ImGui::Begin("gmaterial_editor", &bIsEnabled, ImGuiWindowFlags_MenuBar);
		if (pContext == nullptr) { ImGui::End(); return; }

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("file")) {
				if (ImGui::MenuItem("save...")) {
					String strFPath = CoreEngine::Get().FDialogSave("all_files(*.*)\0*.*\0graphics_material(*.gmt)\0(*.gmt)\0\0");
					if (!strFPath.empty()) {}
				}
				else if (ImGui::MenuItem("load...")) {
					String strFPath = CoreEngine::Get().FDialogLoad("all_files(*.*)\0*.*\0graphics_material(*.gmt)\0(*.gmt)\0\0");
					if (!strFPath.empty()) {}
				}
				ImGui::EndMenu();
			}
			else if (ImGui::BeginMenu("context")) {
				if (ImGui::BeginCombo("set gmaterial", pContext == nullptr ? "no material" : &pContext->GetName()[0])) {
					auto& GfxMaterials = DataSys::GetStorage<GfxMaterial>();
					for (auto& itGMtl : GfxMaterials) {
						if (ImGui::Button(&itGMtl.second->GetName()[0])) {
							this->SetContext(*itGMtl.second);
						}
					}
					ImGui::EndCombo();
				}
				if (ImGui::MenuItem("reset")) { SetContext(*DataSys::GetStorage<GfxMaterial>().begin()->second); }
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
					auto& Textures = DataSys::GetStorage<Texture>();
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
			Shader* pShader = pContext->GetShader();
			ImGui::Text("id = %d;\nname: %s;", pShader->GetId(), pShader->GetName());
			//if (ImGui::Button("code editor")) { GuiOfCodeEditor::Get().SetContext(pShader); }
			if (ImGui::BeginCombo("change shader", &pShader->GetName()[0])) {
				auto& Shaders = DataSys::GetStorage<Shader>();
				for (auto& pShd : Shaders) {
					if (ImGui::Button(&pShd.second->GetName()[0])) { pContext->SetShader(pShd.second); }
				}
				ImGui::EndCombo();
			}
			ImGui::TreePop();
		}

		ImGui::End();
	}
	// --==</GuiOfGfxMaterialEditor>==--

	// --==<GuiOfSpriteEditor>==--
	GuiOfSpriteEditor::GuiOfSpriteEditor() { }
	// --setters
	void GuiOfSpriteEditor::SetContext(Texture& rContext) {
		this->pContext = &rContext;
		if (pContext != nullptr) {
			bIsEnabled = false;
			pContext = DataSys::GetDR<Texture>("tex_white_solid");
		}
		else {
			bIsEnabled = true;
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
					String strFPath = CoreEngine::Get().FDialogSave("all_files\0*.*\0images\0*.png");
					if (!strFPath.empty()) { pContext->SaveF(&strFPath[0]); }
				}
				else if (ImGui::MenuItem("load...")) {
					String strFPath = CoreEngine::Get().FDialogLoad("all_files\0*.*\0images\0*.png");
					if (!strFPath.empty()) { pContext->LoadF(&strFPath[0]); }
				}
				ImGui::EndMenu();
			}
			else if (ImGui::BeginMenu("context")) {
				if (ImGui::MenuItem("set texture")) {
					ImGui::OpenPopup("set context");
					if (ImGui::BeginPopup("set_context")) {
						auto& Textures = DataSys::GetStorage<Texture>();
						for (auto& itTex : Textures) {
							ImGui::Text(itTex.second->GetName());
							const ImageInfo& rImgInfo = itTex.second->GetImgInfo();
							if (ImGui::ImageButton(reinterpret_cast<void*>(itTex.second->GetRenderId()),
								ImVec2{ 64.0f * rImgInfo.nWidth / rImgInfo.nHeight, 64.0f })) {
								SetContext(*itTex.second);
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
		ImGui::Text("current: width = %d; height = %d", pContext->GetImgInfo().nWidth, pContext->GetImgInfo().nHeight);

		if (ImGui::Button("apply")) { pContext->SetInfo(ImgInfo); pContext->SetInfo(TexInfo); pContext->Remake(); }

		if (ImGui::TreeNodeEx("texture_info", GUI_DEFAULT_TREE_FLAGS)) {

			if (ImGui::BeginCombo("filter_min", TexInfo.FilterMin == TXF_LINEAR ? "linear" :
				TexInfo.FilterMin == TXF_NEAREST ? "nearest" : "none")) {
				if (ImGui::MenuItem("linear")) { TexInfo.FilterMin = TXF_LINEAR; }
				else if (ImGui::MenuItem("nearest")) { TexInfo.FilterMin = TXF_NEAREST; }
				ImGui::EndCombo();
			}
			else if (ImGui::BeginCombo("filter_mag", TexInfo.FilterMag == TXF_LINEAR ? "linear" :
				TexInfo.FilterMag == TXF_LINEAR ? "nearest" : "none")) {
				if (ImGui::MenuItem("linear")) { TexInfo.FilterMag = TXF_LINEAR; }
				else if (ImGui::MenuItem("nearest")) { TexInfo.FilterMag = TXF_NEAREST; }
				ImGui::EndCombo();
			}

			else if (ImGui::BeginCombo("wrap_s", TexInfo.WrapTypeS == TXW_REPEAT ? "repeat" : TexInfo.WrapTypeS == TXW_CLAMP ? "clamp" : "none")) {
				if (ImGui::MenuItem("clamp")) { TexInfo.WrapTypeS = TXW_CLAMP; }
				else if (ImGui::MenuItem("repeat")) { TexInfo.WrapTypeS = TXW_REPEAT; }
				ImGui::EndCombo();
			}
			else if (ImGui::BeginCombo("wrap_t", TexInfo.WrapTypeT == TXW_REPEAT ? "repeat" : TexInfo.WrapTypeT == TXW_CLAMP ? "clamp" : "none")) {
				if (ImGui::MenuItem("clamp")) { TexInfo.WrapTypeT = TXW_CLAMP; }
				else if (ImGui::MenuItem("repeat")) { TexInfo.WrapTypeT = TXW_REPEAT; }
				ImGui::EndCombo();
			}

			else if (ImGui::BeginCombo("format", TexInfo.texFormat == TXF_RGB ? "rgb" : TexInfo.texFormat == TXF_RGBA ? "rgba" :
				TexInfo.texFormat == TXF_RED ? "grayscale" : "none")) {
				if (ImGui::MenuItem("rgb")) { TexInfo.texFormat = TXF_RGB; TexInfo.texInterFormat = TXFI_RGB; }
				else if (ImGui::MenuItem("rgba")) { TexInfo.texFormat = TXF_RGBA; TexInfo.texInterFormat = TXFI_RGBA8; }
				ImGui::EndCombo();
			}

			ImGui::TreePop();
		}
		ImGui::Image(reinterpret_cast<void*>(pContext->GetRenderId()),
			ImVec2{ static_cast<float>(64.0f * nAspectRatio), static_cast<float>(64.0f) });

		ImGui::End();
	}
	// --==</GuiOfSpriteEditor>==--
#if false
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
		ImGui::PushID(pACmp->GetId());
		bool bOpened0 = ImGui::TreeNodeEx(pACmp->GetName(), GUI_DEFAULT_TREE_FLAGS);
		if (ImGui::BeginPopupContextItem("settings", ImGuiPopupFlags_MouseButtonRight)) {
			if (ImGui::MenuItem("remove")) {
				pACmp->GetEntity()->RmvACmp(pACmp->GetTypeInfo());
				pACmp = nullptr;
			}
			ImGui::EndPopup();
		}
		if (bOpened0) {
			if (pACmp != nullptr) {
				bool bEnabled0 = pACmp->IsEnabled();
				if (ImGui::Checkbox("is enabled", &bEnabled0)) { pACmp->SetEnabled(bEnabled0); }
				ImGui::Text("id: %d", pACmp->GetId());
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
	template <> void GuiOfEntityEditor::OnDraw<ATFormCmp>(ATFormCmp* pTFCmp) {
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
			Int32 nDrawOrder = pGCmp->DOData.unDrawOrder;
			if (ImGui::InputInt("draw order", &nDrawOrder)) { pGCmp->DOData.unDrawOrder = nDrawOrder; }
			if (ImGui::BeginCombo("graphics layer", &pGCmp->pGLayer->strName[0])) {
				for (auto& itGState : GraphEnigne::Get().GetLayers()) {
					if (ImGui::MenuItem(&itGState.strName[0])) { pGCmp->pGLayer = &itGState; }
				}
				ImGui::EndCombo();
			}
			ImGui::Separator();
			if (ImGui::TreeNodeEx("--==<gmaterial>==--", GUI_DEFAULT_TREE_FLAGS)) {
				GfxMaterial* pGMtl = pGCmp->GetDrawable()->pGMtl;
				Char cBuffer[128]{ 0 };
				strcpy_s(cBuffer, pGMtl->GetName());
				if (ImGui::InputText("name", &cBuffer[0], 128)) { pGMtl->SetName(cBuffer); }
				if (ImGui::BeginCombo("change gmaterial", &pGMtl->GetName()[0])) {
					auto& GfxMaterials = ADataRes::GetStorage<GfxMaterial>();
					for (auto& itGMtl : GfxMaterials) {
						if (ImGui::Button(&itGMtl.second->GetName()[0])) { pGCmp->GetDrawable()->pGMtl = itGMtl.second; }
					}
					ImGui::EndCombo();
				}
				if (ImGui::Button("gmaterial editor")) { GuiOfGfxMaterialEditor::Get().SetContext(pGMtl); }
				ImGui::TreePop();
			}
			if (TileMapCmp* ptmCmp = dynamic_cast<TileMapCmp*>(pGCmp)) {
				TileMapSprite* ptmSprite = static_cast<TileMapSprite*>(ptmCmp->GetDrawable());
				ATexture2d* pTileMap = static_cast<ATexture2d*>(ptmSprite->pGMtl->GetTexture());
				V2i whMapSize = { pTileMap->GetWidth(), pTileMap->GetHeight() };

				if (ImGui::DragFloat("tile size", &ptmSprite->whTileSize[0], 0.3f, 0,
					std::max(pTileMap->GetWidth(), pTileMap->GetHeight()))) {
				}

				else if (ImGui::DragFloat4("tile padding", &ptmSprite->xywhTilePadding[0], 1.0f, 0,
					std::max(whMapSize.x, whMapSize.y))) {
				}

				else if (ImGui::BeginCombo("change texture", pTileMap->GetName())) {
					auto& rTextures = ADataRes::GetStorage<ATexture2d>();
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
					//ptmSprite->Tiles.push_back(TileSprite{ V2i{0, 0}, TileSubTex });
					//ptmSprite->Tiles.push_back(TileSprite{ V2i{1, 1}, TileSubTex });
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
			if (NW::Rectangle* pRect = dynamic_cast<NW::Rectangle*>(pC2Cmp->GetShape())) {
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
		OnDraw<LuaScriptCmp>(pContext->GetCmp<LuaScriptCmp>());
		OnDraw<ATFormCmp>(pContext->GetCmp<ATFormCmp>());
		OnDraw<AGraphicsCmp>(pContext->GetCmp<AGraphicsCmp>());
		OnDraw<ACollider2dCmp>(pContext->GetCmp<ACollider2dCmp>());
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
				Gfx2dCmp* pG2dCmp = MemSys::NewT<Gfx2dCmp>(*pContext);
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
namespace NWG
{
	GuiOfSceneEditor::GuiOfSceneEditor()
	{
	}
	// --core_methods
	void GuiOfSceneEditor::OnDraw() {
		if (!bIsEnabled) { return; }
		ImGui::Begin("scene_editor", &bIsEnabled);

		if (ImGui::BeginPopupContextWindow(0, 1, false)) {
			AEntity* pEnt0 = nullptr;
			if (ImGui::MenuItem("create entity")) {
				pEnt0 = MemSys::NewT<AEntity>();
				pEnt0->AddCmp<Transform2dCmp>();
				pEnt0->AddCmp<Gfx2dCmp>();
			}
			else if (ImGui::MenuItem("create 10 entities")) {
				for (UInt8 ei = 0; ei < 10; ei++) {
					pEnt0 = MemSys::NewT<AEntity>();
					pEnt0->AddCmp<Transform2dCmp>();
					pEnt0->AddCmp<Gfx2dCmp>();
				}
			}
			else if (ImGui::MenuItem("create 50 entities")) {
				for (UInt8 ei = 0; ei < 50; ei++) {
					pEnt0 = MemSys::NewT<AEntity>();
					pEnt0->AddCmp<Transform2dCmp>();
					pEnt0->AddCmp<Gfx2dCmp>();
				}
			}
			else if (ImGui::MenuItem("destroy 50 entities")) {
				auto& rEnts = ADataRes::GetStorage<AEntity>();
				for (UInt8 ei = 0; ei < 50; ei++) {
					if (rEnts.empty()) { return; };
					MemSys::DelT<AEntity>(rEnts.begin()->second);
					rEnts.erase(rEnts.begin());
				}
			}
			ImGui::EndPopup();
		}
		OnDraw(ADataRes::GetStorage<AEntity>());
		ImGui::End();

		OnDraw(GfxCameraLad::Get().GetGfxCamera());
		ImGui::Begin("scene_frame");
		if (ImGui::ImageButton(reinterpret_cast<void*>(pIcoCamera->GetRenderId()),
			{ 32.0f * (static_cast<float>(pIcoCamera->GetWidth()) / static_cast<float>(pIcoCamera->GetHeight())), 32.0f })) {
			bIsEnabledCamera = true;
		}
		ImGui::Separator();

		UInt32 unSizeW = ImGui::GetContentRegionAvail().x;
		UInt32 unSizeH = ImGui::GetContentRegionAvail().y - 48.0f;
		const AFrameBuf* pFrameBuf = GraphEnigne::Get().GetLayer()->pFrameBuf;
		const FrameBufInfo& rfbInfo = pFrameBuf->GetInfo();
		if (rfbInfo.unWidth != unSizeW || rfbInfo.unHeight != unSizeH) {
			auto& rLayers = GraphEnigne::Get().GetLayers();
			for (auto& itLayer : rLayers) {
				itLayer.SetViewport({ 0.0f, 0.0f, unSizeW, unSizeH });
			}
		}
		ImGui::Image(reinterpret_cast<void*>(pFrameBuf->GetColorAttachment()->GetRenderId()),
			{ static_cast<float>(unSizeW), static_cast<float>(unSizeH) }, { 0.0f, 0.0f }, { 1.0f, -1.0f });
		ImGui::Separator();
		ImGui::End();

		if (pDestroyEnt != nullptr) {
			GuiOfEntityEditor::Get().SetContext(nullptr);
			MemSys::DelT<AEntity>(ADataRes::GetDataRes<AEntity>(pDestroyEnt->GetId()));
			pDestroyEnt = nullptr;
		}
	}

	inline void GuiOfSceneEditor::OnDraw(AEntity* pEnt)
	{
		bool bOpened = ImGui::TreeNodeEx(&pEnt->GetName()[0], GUI_DEFAULT_TREE_FLAGS);
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) { GuiOfEntityEditor::Get().SetContext(pEnt); }

		if (ImGui::BeginPopupContextItem(&pEnt->GetName()[0])) {
			if (ImGui::MenuItem("create subent")) {
				AEntity* pSubEnt = MemSys::NewT<AEntity>();
				pEnt->AddSubEnt(pSubEnt);
				pSubEnt->AddCmp<Transform2dCmp>();
				pSubEnt->AddCmp<Gfx2dCmp>();
			}
			else if (ImGui::MenuItem("create 10 subents")) {
				for (UInt8 ei = 0; ei < 10; ei++) {
					AEntity* pSubEnt = MemSys::NewT<AEntity>();
					pEnt->AddSubEnt(pSubEnt);
					pSubEnt->AddCmp<Transform2dCmp>();
					pSubEnt->AddCmp<Gfx2dCmp>();
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
#endif
}
// --==</GuiOfSceneEditor>==--