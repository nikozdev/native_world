#ifndef NW_GUI_OF_H
#define NW_GUI_OF_H

#include <core/nw_data_res.h>
#include <lib/utils/nw_code_chunk.h>

#include <glib/nw_gbuffer.h>
#include <glib/render/nw_texture.h>

#include <nw_decl.hpp>
#include <gui_decl.hpp>
#include <glib_decl.hpp>
#include <ecs_decl.hpp>

#include <lib/utils/math_vector.h>

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
	/// GuiOfEngine struct
	/// Description:
	/// -- Renders the core engine state and window gui
	struct NW_API GuiOfCoreEngine : public AGuiOf<GuiOfCoreEngine>
	{
		friend class AGuiOf<GuiOfCoreEngine>;
	public:
		bool bWindow = false, bAppState = false, bLuaVM = false;
	private:
		GuiOfCoreEngine();
		~GuiOfCoreEngine();
	public:
		virtual void OnDraw() override;
	private:
		// --core_engine
		CoreState* pCoreState = nullptr;
		AWindow* pWindow = nullptr;
		Char strWindowTitle[256] = "nw_engine";
	};
	/// GuiOfGraphEngine struct
	struct NW_API GuiOfGraphEngine : public AGuiOf<GuiOfGraphEngine>
	{
		friend class AGuiOf<GuiOfGraphEngine>;
	private:
		GuiOfGraphEngine();
		~GuiOfGraphEngine();
	private:
		bool bGApi = false, bGContext = false, bStates = false;
		Int32 szMaxVtx = 0, szMaxIdx = 0, szMaxShd = 0, unMaxTex = 0;
		String strDrawMode = "MD_FILL";
		float nLineW = 0.01f, nPixelSz = 0.01f;
		Char strStateName[256];
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
	// --data_system
		Char strDir[256];
		Char strCurrDir[256];
	// --time system
	};
	struct NW_API GuiOfMemSys : public AGuiOf<GuiOfMemSys>
	{
	public:
		virtual void OnDraw() override;
	};
	/// GuiOfTimeSys struct
	struct NW_API GuiOfTimeSys : public AGuiOf<GuiOfTimeSys>
	{
	public:
		virtual void OnDraw() override;
	private:
		Char strDate[256] = "11/01/2021";
	};
	/// GuiOfLogSys struct
	struct NW_API GuiOfLogSys : public AGuiOf<GuiOfLogSys>
	{
	public:
		virtual void OnDraw() override;
	};
	/// GuiOfRandSys struct
	struct NW_API GuiOfRandSys : public AGuiOf<GuiOfRandSys>
	{
	public:
		bool bRand = false;
		Byte btRand = 0;
		UByte ubtRand = 0;
		Int32 nRand = 0;
		UInt32 unRand = 0;
		Float32 fRand = 0.0f;
		Float64 dRand = 0.0f;
	public:
		virtual void OnDraw() override;
	};
}
namespace NW
{
	/// GuiOfCodeEditor struct
	struct NW_API GuiOfCodeEditor : public AGuiOf<GuiOfCodeEditor>
	{
		friend class AGuiOf<GuiOfCodeEditor>;
	private:
		LuaScript* pContextScr = nullptr;
		AShader* pContextShd = nullptr;
		DArray<char> strCodeBuf;
	private:
		GuiOfCodeEditor();
	public:
		// --setters
		void SetContext(ACodeChunk* pContext);
		
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

		DArray<SubTexture2d> Sprites;

		AFrameBuf* pFrameBuf = nullptr;
		ATexture2d* pBGTex = nullptr;
		float nAspectRatio = 1.0f;
		
		RefOwner<AVertexBuf> pVtxBuf;
		RefOwner<AIndexBuf> pIndBuf;
		AShader* pShader = nullptr;

		ImageInfo ImgInfo;
		TextureInfo TexInfo;

		bool bIsSelection = false;
	};
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
		bool bIsEnabledCamera = false;
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
}

#endif	// NW_GUI_OF_H