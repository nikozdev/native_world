#ifndef NW_MEMORY_SYSTEM_H
#define NW_MEMORY_SYSTEM_H
#include <nwlib/nw_memory.h>

namespace NW
{
	class NW_API MemSys
	{
	public:
		// --getters
		static inline const MemInfo& GetInfo() { return s_MemInfo; }
		//template <typename MType>
		//static inline MemArena<MType>& GetArena() { static MemArena<MType> s_Arena(reinterpret_cast<MType*>(s_Memory.Alloc(1 << 18)), 1 << 10); return s_Arena; }
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
	inline const void MemSys::SetAllocation(Size szAlloc, UInt32 unAlloc) { s_MemInfo.szAlloc += szAlloc; s_MemInfo.unAlloc += unAlloc; }
	inline const void MemSys::SetDeallocation(Size szAlloc, UInt32 unAlloc) { s_MemInfo.szAlloc -= szAlloc; s_MemInfo.unAlloc -= unAlloc; }
	// --==<core_methods>==--
	inline void* MemSys::Alloc(Size szAlloc) { return s_Memory.Alloc(szAlloc); }
	inline void* MemSys::AllocArr(Size szAlloc, UInt32 unAlloc) { return s_Memory.Alloc(szAlloc); }
	inline void MemSys::Dealloc(void* pBlock, Size szDealloc) { s_Memory.Dealloc(static_cast<Byte*>(pBlock), szDealloc); }
	inline void MemSys::DeallocArr(void* pBlock, Size szDealloc, UInt32 unDealloc) { s_Memory.Dealloc(static_cast<Byte*>(pBlock), szDealloc); }
	inline void* MemSys::Realloc(void* pBlock, Size szOld, Size szNew) { return s_Memory.Realloc(static_cast<Byte*>(pBlock), szOld, szNew); }
	// --templated
	template<typename MType, typename ... Args>
	inline MType* MemSys::NewT(Args& ... Arguments) {
		//MType* pBlock = GetArena<MType>().Alloc(1);
		MType* pBlock = reinterpret_cast<MType*>(s_Memory.Alloc(sizeof(MType) * 1));
		new(pBlock)MType(std::forward<Args>(Arguments)...);
		return pBlock;
	}
	template <typename MType>
	inline MType* MemSys::NewTArr(UInt32 unAlloc) {
		//return GetArena<MType>().Alloc(unAlloc);
		return reinterpret_cast<MType*>(s_Memory.Alloc(sizeof(MType) * unAlloc));
	}
	template<typename MType>
	inline void MemSys::DelT(MType* pBlock) {
		pBlock->~MType();
		//GetArena<MType>().Dealloc(pBlock);
		s_Memory.Dealloc(reinterpret_cast<Byte*>(pBlock), sizeof(MType) * 1);
	}
	template <typename MType>
	inline void MemSys::DelTArr(MType* pBlock, UInt32 unDealloc) {
		//GetArena<MType>().Dealloc(pBlock, unDealloc);
		s_Memory.Dealloc(reinterpret_cast<Byte*>(pBlock), sizeof(MType) * unDealloc);
	}
	template<typename MType, typename ... Args>
	inline void* MemSys::NewPlaceT(MType* pBlock, Args&& ... Arguments) { return new(pBlock)MType(std::forward<Args>(Arguments)...); }
	// --==</core_methods>==--
}
#endif // NW_MEMORY_SYSTEM_H