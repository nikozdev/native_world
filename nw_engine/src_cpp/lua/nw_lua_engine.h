#ifndef LUA_ENGINE_H
#define LUA_ENGINE_H

#include <nwlib/nw_singleton.h>
#include <nwlib/nw_memory.h>

#include <lua/nw_lua_core.h>

namespace NW
{
	/// LuaVirtualMachiene singleton class
	/// -- Handles lua state
	/// -- Performs current script of the engine state
	class NW_API LuaEngine : public ASingleton<LuaEngine>
	{
	public:
		friend class ASingleton<LuaEngine>;
	private:
		LuaState* m_LState;
		LuaEngineInfo m_Info;
		MemArena<Byte> m_MemAllocator;
	private:
		LuaEngine();
		LuaEngine(LuaEngine& rCpy) = delete;
		void operator=(LuaEngine& rCpy) = delete;
	public:
		~LuaEngine();

		// --getters
		inline LuaState* GetLuaState() { return m_LState; }
		inline const LuaEngineInfo& GetInfo() const { return m_Info; }
		template <typename ValType> ValType GetLuaValueGlb(const char* strName);
		template <typename ValType> ValType GetLuaValueLoc(const char* strName);
		template <typename ValType> ValType GetLuaValue(const char* strLoc, const char* strName);
		// --setters
		template <typename ValType> bool SetLuaValueGlb(const ValType& tValue, const char* strName);
		template <typename ValType> bool SetLuaValueLoc(const ValType& tValue, const char* strName);
		template <typename ValType> bool SetLuaValue(const ValType& tValue, const char* strLoc, const char* strName);

		// --core_methods
		LuaState* CreateLuaState();
		void DestroyLuaState(LuaState* pLState);

		bool LoadScript(const String& strScript);

		bool RunScriptStr(const char* strScript);
		bool RunScriptF(const char* strFPath);
		
		bool CallFunctionGlb(const char* strName);
		bool CallFunctionLoc(const char* strName);
		bool CallFunction(const char* strLoc, const char* strName);
		
		bool LuaReg(LuaRegFunc& rRegFunc);
		bool LuaReg(LuaRegTable& rRegTab);
	private:	// Implementation Methods
		inline bool LuaMoveGlb(const char* strName);
		inline bool LuaMoveLoc(const char* strName);
		inline bool LuaMove(const char* strLoc, const char* strName);

		inline bool LuaMoveTGlb(const char* strName, LuaTypes nLuaType);
		inline bool LuaMoveTLoc(const char* strName, LuaTypes nLuaType);
		inline bool LuaMoveT(const char* strLoc, const char* strName, LuaTypes nLuaType);
		
		inline void LuaClear();
		inline void LuaClear(UInt8 unCount);
		inline bool LuaPush(LuaRegFunc& rRegFunc);
		inline bool LuaPush(LuaRegTable& rRegTab);
		inline void WriteErr(const char* strFormat, ...);
		inline void WriteInfo();

		static int LuaWrite(LuaState* pState, void* pBlock, Size szData, void* pWriter);
		static const char* LuaRead(LuaState* pState, void* pReader, Size* szData);
		static void* LuaAllocate(void* pAllocator, void* pBlock, Size szNew, Size szOld);
		static void* LuaWarn(void* pWarner, const char* strMsg, Int32 nTocont);
	};
}

#endif	//LUA_ENGINE_H