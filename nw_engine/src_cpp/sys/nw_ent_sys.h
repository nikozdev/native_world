#ifndef NW_ENTITY_SYSTEM_H
#define NW_ENTITY_SYSTEM_H

#include <core/nw_core_engine.h>

namespace NW
{
	using EntId = UInt32;
	using CmpId = UInt32;

	class NW_API EntSys;
	class NW_API AEntity;

	class NW_API CmpSys;
}

namespace NW
{
	/// Abstract Entity class
	class NW_API AEntity
	{
		friend class EntSys;
	public:
		AEntity(EntId eId);
		AEntity(const AEntity& rCpy);
		virtual ~AEntity();

		// --getters
		inline const EntId GetEntId() const { return m_eId; }
		// --setters
		void SetEnabled(bool bIsEnabled);
		// --predicates
		inline bool IsEnabled() { return m_bIsEnabled; }
		// --operators
	protected:
		EntId m_eId;
		bool m_bIsEnabled;
	};
}
namespace NW
{
	/// EntitySystem static class
	class NW_API EntSys
	{
	public:
		// --getters
		static inline AMemAllocator& GetMemory() { static MemArena s_Memory; return s_Memory; }
		static inline AEntity* GetEnt(EntId eId);
		// --setters
		template<typename...Args>
		static inline EntId AddEnt(Args...Arguments);
		static inline void RmvEnt(EntId eId);
		// --core_methods
		static void OnInit(Size szMem);
		static void OnQuit();
	};
	inline AEntity* EntSys::GetEnt(EntId eId) {
		AEntity* pEnt = &static_cast<AEntity*>(GetMemory().GetDataBeg())[eId];
		return pEnt->GetEntId() == eId ? pEnt : nullptr;
	}
	template<typename...Args>
	inline EntId EntSys::AddEnt(Args...Arguments) {
		EntId eId = GetEnts()->GetAllocCount();
		MemSys::NewT<AEntity>(*GetEnts(), eId, std::forward<Args>(Arguments)...);
		return eId;
	}
	inline void EntSys::RmvEnt(EntId eId) {
		AEntity* pEnt = GetEnt(eId);
		if (pEnt == nullptr) { return; }
		DelT<AEntity>(GetMemory(), pEnt);
	}
}

namespace NW
{
	class NW_API ACmp
	{
	public:
		ACmp() : m_eId(0) {}
		virtual ~ACmp() {}
		// --getters
		inline EntId GetEntId() const { return m_eId; }
		inline void SetEntId(EntId eId) { m_eId = eId; }
	private:
		EntId m_eId;
	};
	/// TransformCmp class
	class NW_API TransformCmp : public ACmp
	{
	public:
		TransformCmp() {}
		~TransformCmp() {}

		// --operators
	public:
		V3f xyzCrd = { 0.0f ,0.0f, 0.0f };
		V3f xyzRtn = { 0.0f ,0.0f, 0.0f };
		V3f xyzScl = { 0.0f ,0.0f, 0.0f };
	};
}
namespace NW
{
	/// CmpSys static class
	class NW_API CmpSys
	{
	public:
		// --getters
		static inline AMemAllocator& GetMemory() { static MemArena s_Memory; return s_Memory; }
		template<class CType>
		static inline AMemAllocator& GetCmps();
		template<class CType>
		static inline CType* GetCmp(EntId eId);
		// --setters
		// --core_methods
		static void OnInit(Size szMem);
		static void OnQuit();

		template<class CType, typename ... Args>
		static inline CType* AddCmp(EntId eId, Args... Arguments);
		template<class EType>
		static inline void RmvCmp(EntId eId);
	private:
	};
	// --getters
	template<class CType>
	inline AMemAllocator& CmpSys::GetCmps() {
		static Size szMem = GetMemory().GetDataSize() >> 4;
		static Ptr pMem = GetMemory().Alloc(szMem);
		static MemArena* s_tCmps = MemSys::NewT<MemArena>(GetMemory(), pMem, szMem);
		return s_tCmps;
	}
	template<class CType>
	inline CType* CmpSys::GetCmp(EntId eId) {
		CType* pCmp = &( static_cast<CType*>(GetCmps<CType>().GetDataBeg()) )[eId];
		return pCmp;
	}
	// --setters
	template<class CType, typename...Args>
	static inline CType* CmpSys::AddCmp(EntId eId, Args...Arguments) {
		CType* pCmp = &( static_cast<CType*>(GetCmps<CType>().GetDataBeg()) )[eId];
		new(pCmp)CType(std::forward<Args>(Arguments)...);
		pCmp->SetEntId(eId);
		return pCmp;
	}
	template<class CType>
	static inline void CmpSys::RmvCmp(EntId eId) {
		CType* pCmp = GetCmp<CType>(eId);
		if (pCmp->GetEntId() != eId) { return; }
		MemSys::DelT<CType>(GetCmps<CType>(), pCmp);
	}
}

#endif	// NW_ENTITY_SYSTEM_H