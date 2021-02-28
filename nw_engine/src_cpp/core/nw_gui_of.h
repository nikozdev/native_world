#ifndef NW_GUI_OF_H
#define NW_GUI_OF_H

#include <nw_core.hpp>

#include <core/nw_engine.h>
#include <lua/lua_engine.h>

#if (defined NWG_GAPI)
#	include <gui/imgui_core.hpp>
#	include <gui/imgui_internal.h>
#	include <gui/impl/imgui_win.h>
#	if (NWG_GAPI & NWG_GAPI_OGL)
#		include <gui/impl/imgui_ogl.h>
#	endif
#	if (NWG_GAPI & NWG_GAPI_DX)
#		include <gui/impl/imgui_dx.h>
#	endif
#	define GUI_DEFAULT_TREE_FLAGS ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick
#endif	// NWG_GAPI

namespace NW
{
	/// Abstract GuiOf struct
	template <typename GuiOfType> struct NW_API AGuiOf
	{
	public:
		bool bIsEnabled = false;
	protected:
		AGuiOf() = default;
	public:
		AGuiOf(AGuiOf& rCpy) = delete;
		void operator=(AGuiOf& rCpy) = delete;
		virtual ~AGuiOf() = default;
		// --getters
		static inline GuiOfType& Get() { static GuiOfType s_GuiOf; return s_GuiOf; }
		// --core_methods
		virtual void OnDraw() = 0;
	};
}

namespace NW
{
	/// GuiOfCoreEngine struct
	/// Description:
	/// -- Renders the core engine state and window gui
	struct NW_API GuiOfCoreEngine : public AGuiOf<GuiOfCoreEngine>
	{
		friend class AGuiOf<GuiOfCoreEngine>;
	private:
		GuiOfCoreEngine();
		~GuiOfCoreEngine();
	public:
		virtual void OnDraw() override;
	};
	/// GuiOfGraphEngine struct
	struct NW_API GuiOfGfxEngine : public AGuiOf<GuiOfGfxEngine>
	{
		friend class AGuiOf<GuiOfGfxEngine>;
	private:
		GuiOfGfxEngine();
	public:
		~GuiOfGfxEngine();
		virtual void OnDraw() override;
	private:
	};
	/// GuiOfCmdEngine struct
	struct NW_API GuiOfCmdEngine : public AGuiOf<GuiOfCmdEngine>
	{
		friend class AGuiOf<GuiOfCmdEngine>;
	private:
		GuiOfCmdEngine();
		~GuiOfCmdEngine();
	public:
		virtual void OnDraw() override;
	};
}
namespace NW
{
	/// GuiOfDataSys struct
	struct NW_API GuiOfDataSys : public AGuiOf<GuiOfDataSys>
	{
		friend class AGuiOf<GuiOfDataSys>;
	private:
		GuiOfDataSys();
		~GuiOfDataSys();
	public:
		virtual void OnDraw() override;
	private:
		Char strDir[256]{ 0 };
		Char strCurrDir[256]{ 0 };
	};
}
namespace NW
{
	/// GuiOfShaderEditor struct
	struct NW_API GuiOfShaderEditor : public AGuiOf<GuiOfShaderEditor>
	{
		friend class AGuiOf<GuiOfShaderEditor>;
	private:
		RefKeeper<ShaderProg> pContext;
	private:
		GuiOfShaderEditor();
	public:
		// --setters
		void SetContext(RefKeeper<ShaderProg>& rContext);
		// --core_methods
		virtual void OnDraw() override;
	};
	/// GuiOfSpriteEditor struct
	struct NW_API GuiOfGfxMaterialEditor : public AGuiOf<GuiOfGfxMaterialEditor>
	{
		friend class AGuiOf<GuiOfGfxMaterialEditor>;
	private:
		GuiOfGfxMaterialEditor();
	public:
		// --setters
		void SetContext(GfxMaterial& rContext);

		// --core_methods
		virtual void OnDraw() override;
	private:
		GfxMaterial* pContext;
		Char strContextName[128];
	};
	/// GuiOfSpriteEditor struct
	struct NW_API GuiOfSpriteEditor : public AGuiOf<GuiOfSpriteEditor>
	{
		friend class AGuiOf<GuiOfSpriteEditor>;
	private:
		GuiOfSpriteEditor();
	public:
		// --setters
		void SetContext(Texture& rContext);

		// --core_methods
		virtual void OnDraw() override;
	private:
		Texture* pContext;
		Char strContextName[128];

		Float32 nAspectRatio = 1.0f;

		ImageInfo ImgInfo;
	};
}
#endif	// NW_GUI_OF_H