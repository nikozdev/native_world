#include <nw_pch.hpp>
#include "lua_engine.h"

#include <lua.hpp>
#include <lstate.h>
#include <lobject.h>

namespace NW
{
	LuaEngine::LuaEngine() :
		m_MemAllocator(MemArena(nullptr, 0)),
		m_LState(nullptr)
	{
		m_LState = CreateLuaState();
	}
	LuaEngine::~LuaEngine()
	{
		DestroyLuaState(m_LState);
	}

	// --getters
	template <> Int32 LuaEngine::GetLuaValueGlb<Int32>(const char* strName) {
		Int32 nRes = 0;
		if (LuaMoveTGlb(strName, LT_NUM)) {
			nRes = static_cast<Int32>(lua_tonumberx(m_LState, -1, 0));
		}
		else {
			nRes = 0;
		}
		LuaClear(1);
		return nRes;
	}
	template <> Int32 LuaEngine::GetLuaValueLoc<Int32>(const char* strName) {
		Int32 nRes = 0;
		if (LuaMoveTLoc(strName, LT_NUM)) {
			nRes = static_cast<Int32>(lua_tonumberx(m_LState, -1, 0));
		}
		else {
			nRes = 0;
		}
		LuaClear(1);
		return nRes;
	}
	template <> Int32 LuaEngine::GetLuaValue<Int32>(const char* strLoc, const char* strName) {
		Int32 nRes = 0;
		if (LuaMoveT(strLoc, strName, LT_NUM)) {
			nRes = static_cast<Int32>(lua_tonumberx(m_LState, -1, 0));
		}
		else {
			nRes = 0;
		}
		LuaClear();
		return nRes;
	}
	// --setters
	template <> bool LuaEngine::SetLuaValueGlb<Int32>(const Int32& tValue, const char* strName) {
		lua_pushnumber(m_LState, tValue);
		m_Info.AddType(lua_type(m_LState, -1));
		if (LuaMoveGlb(strName)) { LuaClear(2); return false; }
		lua_setglobal(m_LState, strName);
		m_Info.RemoveType(1);
		return true;
	}
	template <> bool LuaEngine::SetLuaValueLoc<Int32>(const Int32& tValue, const char* strName) {
		if (m_Info.GetStackType(m_Info.GetLevel() - 1) == LT_TAB) {
			lua_pushnumber(m_LState, tValue);
			m_Info.AddType(lua_type(m_LState, -1));
			if (LuaMoveLoc(strName)) { LuaClear(2); return false; }
			lua_setfield(m_LState, -2, strName);
			m_Info.RemoveType(1);
			return true;
		}
		else { return false; }
	}
	template <> bool LuaEngine::SetLuaValue<Int32>(const Int32& tValue, const char* strLoc, const char* strName) {
		if (LuaMoveT(strLoc, strName, LT_NUM)) { LuaClear(); return false; }
		else {
			m_Info.AddType(lua_type(m_LState, -1));
			lua_pushnumber(m_LState, tValue);
			lua_setfield(m_LState, -2, strName);
			m_Info.RemoveType(1);
		}
		LuaClear();
		return true;
	}

	// --==<core_methods>==--
	//LuaRegTable LuaCmpSys::lrtCmpSys = LuaRegTable("cmp_sys");
	//LuaRegTable LuaEntSys::lrtEntSys = LuaRegTable("ent_sys");
	LuaState* LuaEngine::CreateLuaState()
	{
		//m_MemAllocator = MemPool(1024 * 8);

		LuaState* pLState = lua_newstate(LuaEngine::LuaAllocate, &m_MemAllocator);
		//LuaState* pLState = luaL_newstate();
		//lua_setallocf(pLState, LuaEngine::LuaAllocate, &m_MemAllocator);

		luaL_openlibs(pLState);
		m_LState = pLState;
#if false
		{
			// Cmp methods
			LuaRegFunc lrfGetCmpId = LuaRegFunc("get_cmp_id", LuaCmpSys::GetCmpId);
			LuaCmpSys::lrtCmpSys.lrfMethods.push_back(&lrfGetCmpId);
			LuaRegFunc lrfGetCmpName = LuaRegFunc("get_cmp_name", LuaCmpSys::GetCmpName);
			LuaCmpSys::lrtCmpSys.lrfMethods.push_back(&lrfGetCmpName);
			LuaRegFunc lrfGetCmpEnt = LuaRegFunc("get_cmp_ent", LuaCmpSys::GetCmpEnt);
			LuaCmpSys::lrtCmpSys.lrfMethods.push_back(&lrfGetCmpEnt);
			// CmpSys methods
			LuaRegFunc lrfGetCmp = LuaRegFunc("get_cmp", LuaCmpSys::GetCmp);
			LuaCmpSys::lrtCmpSys.lrfMethods.push_back(&lrfGetCmp);
			LuaRegFunc lrfCreateCmp = LuaRegFunc("create_cmp", LuaCmpSys::CreateCmp);
			LuaCmpSys::lrtCmpSys.lrfMethods.push_back(&lrfCreateCmp);
			LuaRegFunc lrfDestroyCmp = LuaRegFunc("destroy_cmp", LuaCmpSys::DestroyCmp);
			LuaCmpSys::lrtCmpSys.lrfMethods.push_back(&lrfDestroyCmp);
			// Metamethods
			LuaRegFunc lrfIndex = LuaRegFunc("__index", LuaCmpSys::Index);
			LuaCmpSys::lrtCmpSys.lrfMethods.push_back(&lrfIndex);
			LuaRegFunc lrfNewIndex = LuaRegFunc("__newindex", LuaCmpSys::NewIndex);
			LuaCmpSys::lrtCmpSys.lrfMethods.push_back(&lrfNewIndex);

			LuaReg(LuaCmpSys::lrtCmpSys);
		}
		{
			// Ent methods
			LuaRegFunc lrfGetEntId = LuaRegFunc("get_ent_id", LuaEntSys::GetEntId);
			LuaEntSys::lrtEntSys.lrfMethods.push_back(&lrfGetEntId);
			LuaRegFunc lrfGetEntName = LuaRegFunc("get_ent_name", LuaEntSys::GetEntName);
			LuaEntSys::lrtEntSys.lrfMethods.push_back(&lrfGetEntName);
			LuaRegFunc lrfGetOverEnt = LuaRegFunc("get_over_ent", LuaEntSys::GetOverEnt);
			LuaEntSys::lrtEntSys.lrfMethods.push_back(&lrfGetOverEnt);

			LuaRegFunc lrfSetName = LuaRegFunc("set_ent_name", LuaEntSys::SetEntName);
			LuaEntSys::lrtEntSys.lrfMethods.push_back(&lrfSetName);
			LuaRegFunc lrfSetEntEnabled = LuaRegFunc("set_ent_enabled", LuaEntSys::SetEntEnabled);
			LuaEntSys::lrtEntSys.lrfMethods.push_back(&lrfSetEntEnabled);
			LuaRegFunc lrfSetOverEnt = LuaRegFunc("set_over_ent", LuaEntSys::SetOverEnt);
			LuaEntSys::lrtEntSys.lrfMethods.push_back(&lrfSetOverEnt);
			// EntSys methods
			LuaRegFunc lrfGetEnt = LuaRegFunc("get_ent", LuaEntSys::GetEnt);
			LuaEntSys::lrtEntSys.lrfMethods.push_back(&lrfGetEnt);
			LuaRegFunc lrfCreateEnt = LuaRegFunc("create_ent", LuaEntSys::CreateEnt);
			LuaEntSys::lrtEntSys.lrfMethods.push_back(&lrfCreateEnt);
			LuaRegFunc lrfDestroyEnt = LuaRegFunc("destroy_ent", LuaEntSys::DestroyEnt);
			LuaEntSys::lrtEntSys.lrfMethods.push_back(&lrfDestroyEnt);
			// Metamethods
			LuaRegFunc lrfIndex = LuaRegFunc("__index", LuaEntSys::Index);
			LuaEntSys::lrtEntSys.lrfMethods.push_back(&lrfIndex);
			LuaRegFunc lrfNewIndex = LuaRegFunc("__newindex", LuaEntSys::NewIndex);
			LuaEntSys::lrtEntSys.lrfMethods.push_back(&lrfNewIndex);

			LuaReg(LuaEntSys::lrtEntSys);
		}
#endif

		return pLState;
	}
	void LuaEngine::DestroyLuaState(LuaState* pLState)
	{
		lua_close(pLState);
		m_MemAllocator = MemArena(nullptr, 0);
	}

	bool LuaEngine::LoadScript(const String& strScript)
	{
		LuaReaderState lRState;
		lRState.strChunk = &strScript;
		Int32 nResult = 0;
		//lua_load(m_LState, LuaEngine::LuaRead, &lRState, "lua_chunk", "r");
		return nResult == LUA_OK;
	}

	bool LuaEngine::RunScriptStr(const char* strScript)
	{
		bool bResult = true;
		luaL_loadstring(m_LState, &strScript[0]);
		m_Info.AddType(lua_type(m_LState, -1));
		if (lua_pcallk(m_LState, 0, 0, 0, 0, 0) != LUA_OK) {
			WriteErr("Could not run the script");
			bResult = false;
		}
		else {
			bResult = true;
		}
		LuaClear(1);

		return bResult;
	}
	bool LuaEngine::RunScriptF(const char* strFPath)
	{
		bool bResult = true;
		if (luaL_dofile(m_LState, &strFPath[0]) != LUA_OK) {
			WriteErr("Could not run the script");
			bResult = false;
		}
		else {
			bResult = true;
		}
		return bResult;
	}
	
	bool LuaEngine::CallFunctionGlb(const char* strName)
	{
		bool bResult = true;
		bResult = LuaMoveTGlb(strName, LT_FNC);
		if (bResult) { bResult = (lua_pcallk(m_LState, 0, 0, 0, 0, 0) == LUA_OK); LuaClear(1); }
		else { WriteErr("Function \"{str}\" was not found;", strName); }
		return bResult;
	}
	bool LuaEngine::CallFunctionLoc(const char* strName)
	{
		bool bResult = true;
		bResult = LuaMoveTLoc(strName, LT_FNC);
		if (bResult) { bResult = (lua_pcallk(m_LState, 0, 0, 0, 0, 0) == LUA_OK); LuaClear(1); }
		else { WriteErr("Function \"{str}\" was not found;", strName); }
		return bResult;
	}
	bool LuaEngine::CallFunction(const char* strLoc, const char* strName)
	{
		bool bResult = true;
		bResult = LuaMoveT(strLoc, strName, LT_FNC);
		if (bResult) { bResult = (lua_pcallk(m_LState, 0, 0, 0, 0, 0) == LUA_OK); LuaClear(); }
		else { WriteErr("Function \"{str}\" was not found;", strName); }
		return bResult;
	}

	bool LuaEngine::LuaReg(LuaRegFunc& rRegFunc)
	{
		bool bIsGlb = strcmp(rRegFunc.strLoc, "") == 0;
		if (bIsGlb) {	// The function is global
			if (LuaMoveGlb(&rRegFunc.strName[0])) {	// This name is already defined globally
				LuaClear(1);
				return false;
			}
		}
		else if (LuaMove(rRegFunc.strLoc, rRegFunc.strName)) {	// This name is already defined locally
			LuaClear();
			return false;
		}
		if (!LuaPush(rRegFunc)) {	// The function is on the top
			LuaClear();
			return false;
		}
		if (bIsGlb) { lua_setglobal(m_LState, rRegFunc.strName); m_Info.RemoveType(1); }
		else { lua_setfield(m_LState, -2, rRegFunc.strName); m_Info.RemoveType(1); LuaClear(); }
		
		LuaClear();
		return true;
	}
	bool LuaEngine::LuaReg(LuaRegTable& rRegTab)
	{	// Registration of lua table is same as a registration of lua function
		bool bIsGlb = (strcmp(&rRegTab.strLoc[0], "") == 0);
		if (bIsGlb) {
			if (LuaMoveGlb(rRegTab.strName)) {
				LuaClear(1);
				return false;
			}
		}
		else if (LuaMove(rRegTab.strLoc, rRegTab.strName)) {
			LuaClear();
			return false;
		}
		if (!LuaPush(rRegTab)) {
			LuaClear();
			return false;
		}
		if (bIsGlb) { lua_setglobal(m_LState, rRegTab.strName); m_Info.RemoveType(1); }
		else { lua_setfield(m_LState, -2, rRegTab.strName); m_Info.RemoveType(1); LuaClear(); }

		return true;
	}
	// --==</core_methods>==--

	// --==<Implementation Methods>==--
	inline bool LuaEngine::LuaMoveGlb(const char* strName)
	{
		lua_getglobal(m_LState, &strName[0]);
		m_Info.AddType(lua_type(m_LState, -1));
		if (m_Info.GetLastType() == LT_NIL) {		// Value is not found
			LuaClear(1);
			return false;
		}
		return true;
	}
	inline bool LuaEngine::LuaMoveLoc(const char* strName)
	{
		if (m_Info.GetLastType() != LT_TAB) { return false; }
		lua_getfield(m_LState, -1, &strName[0]);
		m_Info.AddType(lua_type(m_LState, -1));
		if (m_Info.GetLastType() == LT_NIL) {		// Value is not found
			LuaClear(1);
			return false;
		}
		return true;
	}
	inline bool LuaEngine::LuaMove(const char* strLoc, const char* strName)
	{	// Get on the top of the stack some value due to string "tab1.tab2.value"
		String strBuf("", 256);		// String buffer for the currently serched name
		StrStream strStream = StrStream(String(strLoc) + '.' + strName);

		// Start with the global namespace
		strStream.getline(&strBuf[0], strBuf.size(), '.');
		if (!LuaMoveGlb(&strBuf[0])) { return false; }

		while (!strStream.eof()) {	// Works only if with the global name is char '.'
			strStream.getline(&strBuf[0], strBuf.size(), '.');
			if (!LuaMoveLoc(&strBuf[0])) { return false; }
		}
		//// We have found a value by the full path. It can be anything
		return true;
	}

	inline bool LuaEngine::LuaMoveTGlb(const char* strName, LuaTypes nLuaType) {
		if (LuaMoveGlb(strName)) { return m_Info.GetLastType() == nLuaType; }
		else { return false; }
	}
	inline bool LuaEngine::LuaMoveTLoc(const char* strName, LuaTypes nLuaType) {
		if (LuaMoveLoc(strName)) { return m_Info.GetLastType() == nLuaType; }
		else { return false; }
	}
	inline bool LuaEngine::LuaMoveT(const char* strLoc, const char* strName, LuaTypes nLuaType) {
		if (LuaMove(strLoc, strName)) { return m_Info.GetLastType() == nLuaType; }
		else { return false; }
	}

	inline void LuaEngine::LuaClear() {
		UInt8 unLevel = lua_gettop(m_LState);
		LuaClear(unLevel);
	}
	inline void LuaEngine::LuaClear(UInt8 unCount)
	{
		m_Info.RemoveType(unCount);
		if (unCount > lua_gettop(m_LState)) { return; }
		lua_pop(m_LState, unCount);
	}

	inline bool LuaEngine::LuaPush(LuaRegFunc& rRegFunc)
	{
		if (rRegFunc.fnNative == nullptr) { return false; }
		if (strcmp(rRegFunc.strName, "") == 0) { return false; }

		LuaUpValues UpValsCpy = rRegFunc.UpValues;
		for (UInt16 uvi = 0; uvi < UpValsCpy.size(); uvi++) {	// Throw all the upvalues on the top of the stack
			if (UpValsCpy.top() == nullptr) {
				LuaClear();
				WriteErr("Native function has null pointers as upvalues");
				return false;
			}
			lua_pushlightuserdata(m_LState, UpValsCpy.top());
			UpValsCpy.pop();
		}
		// Here all upvalues are poped out of the stack
		lua_pushcclosure(m_LState, rRegFunc.fnNative, rRegFunc.UpValues.size());
		m_Info.AddType(lua_type(m_LState, -1));
		return true;
	}
	inline bool LuaEngine::LuaPush(LuaRegTable& rRegTab)
	{
		if (strcmp(rRegTab.strName, "") == 0) { return false; }
		
		lua_createtable(m_LState, 0, 0);
		m_Info.AddType(lua_type(m_LState, -1));
		for (auto pRegMeth : rRegTab.lrfMethods) {
			if (!LuaPush(*pRegMeth)) { LuaClear(); return false; }
			else { lua_setfield(m_LState, -2, pRegMeth->strName); m_Info.RemoveType(1); }
		}
		if (rRegTab.lrtMeta != nullptr) {
			if (!LuaPush(*rRegTab.lrtMeta)) { LuaClear(); return false; }
			lua_pushstring(m_LState, rRegTab.lrtMeta->strName);
			lua_setfield(m_LState, -2, "__name");  // metatable.__name = tname
			lua_pushvalue(m_LState, -1);
			lua_setfield(m_LState, LUA_REGISTRYINDEX, rRegTab.lrtMeta->strName);  // registry.name = metatable
			lua_setmetatable(m_LState, -2); m_Info.RemoveType(1);
		}
		return true;
	}

	inline void LuaEngine::WriteInfo()
	{
		UInt8 unLevel = lua_gettop(m_LState);
		m_Info.RemoveType(unLevel);
		for (UInt16 ti = 0; ti < unLevel; ti++) { m_Info.AddType(lua_type(m_LState, -ti)); }
		
		LogSys::WriteInf(
			"Lua_stack_info\n"
			"status: {int};\nvalues: {int}\nstack_size: {int};",
			m_LState->status, m_LState->stack->val, m_LState->stacksize
		);
		
		std::cout << m_Info;
	}
	inline void LuaEngine::WriteErr(const char* strFormat, ...)
	{
		va_list valArgs;
		va_start(valArgs, strFormat);
		String strErrLog = StrGetFormatVa(strFormat, valArgs);
		va_end(valArgs);
		if (lua_isstring(m_LState, -1)) { strErrLog += lua_tolstring(m_LState, -1, 0); }
		LuaClear();
		NWL_ERR(&strErrLog[0]);
	}
	// --==</Implementation Methods>==--

	// --==<Lua Delegates>==--
	int LuaEngine::LuaWrite(LuaState* pState, void* pBlock, Size szData, void* pWriter)
	{
		return 0;
	}
	const char* LuaEngine::LuaRead(LuaState* pState, void* pData, Size* szData)
	{
		LuaReaderState* pRState = static_cast<LuaReaderState*>(pData);
		if (pRState->bIsRead) {
			return nullptr;
		}
		else {
			pRState->bIsRead = true;
			*szData = pRState->strChunk->length();
			return &(*pRState->strChunk)[0];
		}
		return nullptr;
	}
	void* LuaEngine::LuaAllocate(Ptr pAllocator, void* pBlock, Size szOld, Size szNew)
	{
		AMemAllocator* pMemAllocator = static_cast<AMemAllocator*>(pAllocator);
		if (pBlock != nullptr) {
			if (szNew == 0) {
				pMemAllocator->Dealloc(pBlock, szOld);
			}
			else if (szOld > 0) {
				pBlock = pMemAllocator->Realloc(pBlock, szOld, szNew);
			}
		}
		else if (szNew > 0) {
			pBlock = pMemAllocator->Alloc(szNew);
		}
		return pBlock;
	}
	// --==</Lua Delegates>==--
}

/// What are the functions I am going to add into the lua scripts?
//inline const LuaRegFunc* LuaEngine::GetRegFunction(const char* strName) {
//	RegFunctions::iterator itRegFunc = m_RegFunctions.find(strName);
//	return itRegFunc == m_RegFunctions.end() ? nullptr : &itRegFunc->second;
//}
//inline const LuaRegTable* LuaEngine::GetRegTable(const char* strName) {
//	RegTables::iterator itRegTab = m_RegTables.find(strName);
//	return itRegTab == m_RegTables.end() ? nullptr : &itRegTab->second;
//}
// --setters
//void LuaEngine::AddRegFunction(LuaRegFunc& rRegFunc) {
//	if (HasRegFunction(&rRegFunc.strName[0]) || HasFunction(&rRegFunc.strName[0])) { return; }
//	String strAbsName = "";
//	UInt16 unLevel = 0;
//
//	if (rRegFunc.fnNative != nullptr) { PushRegFunction(rRegFunc.fnNative, rRegFunc.UpValues); }
//	else { return; }
//
//	if (strcmp(rRegFunc.strLoc, "") == 0) {
//		strAbsName = strAbsName + rRegFunc.strName;
//		lua_setglobal(m_LState, &strAbsName[0]);
//	}
//	else {	// Find the location table
//		strAbsName = strAbsName + rRegFunc.strLoc + '.' + rRegFunc.strName;
//		unLevel = LuaMove(&rRegFunc.strLoc[0]);
//		Int32 nType = lua_type(m_LState, -1);
//		if (nType == LUA_TTABLE) {
//			 Get the function on the top again
//			lua_pushvalue(m_LState, -2); unLevel++;
//			 Stack now has ...[-3]function[-2]table[-1]function
//			 Set that function as a part of the table - function goes out
//			lua_setfield(m_LState, -2, &rRegFunc.strName[0]); unLevel--;
//			LuaClear(unLevel);
//		}
//		else { return; }
//	}
//	m_RegFunctions.insert({ &strAbsName[0], rRegFunc });
//}
//void LuaEngine::RemoveRegFunction(const char* strName) {
//	RegFunctions::iterator itRegFunc = m_RegFunctions.find(strName);
//	if (itRegFunc == m_RegFunctions.end()) { return; }
//	m_RegFunctions.erase(itRegFunc);
//}
//void LuaEngine::RemoveRegTable(const char* strName) {
//	RegTables::iterator itRegTab = m_RegTables.find(strName);
//	if (itRegTab == m_RegTables.end()) { return; }
//	m_RegTables.erase(itRegTab);
//}
// -- Predicates
//bool LuaEngine::HasTable(const char* strName) {
//	UInt16 unLevel = LuaMove(strName);
//	bool bHasTab = lua_type(m_LState, -1) == LUA_TTABLE;
//	LuaClear(unLevel);
//	return bHasTab;
//}
//bool LuaEngine::HasRegTable(const char* strName) { return m_RegTables.find(strName) != m_RegTables.end(); }
//bool LuaEngine::HasFunction(const char* strName) {
//	UInt16 unLevel = LuaMove(strName);
//	bool bHasFunc = lua_type(m_LState, -1) == LUA_TFUNCTION;
//	LuaClear(unLevel);
//	return bHasFunc;
//}
//bool LuaEngine::HasRegFunction(const char* strName) { return m_RegFunctions.find(strName) != m_RegFunctions.end(); }
/// This is registration of native types in Lua tables
/// In lua virtual machiene will be created a table with default types and it will be like a class
/// But to use default "__meta" methods it is essential to set metatable from the "fnCreate" function
/// It is enough to "lua_getmetatable(LuaNative, "{RegTable_name}_meta")" and "setmetatable(LuaNative)"
//void LuaEngine::AddRegTable(LuaRegTable& rRegTab) {
//	String strAbsName = "";
//	UInt16 unLevel = 0;
//	if (HasRegTable(&strAbsName[0]) || HasTable(&strAbsName[0])) { return; }
//	if (strcmp(rRegTab.strLoc, "") == 0) {	// Location is global, set the table globally
//		strAbsName = strAbsName + rRegTab.strName;
//		lua_createtable(m_LState, 0, 0);
//		unLevel++;
//		lua_pushvalue(m_LState, -1);	// Keep on the stack the second pointer
//		lua_setglobal(m_LState, &strAbsName[0]);	// Set the table and pop it from the stack
//	}
//	else {	// Find the overtable (location)
//		UInt16 unLevel = LuaMove(&rRegTab.strLoc[0]);
//		if (lua_type(m_LState, -1) == LUA_TTABLE) {		// The over table is on the stack
//			LuaRegTable* pOverTab = &rRegTab;
//			// Set the table as a field of the over table
//			lua_createtable(m_LState, 0, 0);
//			unLevel++;
//			lua_pushvalue(m_LState, -1);
//			lua_setfield(m_LState, -2, &rRegTab.strName[0]);
//		}
//		else {
//			lua_pop(m_LState, 1);
//			WriteErrLog("Over table of \"{str}\" table was not found!", &rRegTab.strName[0]);
//			return;
//		}
//	}
//	if (rRegTab.fnCreate != nullptr) {		// Set the constructor method "Create"
//		PushRegFunction(*rRegTab.fnCreate, rRegTab.UpValues);
//		lua_setfield(m_LState, -2, "create");
//	}
//	// New metatable with special methods specially for this type
//	luaL_newmetatable(m_LState, &(String(rRegTab.strName) + "_meta")[0]);
//	unLevel++;
//	// "__gc" - called whenever the object is garbage collected;
//	if (rRegTab.fnDestroy != nullptr) {
//		PushRegFunction(*rRegTab.fnDestroy, rRegTab.UpValues);
//		lua_setfield(m_LState, -2, "__gc");
//	}
//	// "__index" - called whenever we get some value from the table;
//	if (rRegTab.fnIndex != nullptr) {
//		PushRegFunction(*rRegTab.fnNewIndex, rRegTab.UpValues);
//		lua_setfield(m_LState, -2, "__index");
//	}
//	// "__newindex" - called whenever we set some value into the table
//	if (rRegTab.fnNewIndex != nullptr) {
//		PushRegFunction(*rRegTab.fnNewIndex, rRegTab.UpValues);
//		lua_setfield(m_LState, -2, "__newindex");
//	}
//	lua_setmetatable(m_LState, -2);
//
//	LuaClear(unLevel);
//	m_RegTables.insert({ &strAbsName[0], rRegTab });
//}