#ifndef NWG_GUI_OF_H
#define NWG_GUI_OF_H

#include <nwg_pch.hpp>

using namespace NW;
using namespace GLIB;

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
		Shader* pContextShd = nullptr;
		DArray<char> strCodeBuf;
	private:
		GuiOfCodeEditor();
	public:
		// --setters
		void SetContext(ACodeRes* pContext);

		// --core_methods
		virtual void OnDraw() override;
	};
	/// GuiOfSpriteEditor struct
	struct NW_API GuiOfGMaterialEditor : public AGuiOf<GuiOfGMaterialEditor>
	{
		friend class AGuiOf<GuiOfGMaterialEditor>;
	private:
		GuiOfGMaterialEditor();
	public:
		// --getters
		// --setters
		void SetContext(GMaterial* pContext);

		// --core_methods
		virtual void OnDraw() override;
	private:
		GMaterial* pContext = nullptr;
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
		void SetContext(Texture* pContext);

		// --core_methods
		virtual void OnDraw() override;
	private:
		Texture* pContext = nullptr;
		Char strContextName[128];

		float nAspectRatio = 1.0f;

		ImageInfo ImgInfo;
		TextureInfo TexInfo;
	};
}
#endif	// NWG_GUI_OF_H