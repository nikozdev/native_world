#ifndef NWG_MEMORY_SYSTEM_H
#define NWG_MEMORY_SYSTEM_H
#include <nwg_pch.hpp>
#include <nwg_memory.h>

namespace NWG
{
	class MemSys
	{
		using Allocators = HashMap<String, AMemAllocator*>;
	public:
		// --getters
		static inline const MemInfo& GetInfo() { return s_MemInfo; }
		static inline AMemAllocator& GetMemory() { static MemArena s_Memory(nullptr, 0); return s_Memory; }
		static inline Allocators& GetAllocators() { static Allocators s_Allocers; return s_Allocers; }
		static inline AMemAllocator* GetAllocator(const char* strName);
		// --setters
		static inline const void SetAllocation(Size szAlloc, Size unAlloc);
		static inline const void SetDeallocation(Size szAlloc, Size unAlloc);
		template<class MAType, typename ...Args>
		static inline AMemAllocator* AddAllocator(const char* strName, Size szMem, Args ...Arguments);
		template<class MAType>
		static inline void RmvAllocator(const char* strName);

		// --core_methods
		static void OnInit(Size szMemory);
		static void OnQuit();

		static inline void* Alloc(Size szAlloc);
		static inline void Dealloc(void* pBlock, Size szDealloc);
		static inline void* Realloc(void* pBlock, Size szOld, Size szNew);

		template<typename MType, typename ... Args>
		static inline MType* NewT(AMemAllocator& rmAllocator, Args& ... Arguments);
		template <typename MType>
		static inline MType* NewTArr(AMemAllocator& rmAllocator, UInt64 unAlloc);
		template<typename MType>
		static inline void DelT(AMemAllocator& rmAllocator, MType* pBlock);
		template <typename MType>
		static inline void DelTArr(AMemAllocator& rmAllocator, MType* pBlock, UInt64 unDealloc);
		template<typename MType, typename ... Args>
		static inline void* NewPlaceT(MType* pBlock, Args&& ... Arguments);
	private:
		static MemInfo s_MemInfo;
	};
	// --getters
	inline AMemAllocator* MemSys::GetAllocator(const char* strName) {
		auto& s_Allocers = GetAllocators();
		auto& itAllocer = s_Allocers.find(strName);
		return itAllocer == s_Allocers.end() ? nullptr : itAllocer->second;
	}
	// --setters
	inline const void MemSys::SetAllocation(Size szAlloc, Size unAlloc) { s_MemInfo.szAlloc += szAlloc; s_MemInfo.unAlloc += unAlloc; }
	inline const void MemSys::SetDeallocation(Size szDealloc, Size unDealloc) { s_MemInfo.szAlloc -= szDealloc; s_MemInfo.unAlloc -= unDealloc; }
	template<class MAType, typename...Args>
	inline AMemAllocator* MemSys::AddAllocator(const char* strName, Size szMem, Args...Arguments) {
		auto& s_Allocers = GetAllocators();
		Ptr pMem = Alloc(szMem);
		s_Allocers[strName] = NewT<MAType>(GetMemory(), pMem, szMem, std::forward<Args>(Arguments)...);
		return s_Allocers[strName];
	}
	template<class MAType>
	inline void MemSys::RmvAllocator(const char* strName) {
		auto& s_Allocers = GetAllocators();
		auto& itAllocer = s_Allocers.find(strName);
		if (itAllocer == s_Allocers.end()) { return; }
		DelT<MAType>(GetMemory(), static_cast<MAType*>(itAllocer->second));
		s_Allocers.erase(itAllocer);
	}
	// --==<core_methods>==--
	inline void* MemSys::Alloc(Size szAlloc) {
		SetAllocation(szAlloc, 1);
		return GetMemory().Alloc(szAlloc);
	}
	inline void MemSys::Dealloc(void* pBlock, Size szDealloc) {
		SetDeallocation(szDealloc, 1);
		GetMemory().Dealloc(pBlock, szDealloc);
	}
	inline void* MemSys::Realloc(void* pBlock, Size szOld, Size szNew) {
		SetDeallocation(szOld, 1);
		SetAllocation(szNew, 1);
		return GetMemory().Realloc(pBlock, szOld, szNew);
	}
	// --templated
	template<typename MType, typename ... Args>
	inline MType* MemSys::NewT(AMemAllocator& rmAllocator, Args& ... Arguments) {
		MType* pBlock = reinterpret_cast<MType*>(rmAllocator.Alloc(1 * sizeof(MType), __alignof(MType)));
		NewPlaceT<MType>(pBlock, std::forward<Args>(Arguments)...);
		return pBlock;
	}
	template <typename MType>
	inline MType* MemSys::NewTArr(AMemAllocator& rmAllocator, UInt64 unAlloc) {
		return reinterpret_cast<MType*>(rmAllocator.Alloc(unAlloc * sizeof(MType), __alignof(MType)));
	}
	template<typename MType>
	inline void MemSys::DelT(AMemAllocator& rmAllocator, MType* pBlock) {
		pBlock->~MType();
		rmAllocator.Dealloc(pBlock, 1 * sizeof(MType));
	}
	template <typename MType>
	inline void MemSys::DelTArr(AMemAllocator& rmAllocator, MType* pBlock, UInt64 unDealloc) {
		for (Size bi = 0; bi < unDealloc; bi++) { pBlock[bi].~MType(); }
		rmAllocator.Dealloc(pBlock, unDealloc * sizeof(MType));
	}
	template<typename MType, typename ... Args>
	inline void* MemSys::NewPlaceT(MType* pBlock, Args&& ... Arguments) {
		return new(pBlock)MType(std::forward<Args>(Arguments)...);
	}
	// --==</core_methods>==--
}

#endif // NWG_MEMORY_SYSTEM_H