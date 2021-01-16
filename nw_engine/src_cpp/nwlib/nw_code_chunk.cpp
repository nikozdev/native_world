#include <nw_pch.hpp>
#include "nw_code_chunk.h"

#include <lua/nw_lua_engine.h>

namespace NW
{
	// --==<code_chunk>==--
	ACodeChunk::ACodeChunk(const char* strName) :
		ADataRes(strName),
		m_strCode("")
	{ }
	ACodeChunk::~ACodeChunk()
	{ }
	// --==</code_chunk>==--

	// --==<lua_script>==--
	LuaScript::LuaScript(const char* strName) :
	ACodeChunk(strName)
	{
		//
	}
	LuaScript::~LuaScript() {}

	// --core_methods
	bool LuaScript::Compile()
	{
		Char strBuf[128]{0};
		String strResult = m_strCode;
		Int32 nCurr = 0;

		if ((nCurr = strResult.find("update")) != -1) {
			sprintf(&strBuf[0], "%s_", &GetCode()[0]);
			strResult.insert(nCurr, &strBuf[0]);
		}
		else {
			sprintf(&strBuf[0], "\nfunction %s_update() end", &m_strName[0]);
			strResult.append(&strBuf[0]);
		}
		if ((nCurr = strResult.find("on_enable")) != -1) {
			sprintf(&strBuf[0], "%s_", &m_strName[0]);
			strResult.insert(nCurr, &strBuf[0]);
		}
		else {
			sprintf(&strBuf[0], "\nfunction %s_on_enable() end", &m_strName[0]);
			strResult.append(&strBuf[0]);
		}
		if ((nCurr = strResult.find("on_disable")) != -1) {
			sprintf(&strBuf[0], "%s_", &m_strName[0]);
			strResult.insert(nCurr, &strBuf[0]);
		}
		else {
			sprintf(&strBuf[0], "\nfunction %s_on_disable() end", &m_strName[0]);
			strResult.append(&strBuf[0]);
		}

		return LuaEngine::Get().RunScriptStr(&strResult[0]);
	}
	// --data_methods
	bool LuaScript::SaveF(const char* strFPath)
	{
		return true;
	}
	bool LuaScript::LoadF(const char* strFPath)
	{
		return true;
	}
	// --==</lua_script>==--
}