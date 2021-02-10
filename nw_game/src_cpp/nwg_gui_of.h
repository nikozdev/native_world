#ifndef NWG_GUI_OF_H
#define NWG_GUI_OF_H

#include <nwg_pch.hpp>

using namespace NW;

namespace NWG
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

namespace NWG
{
	/// GuiOfEngine struct
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
	struct NW_API GuiOfGraphEngine : public AGuiOf<GuiOfGraphEngine>
	{
		friend class AGuiOf<GuiOfGraphEngine>;
	private:
		GuiOfGraphEngine();
		~GuiOfGraphEngine();
	public:
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
namespace NWG
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
		// --data_system
		Char strDir[256]{ 0 };
		Char strCurrDir[256]{ 0 };
		// --time system
	};
}
namespace NWG
{
	/// GuiOfCodeEditor struct
	struct NW_API GuiOfCodeEditor : public AGuiOf<GuiOfCodeEditor>
	{
		friend class AGuiOf<GuiOfCodeEditor>;
	private:
		RefKeeper<ACodeRes> pContext;
		DArray<char> strCodeBuf;
	private:
		GuiOfCodeEditor();
	public:
		// --setters
		void SetContext(RefKeeper<ACodeRes>& rContext);

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
		// --getters
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
		// --getters
		// --setters
		void SetContext(Texture& rContext);

		// --core_methods
		virtual void OnDraw() override;
	private:
		Texture* pContext;
		Char strContextName[128];

		float nAspectRatio = 1.0f;

		ImageInfo ImgInfo;
		TextureInfo TexInfo;
	};
}
#endif	// NWG_GUI_OF_H