#include <nw_pch.hpp>
#include "nw_gui_of.h"

#pragma warning(disable:4312)

// --==<GuiOfGlobal>==--
namespace NW
{
	// --==<GuiOfCoreEngine>==--
	GuiOfCoreEngine::GuiOfCoreEngine() {
	}
	GuiOfCoreEngine::~GuiOfCoreEngine() { }

	// --core_methods
	void GuiOfCoreEngine::OnDraw() {
		if (!bIsEnabled) return;
		ImGui::Begin("core_engine", &bIsEnabled);
		ImGui::End();
	}
	// --==</GuiOfCoreEngine>==--

	// --==<GuiOfGraphEnigne>==--
	GuiOfGfxEngine::GuiOfGfxEngine() { }
	GuiOfGfxEngine::~GuiOfGfxEngine() { }
	
	// --core_methods
	void GuiOfGfxEngine::OnDraw() {
		if (!bIsEnabled) return;
		ImGui::Begin("graphics_engine", &bIsEnabled);

		const GfxContextInfo& rGInfo = CoreEngine::Get().GetGfx()->GetInfo();
		ImGui::Text("\ncontext version: %s;\nrenderer: %s;"
			"\nvendor: %s;\nshading language: %s"
			"\nmax texture count: %d;\nmax vertex attributes: %d",
			rGInfo.strRenderer, rGInfo.strVersion,
			rGInfo.strVendor, rGInfo.strShdLang,
			rGInfo.nMaxTextures, rGInfo.nMaxVertexAttribs);
		ImGui::Separator();
		ImGui::End();

		ImGui::Begin("framebuffer");
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
namespace NW
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
				if (ImGui::MenuItem("save_as...", "ctrl+s")) {
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

		ImGui::End();
	}
	// --==</GuiOfDataSys>==--
}

// --==<GuiOfEditors>==--
namespace NW
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
	}
	// --==</GuiOfCodeEditor>==--

	// --==<GuiOfGfxMaterialEditor>==--
	GuiOfGfxMaterialEditor::GuiOfGfxMaterialEditor() :
		pContext() { }
	// --setters
	void GuiOfGfxMaterialEditor::SetContext(GfxMaterial& rContext) {
		this->pContext = &rContext;
		if (pContext == nullptr) {
			bIsEnabled = false;
		}
		else {
			if (pContext == nullptr) {}
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
			ImGui::EndMenuBar();
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
		}
		else {
			bIsEnabled = true;
		}
		strcpy(strContextName, pContext->GetName());
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
			ImGui::EndMenuBar();
		}
		if (pContext == nullptr) { ImGui::End(); return; }

		ImGui::Image(reinterpret_cast<void*>(pContext->GetEntId()),
			ImVec2{ static_cast<float>(64.0f * nAspectRatio), static_cast<float>(64.0f) });

		ImGui::End();
	}
	// --==</GuiOfSpriteEditor>==--
}