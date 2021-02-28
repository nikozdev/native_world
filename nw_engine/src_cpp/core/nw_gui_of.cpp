#include <nw_pch.hpp>
#include "nw_gui_of.h"

#pragma warning(disable:4312)

// --==<GuiOfGlobal>==--
namespace NW
{
	// --==<GuiOfCoreEngine>==--
	GuiOfCoreEngine::GuiOfCoreEngine() { }
	GuiOfCoreEngine::~GuiOfCoreEngine() { }

	// --core_methods
	void GuiOfCoreEngine::OnDraw() {
		if (!bIsEnabled) return;
		GUI::Begin("core_engine", &bIsEnabled);
		GUI::End();
	}
	// --==</GuiOfCoreEngine>==--

	// --==<GuiOfGraphEnigne>==--
	GuiOfGfxEngine::GuiOfGfxEngine() { }
	GuiOfGfxEngine::~GuiOfGfxEngine() { }
	
	// --core_methods
	void GuiOfGfxEngine::OnDraw() {
		if (!bIsEnabled) return;
		GUI::Begin("graphics_engine", &bIsEnabled);
		GUI::End();
	}
	// --==</GuiOfGraphEnigne>==--

	// --==<GuiOfCmdEngine>==--
	GuiOfCmdEngine::GuiOfCmdEngine() { }
	GuiOfCmdEngine::~GuiOfCmdEngine() { }
	// --core_methods
	void GuiOfCmdEngine::OnDraw() {
		if (!bIsEnabled) return;
		GUI::Begin("console_engine");
		GUI::End();
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
		GUI::Begin("data_system", &bIsEnabled, ImGuiWindowFlags_MenuBar);

		if (GUI::BeginMenuBar()) {
			if (GUI::BeginMenu("files")) {
				if (GUI::MenuItem("new...", "ctrl+n")) {
				}
				if (GUI::MenuItem("save_as...", "ctrl+s")) {
				}
				if (GUI::MenuItem("load...", "ctrl+l")) {
				}
				GUI::EndMenu();
			}
			GUI::EndMenuBar();
		}

		if (GUI::Button("file system tree")) { system("tree"); }
		else if (GUI::Button("files list")) { system("dir"); }
		GUI::SeparatorEx(ImGuiSeparatorFlags_Horizontal | ImGuiSeparatorFlags_SpanAllColumns);

		GUI::End();
	}
	// --==</GuiOfDataSys>==--
}

// --==<GuiOfEditors>==--
namespace NW
{
	// --==<GuiOfShaderEditor>==--
	GuiOfShaderEditor::GuiOfShaderEditor() { }

	// --setters
	void GuiOfShaderEditor::SetContext(RefKeeper<ShaderProg>& rContext) {
		pContext.SetRef(rContext);
		if (rContext.IsValid()) {
			bIsEnabled = true;
		}
		else {
			bIsEnabled = false;
		}
	}
	// --core_methods
	void GuiOfShaderEditor::OnDraw()
	{
		GUI::Begin("shader_editor", &bIsEnabled, ImGuiWindowFlags_MenuBar);
		if (pContext == nullptr) { GUI::End(); return; }
		GUI::End();
	}
	// --==</GuiOfShaderEditor>==--

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
		GUI::Begin("graphics_material_editor", &bIsEnabled, ImGuiWindowFlags_MenuBar);
		if (pContext == nullptr) { GUI::End(); return; }

		if (GUI::BeginMenuBar()) {
			if (GUI::BeginMenu("file")) {
				if (GUI::MenuItem("save...")) {
				}
				else if (GUI::MenuItem("load...")) {
				}
				GUI::EndMenu();
			}
			GUI::EndMenuBar();
		}

		GUI::End();
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
		GUI::Begin("sprite_editor", &bIsEnabled, ImGuiWindowFlags_MenuBar);

		if (GUI::BeginMenuBar()) {
			if (GUI::BeginMenu("file")) {
				if (GUI::MenuItem("save...")) {
				}
				else if (GUI::MenuItem("load...")) {
				}
				GUI::EndMenu();
			}
			GUI::EndMenuBar();
		}
		if (pContext == nullptr) { GUI::End(); return; }

		GUI::Image(reinterpret_cast<void*>(pContext->GetRenderId()),
			ImVec2{ static_cast<float>(64.0f * nAspectRatio), static_cast<float>(64.0f) });

		GUI::End();
	}
	// --==</GuiOfSpriteEditor>==--
}