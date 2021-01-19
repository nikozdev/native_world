#ifndef NWG_MEMORY_SYSTEM_H
#define NWG_MEMORY_SYSTEM_H
#include <nwg_memory.h>

namespace NWG
{
	class MemSys
	{
	public:
		// --getters
		static inline const MemInfo& GetInfo() { return s_MemInfo; }
		template <typename MType>
		static inline MemArena<MType>& GetArena() { static MemArena<MType> s_Arena(reinterpret_cast<MType*>(s_Memory.Alloc(1 << 18)), 1 << 10); return s_Arena; }
		// --setters
		static inline const void SetAllocation(Size szAlloc, UInt32 unAlloc);
		static inline const void SetDeallocation(Size szAlloc, UInt32 unAlloc);

		// --core_methods
		static void OnInit(Size szMemory);
		static void OnQuit();

		static void* Alloc(Size szAlloc);
		static void* AllocArr(Size szAlloc, UInt32 unAlloc);
		static void Dealloc(void* pBlock, Size szDealloc);
		static void DeallocArr(void* pBlock, Size szDealloc, UInt32 unDealloc);
		static inline void* Realloc(void* pBlock, Size szOld, Size szNew);

		template<typename MType, typename ... Args>
		static inline MType* NewT(Args& ... Arguments);
		template <typename MType>
		static inline MType* NewTArr(UInt32 unAlloc);
		template<typename MType>
		static inline void DelT(MType* pBlock);
		template <typename MType>
		static inline void DelTArr(MType* pBlock, UInt32 unDealloc);
		template<typename MType, typename ... Args>
		static inline void* NewPlaceT(MType* pBlock, Args&& ... Arguments);
	private:
		static MemInfo s_MemInfo;
		static MemArena<Byte> s_Memory;
	};
	// -- setters
	inline const void MemSys::SetAllocation(Size szAlloc, UInt32 unAlloc) {
		s_MemInfo.szAlloc += szAlloc; s_MemInfo.unAlloc += unAlloc;
	}
	inline const void MemSys::SetDeallocation(Size szAlloc, UInt32 unAlloc) {
		s_MemInfo.szAlloc -= szAlloc; s_MemInfo.unAlloc -= unAlloc;
	}
	// --==<core_methods>==--
	inline void* MemSys::Alloc(Size szAlloc) { return s_Memory.Alloc(szAlloc); }
	inline void* MemSys::AllocArr(Size szAlloc, UInt32 unAlloc) { return s_Memory.Alloc(szAlloc); }
	inline void MemSys::Dealloc(void* pBlock, Size szDealloc) { s_Memory.Dealloc(static_cast<Byte*>(pBlock), szDealloc); }
	inline void MemSys::DeallocArr(void* pBlock, Size szDealloc, UInt32 unDealloc) { s_Memory.Dealloc(static_cast<Byte*>(pBlock), szDealloc); }
	inline void* MemSys::Realloc(void* pBlock, Size szOld, Size szNew) { return s_Memory.Realloc(static_cast<Byte*>(pBlock), szOld, szNew); }
	template<typename MType, typename ... Args>
	inline MType* MemSys::NewT(Args& ... Arguments) {
		MType* pBlock = GetArena<MType>().Alloc(1);
		new(pBlock)MType(std::forward<Args>(Arguments)...);
		return pBlock;
	}
	template <typename MType>
	inline MType* MemSys::NewTArr(UInt32 unAlloc) { return GetArena<MType>().Alloc(unAlloc); }
	template<typename MType>
	inline void MemSys::DelT(MType* pBlock) {
		pBlock->~MType();
		GetArena<MType>().Dealloc(pBlock);
	}
	template <typename MType>
	inline void MemSys::DelTArr(MType* pBlock, UInt32 unDealloc) { return GetArena<MType>().Dealloc(pBlock, unDealloc); }
	template<typename MType, typename ... Args>
	inline void* MemSys::NewPlaceT(MType* pBlock, Args&& ... Arguments) { return new(pBlock)MType(std::forward<Args>(Arguments)...); }
	// --==</core_methods>==--
}
#endif // NWG_MEMORY_SYSTEM_H