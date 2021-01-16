#ifndef ECS_AENTITY_COMPONENT_H
#define ECS_AENTITY_COMPONENT_H

#include <core/nw_data_res.h>

#include <ecs_decl.hpp>

namespace NW
{
	/// Abstract EntityComponent class
	class NW_API AEntityCmp : public ADataRes
	{
		friend class AEntity;
	public:
		AEntityCmp(AEntity& rEntity, const TypeInfo& rTypeInfo);
		AEntityCmp(const AEntityCmp& rCpy);
		virtual ~AEntityCmp();

		// --getters
		inline AEntity* GetEntity() { return m_pEntity; }
		inline const TypeInfo GetTypeInfo() const { return m_TypeInfo; }
		// --setters
		virtual inline void SetEnabled(bool bIsEnabled) { m_bIsEnabled = bIsEnabled; }
		// -- Predicates
		inline bool IsEnabled() const { return m_bIsEnabled; }

		// --core_methods
		virtual void OnUpdate() = 0;
		// --data_methods
		virtual bool SaveF(const char* strName) override { return true; }
		virtual bool LoadF(const char* strName) override { return true; }
	private:
		TypeInfo m_TypeInfo;
		bool m_bIsEnabled;
		AEntity *m_pEntity;
	};
}

#endif	// ECS_AENTITY_COMPONENT_H