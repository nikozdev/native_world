#ifndef ECS_AENTITY_COMPONENT_H
#define ECS_AENTITY_COMPONENT_H

#include <core/nw_data_res.h>

#include <ecs_decl.hpp>

namespace NW
{
	/// Abstract EntityComponent class
	class NW_API AEntityCmp
	{
		using CmpTypeId = std::type_index;
		friend class AEntity;
		friend class Scene;
	public:
		AEntityCmp(AEntity& rEnt, const CmpTypeId& TypeIndex);
		AEntityCmp(AEntityCmp& rCpy);
		virtual ~AEntityCmp();

		// -- Getters
		inline AEntity* GetEntity() { return m_pEntity; }
		inline const UInt32 GetCmpId() { return m_TypeIndex.hash_code(); }
		inline const CmpTypeId& GetTypeId() { return m_TypeIndex; }
		inline const char* GetTypeName() { return &m_TypeIndex.name()[10]; }	// class_NW::{name}
		// -- Setters
		virtual inline void SetEnabled(bool bIsEnabled) { m_bIsEnabled = bIsEnabled; }
		// -- Predicates
		inline bool IsEnabled() { return m_bIsEnabled; }

		// -- Interface Methods
		virtual void OnUpdate() = 0;

		template <class CmpType, typename ... Args> static inline AEntityCmp* Create(Args ... Arguments) {
			return MemSys::NewT<CmpType, Args>(Arguments);
		}
	private:
		AEntity *m_pEntity;
		CmpTypeId m_TypeIndex;
		
		bool m_bIsEnabled;
	};
}

#endif	// ECS_AENTITY_COMPONENT_H