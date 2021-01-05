#ifndef ECS_SCRIPT_COMPONENT_H
#define ECS_SCRIPT_COMPONENT_H
#include <ecs/nw_entity_cmp.h>

#include <lib/utils/nw_code_chunk.h>

#include <nw_decl.hpp>

namespace NW
{
	/// LuaScriptComponent class
	class NW_API LuaScriptCmp : public AEntityCmp
	{
	public:
		LuaScriptCmp(AEntity& rEntity);
		~LuaScriptCmp() = default;

		// -- Getters
		inline LuaScript* GetScript() { return &m_Script; }
		// -- Setters
		void SetLuaName(const char* strName);
		virtual inline void SetEnabled(bool bIsEnabled) override;

		// -- Core Methods
		virtual void OnUpdate() override;
		void OnEnable();
		void OnDisable();
	private:
		Char m_strBuf[128];

		LuaScript m_Script;
	};
}

#endif	// ECS_SCRIPT_COMPONENT_H