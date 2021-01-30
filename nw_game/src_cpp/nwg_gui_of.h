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
	public:
		bool bAppState = false;
	private:
		GuiOfCoreEngine();
		~GuiOfCoreEngine();
	public:
		virtual void OnDraw() override;
	private:
		// --core_engine
		CoreState* pCoreState = nullptr;
	};
	/// GuiOfGEngine struct
	struct NW_API GuiOfGEngine : public AGuiOf<GuiOfGEngine>
	{
		friend class AGuiOf<GuiOfGEngine>;
	private:
		GuiOfGEngine();
		~GuiOfGEngine();
	private:
		bool bWindow = false, bLayers = false, bFrameBufs = false;
		AWindow* pWindow = nullptr;
		AFrameBuf* pFrameBuf = nullptr;
		float nLineW = 0.01f, nPixelSz = 0.01f, nWndOpacity = 1.0f;
		Char strLayerName[256] = "del_default";
		Char strWindowTitle[256] = "nw_engine";
		DrawObjectData DOData;
	public:
		virtual void OnDraw() override;
	};
	/// GuiOfCmdEngine struct
	struct NW_API GuiOfCmdEngine : public AGuiOf<GuiOfCmdEngine>
	{
		friend class AGuiOf<GuiOfCmdEngine>;
	private:
		GuiOfCmdEngine();
		~GuiOfCmdEngine();
	private:
		enum CmdModes : UInt8 {
			CMD_NONE = 0,
			CMD_NATIVE = 1,
			CMD_LUA = 2,
		};
	private:
		CmdModes CmdMode = CMD_NONE;
		DArray<String> strCmdBuf;
		DArray<Char> chrCmdBuf;
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
	struct NW_API GuiOfMemSys : public AGuiOf<GuiOfMemSys>
	{
	public:
		virtual void OnDraw() override;
	private:
		Size szOffset = 0;
	};
}
namespace NWG
{
	/// GuiOfCodeEditor struct
	struct NW_API GuiOfCodeEditor : public AGuiOf<GuiOfCodeEditor>
	{
		friend class AGuiOf<GuiOfCodeEditor>;
	private:
		AShader* pContextShd = nullptr;
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
		void SetContext(ATexture2d* pContext);

		// --core_methods
		virtual void OnDraw() override;
	private:
		ATexture2d* pContext = nullptr;
		Char strContextName[128];

		float nAspectRatio = 1.0f;

		ImageInfo ImgInfo;
		TextureInfo TexInfo;
	};
#if false
	/// GuiOfEntityEditor struct
	struct NW_API GuiOfEntityEditor : public AGuiOf<GuiOfEntityEditor>
	{
		using CmpCallback = std::function<void(AEntityCmp*)>;
	private:
		template <typename CBType> inline void OnDraw(AEntityCmp* pACmp, CBType& fnCallback);
		template <class CmpType> inline void OnDraw(CmpType* pCmp);
	public:
		// --setters
		void SetContext(AEntity* pContext);
		// --core_methods
		virtual void OnDraw() override;
	private:
		AEntity* pContext = nullptr;
	};
	/// GuiOfSceneEditor struct
	struct NW_API GuiOfSceneEditor : public AGuiOf<GuiOfSceneEditor>
	{
		using Ents = HashMap<UInt32, AEntity>;
		using RefEnts = HashMap<UInt32, AEntity*>;
		friend class AGuiOf<GuiOfSceneEditor>;
	public:
		bool bIsEnabled = false;
	private:
		GuiOfSceneEditor();
	public:
		// --core_methods
		virtual void OnDraw() override;
	private:
		inline void OnDraw(AEntity* pEnt);
		inline void OnDraw(Ents& rEnts);
		inline void OnDraw(RefEnts& rRefEnts);
		inline void OnDraw(GCamera* pGCamera);
	private:
		ATexture2d* pIcoCamera = nullptr;
		AEntity* pDestroyEnt = nullptr;
	};
#endif
}

#endif	// NW_GUI_OF_H