#ifndef NW_MEMORY_H
#define NW_MEMORY_H

#include <nw_core.hpp>

namespace NW
{
	inline static const Size s_szMaxMemorySize = 1 << 30;
}
namespace NW
{
	/// MemInfo struct
	struct NW_API MemInfo
	{
	public:
		Size szAlloc = 0;
		UInt32 unAlloc = 0;
	public:
		OutStream& operator<<(OutStream& rStream) const;
	};
	inline OutStream& MemInfo::operator<<(OutStream& rStream) const {
		return rStream << "MEMORY_INFO::" << std::endl <<
			"location: " << std::hex << reinterpret_cast<uintptr_t>(this) << std::dec << std::endl <<
			"allocated_bytes: " << szAlloc << std::endl <<
			"allocated_blocks: " << unAlloc << std::endl;
	}
	inline OutStream& operator<<(OutStream& rStream, const MemInfo& rInfo) { return rInfo.operator<<(rStream); }
	/// MemLink struct
	struct MemLink { MemLink* pNext = nullptr; };
}
namespace NW
{
	template <typename MType>
	class NW_API MemArena
	{
	public:
		MemArena(UInt32 unBlocks) :
			m_pBegin(nullptr), m_pEnd(nullptr),
			m_pCurr(nullptr),
			m_FreeList(nullptr),
			m_szBlocks(unBlocks * sizeof(MType)), m_unBlocks(unBlocks),
			m_bIsDynamic(true) { Reset(unBlocks); }
		MemArena(MType* pBlock, UInt32 unBlocks) :
			m_pBegin(pBlock), m_pEnd(nullptr),
			m_pCurr(nullptr),
			m_FreeList(nullptr),
			m_szBlocks(unBlocks * sizeof(MType)), m_unBlocks(unBlocks),
			m_bIsDynamic(false) { Reset(pBlock, unBlocks); }
		~MemArena() { Reset(0); }

		// --getters
		inline MType* GetData() { return m_pBegin; }
		inline UInt32 GetCount() const { return m_unBlocks; }
		inline Size GetSize() const { return m_szBlocks; }
		inline Size GetBlockSize() const { return sizeof(MType); }
		// --setters
		inline void Reset(UInt32 unBlocks);
		inline void Reset(MType* pBlock, UInt32 unBlocks);
		// --predicates
		inline bool IsDynamic() const { return m_bIsDynamic; }
		inline bool HasSpace(UInt32 unHowMuch) const { return ((uintptr_t)(m_pEnd - m_pCurr)) > unHowMuch; }

		// --core_methods
		inline MType* Alloc(UInt32 unBlocks = 1);
		inline void Dealloc(MType* pBlock, UInt32 unDealloc = 1);
		inline MType* Realloc(MType* pBlock, UInt32 unOld, UInt32 unNew);
	private:
		MType *m_pCurr, *m_pBegin, *m_pEnd;
		MemLink* m_FreeList;

		Size m_szBlocks;
		UInt32 m_unBlocks;
		bool m_bIsDynamic;
	};
	// -- setters
	template<typename MType>
	inline void MemArena<MType>::Reset(UInt32 unBlocks) {
		m_unBlocks = unBlocks;
		m_szBlocks = unBlocks * sizeof(MType);
		m_FreeList = nullptr;
		if (m_bIsDynamic) { if (m_pBegin != nullptr) { delete[] m_pBegin; m_pBegin = nullptr; } m_bIsDynamic = false; }
		if (unBlocks <= 1 || m_szBlocks > s_szMaxMemorySize) { m_szBlocks = m_unBlocks = 0; return; }
		m_pCurr = m_pBegin = reinterpret_cast<MType*>(malloc(unBlocks * sizeof(MType)));
		m_bIsDynamic = true;
		m_pEnd = m_pBegin + unBlocks;
	}
	template<typename MType>
	inline void MemArena<MType>::Reset(MType* pBlock, UInt32 unBlocks) {
		m_unBlocks = unBlocks;
		m_szBlocks = unBlocks * sizeof(MType);
		m_FreeList = nullptr;
		if (m_bIsDynamic) { if (m_pBegin != nullptr) { delete[] m_pBegin; m_pBegin = nullptr; } m_bIsDynamic = false; }
		if (unBlocks <= 1 || m_szBlocks > s_szMaxMemorySize) { m_szBlocks = m_unBlocks = 0; return; }
		m_pCurr = m_pBegin = pBlock;
		m_pEnd = m_pBegin + unBlocks;
	}
	// --==<core_methods>==--
	template<typename MType>
	inline MType* MemArena<MType>::Alloc(UInt32 unBlocks) {
		MType* pBlock = nullptr;
		if (unBlocks == 0) { return nullptr; }
		if (m_FreeList != nullptr) {
			pBlock = reinterpret_cast<MType*>(&m_FreeList);
			m_FreeList = m_FreeList->pNext;
		}
		else {
			m_pCurr = reinterpret_cast<MType*>((uintptr_t(m_pCurr) + (alignof(MType) - 1)) & ~(alignof(MType) - 1));
			if (m_pCurr + unBlocks < m_pEnd) {
				pBlock = m_pCurr;
				m_pCurr += unBlocks;
			}
			else { pBlock = reinterpret_cast<MType*>(malloc(sizeof(MType) * unBlocks)); }
		}
		return pBlock;
	}
	template<typename MType>
	inline void MemArena<MType>::Dealloc(MType* pBlock, UInt32 unDealloc) {
		if (pBlock >= m_pBegin && pBlock <= m_pEnd) {	// somewhere inside our memory
			MemLink* pNextFreeList = reinterpret_cast<MemLink*>(pBlock);
			pNextFreeList->pNext = m_FreeList;
			m_FreeList = pNextFreeList;
		}
		else { return; }
	}
	template<typename MType>
	inline MType* MemArena<MType>::Realloc(MType* pBlock, UInt32 unOld, UInt32 unNew) {
		UInt32 unCpy = unOld < unNew ? unOld : unNew;
		MType* pRealloc = Alloc(unNew);
		memcpy(pRealloc, pBlock, unCpy);
		Dealloc(pBlock, unOld);
		return pRealloc;
	}
	// --==</core_methods>==--
}
#endif // NW_MEMORY_H