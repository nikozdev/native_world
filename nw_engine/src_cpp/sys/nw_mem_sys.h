#ifndef NW_MEMORY_SYSTEM_H
#define NW_MEMORY_SYSTEM_H
#include <nwlib/nw_memory.h>

namespace NW
{
	/// MemorySystem static class
	/// -- Manipulates memory allocation and deallocation
	/// -- Counts bytes and blocks - prints memory usage info
	/// -- Has static function for memory arena allocator
	class NW_API MemSys
	{
	public:
		static MemInfo s_MemInfo;
	public:
		// --getters
		static inline const MemInfo& GetInfo() { return s_MemInfo; }
		// --setters
		static inline const void SetAllocation(Size szAlloc, UInt32 unAlloc) {
			s_MemInfo.szAlloc += szAlloc; s_MemInfo.unAlloc += unAlloc;
		}
		static inline const void SetDeallocation(Size szAlloc, UInt32 unAlloc) {
			s_MemInfo.szAlloc -= szAlloc; s_MemInfo.unAlloc -= unAlloc;
		}

		// --core_methods
		static void* Alloc(Size szAlloc);
		static void* AllocArr(Size szAlloc, UInt32 unAlloc);
		static void Dealloc(void* pBlock, Size szDealloc);
		static void DeallocArr(void* pBlock, Size szDealloc, UInt32 unDealloc);

		template<typename MemType, typename ... Args>
		static inline MemType* NewT(Args& ... Arguments) {
			MemType* pBlock = reinterpret_cast<MemType*>(Alloc(sizeof(MemType)));
			new (pBlock) MemType(std::forward<Args>(Arguments)...);
			return pBlock;
		}
		template <typename MemType>
		static inline MemType* NewTArr(UInt32 unAlloc) {
			return new MemType[unAlloc];
		}
		template<typename MemType>
		static inline void DelT(MemType* pBlock) {
			pBlock->~MemType();
			Dealloc(pBlock, sizeof(MemType));
		}
		template <typename MemType>
		static inline void DelTArr(MemType* pBlock, UInt32 unDealloc) {
			return delete[unDealloc](pBlock);
		}
		
		template<typename MemType, typename ... Args>
		static inline void* NewPlaceT(MemType* pBlock, Args&& ... Arguments) {
			pBlock->MemType(std::forward<Args>(Arguments)...);
		}

		/// -- Allocate new the new block of memory with new size
		/// -- Copy old information into the new block and Deallocate old block
		static inline void* Realloc(void* pBlock, Size szOld, Size szNew) {
			Size szOldCpy = szNew > szOld ? szOld : szNew;
			void* pNewBlock = Alloc(szNew);
			memcpy(pNewBlock, pBlock, szOldCpy);
			Dealloc(pBlock, szOld);
			return pNewBlock;
		}
	};
	// inline void* operator new(Size szAlloc) { MemSys::SetAllocation(szAlloc, 1); return malloc(szAlloc); }
	// inline void operator delete(void* pBlock, Size szDealloc) { MemSys::SetDeallocation(szDealloc, 1); free(pBlock); }
}
#endif // NW_MEMORY_SYSTEM_H