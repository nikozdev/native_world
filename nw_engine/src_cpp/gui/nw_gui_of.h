#ifndef NW_GUI_OF_H
#define NW_GUI_OF_H

#include <core/nw_data_res.h>
#include <lib/utils/nw_code_chunk.h>

#include <nw_decl.hpp>
#include <gui_decl.hpp>
#include <gl_decl.hpp>
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
	/// AGuiOf struct
	/// GuiOfEngine struct
	/// Description:
	/// -- Renders the engine state and systems
	struct NW_API GuiOfEngine : public AGuiOf<GuiOfEngine>
	{
		friend class AGuiOf<GuiOfEngine>;
	public:
		bool bWindow = false, bAppState = false, bLuaVM = false;
	private:
		AWindow* pWindow = nullptr;
		CoreState* pEState = nullptr;
		Char strStateScript[1024] = "Script";
		Char strWindowTitle[128] = "NW_Engine";
	private:
		GuiOfEngine();
		~GuiOfEngine();
	public:
		virtual void OnDraw() override;
	};
	/// GuiOfDataSys struct
	struct NW_API GuiOfDataSys : public AGuiOf<GuiOfDataSys>
	{
	public:
		virtual void OnDraw() override;
	};
	/// GuiOfMemSys struct
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
	/// GuiOfRender struct
	struct NW_API GuiOfRender : public AGuiOf<GuiOfRender>
	{
	private:
		bool bGApi = false, bGContext = false, bDrawEngineInfo = false;
		Int32 szMaxVtx = 0, szMaxIdx = 0, unTexCount = 0;
		String strDrawMode = "MD_FILL";
		float nLineW = 0.01f, nPixelSz = 0.01f;
	public:
		virtual void OnDraw() override;
	};
	/// GuiOfConsole struct
	struct NW_API GuiOfConsole : public AGuiOf<GuiOfConsole>
	{
		friend class AGuiOf<GuiOfConsole>;
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
	private:
		GuiOfConsole();
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
	struct NW_API GuiOfSpriteEditor : public AGuiOf<GuiOfSpriteEditor>
	{
		friend class AGuiOf<GuiOfSpriteEditor>;
	public:
		AFrameBuf* pFrameBuf = nullptr;
	private:
		GuiOfSpriteEditor();
	public:
		// --getters
		// --setters
		void SetContext(SubTexture2d* pContext);

		// --core_methods
		virtual void OnDraw() override;
	private:
		SubTexture2d* pContext = nullptr;
		ATexture2d* pBGTex = nullptr;
		float nAspectRatio = 1.0f;
		
		RefOwner<AVertexBuf> pVtxBuf;
		RefOwner<AIndexBuf> pIndBuf;
		AShader* pShader = nullptr;

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
		using Ents = List2<AEntity>;
		using RefEnts = DArray<AEntity*>;
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
		inline void OnDraw(RefEnts& rRefEnts);
		inline void OnDraw(GCamera* pGCamera);
	private:
		AEntity* pDestroyEnt = nullptr;
		ATexture2d* pIcoCamera = nullptr;
	};
}

#endif	// NW_GUI_OF_H