#include <nw_pch.hpp>
#include <ecs/nw_scene.h>

#include <core/nw_core_engine.h>
#include <core/nw_core_state.h>
#include <lua/nw_lua_vm.h>

#include <lua.hpp>

namespace NW
{
	LuaScriptCmp::LuaScriptCmp(AEntity& rEntity) :
		AEntityCmp(rEntity, std::type_index(typeid(LuaScriptCmp))),
		m_Script(LuaScript(rEntity.GetName()))
	{
		m_Script.SetCode(
			"function update()\nend\n"
			"function on_enable()\nio.write(\"Yeah! you have enabled my script...\")\nend\n"
			"function on_disable()\nio.write(\"No! you have disabled my script...\")\nend\n");
		m_Script.Compile();
	}

	// --setters
	void LuaScriptCmp::SetEnabled(bool bIsEnabled) {
		AEntityCmp::SetEnabled(bIsEnabled);
		if (bIsEnabled) { OnEnable(); }
		else { OnDisable(); }
	}
	void LuaScriptCmp::SetLuaName(const char* strName) {
		m_Script.SetName(strName);
		m_Script.Compile();
	}

	// --==<Interface Methods>==--
	void LuaScriptCmp::OnUpdate()
	{
		sprintf(&m_strBuf[0], "%s_update\0", &m_Script.GetName()[0]);
		LuaVM::Get().CallFunctionGlb(&m_strBuf[0]);
	}
	void LuaScriptCmp::OnEnable()
	{
		sprintf(&m_strBuf[0], "%s_on_enable\0", &m_Script.GetName()[0]);
		LuaVM::Get().CallFunctionGlb(&m_strBuf[0]);
	}
	void LuaScriptCmp::OnDisable()
	{
		sprintf(&m_strBuf[0], "%s_on_disable\0", &m_Script.GetName()[0]);
		LuaVM::Get().CallFunctionGlb(&m_strBuf[0]);
	}
	// --==</Interface Methods>==--
}