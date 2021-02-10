#ifndef NW_LUA_ENGINE_H
#define NW_LUA_ENGINE_H

#include <lua_core.hpp>
#include <lua/lua_tools.h>

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
		// --predicates
		inline Bit IsRunning() const { return m_bIsRunning; }

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
	private:
		Bit m_bIsRunning;
		MemArena m_MemAllocator;

		LuaState* m_LState;
		LuaEngineInfo m_Info;
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
		static void* LuaAllocate(Ptr pAllocator, void* pBlock, Size szNew, Size szOld);
		static void* LuaWarn(Ptr pWarner, const char* strMsg, Int32 nTocont);
	};
}

#endif	// NW_LUA_ENGINE_H