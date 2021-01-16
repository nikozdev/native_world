#include <nw_pch.hpp>
#include "nw_memory.h"

#include <sys/nw_mem_sys.h>

namespace NW
{
	OutStream& MemInfo::operator<<(OutStream& rStream) const {
		return rStream << "MEMORY_INFO::" << std::endl <<
			"Allocated bytes: " << szAlloc << std::endl <<
			"Allocated blocks: " << unAlloc << std::endl;
	}
}

namespace NW
{
	void MemArena::Reset(Size szSpace) {
		if (m_pBegin != nullptr) {
			MemSys::Dealloc(m_pBegin, m_pEnd - m_pBegin);
			m_pBegin = nullptr;
		}
		if (szSpace > 0) {
			m_pBegin = MemSys::NewTArr<Byte>(szSpace);
		}
		m_pCurr = m_pBegin;
		m_pEnd = m_pBegin + szSpace;
		m_FreeList = nullptr;
	}

	void* MemArena::Alloc(Size szAlloc) {
		void* pBlock = nullptr;
		Size szBlock = GetMinBlockSize(szAlloc);
		if (m_FreeList != nullptr) {
			pBlock = m_FreeList;
			m_FreeList = m_FreeList->pNext;
		}
		else {
			m_pCurr = (Byte*)((uintptr_t(m_pCurr) + (m_szAllignment - 1)) & ~(m_szAllignment - 1));
			if (m_pCurr + szBlock < m_pEnd) {
				pBlock = m_pCurr;
				m_pCurr += szBlock;
			}
			else {	// There is not enough space - use the heap
				return MemSys::Alloc(szBlock);
			}
		}
		return pBlock;
	}
	void MemArena::Dealloc(void* pBlock, Size szDealloc)
	{
		Size szBlock = GetMinBlockSize(szDealloc);
		if (pBlock >= m_pBegin && pBlock <= m_pEnd) {
			MemLink* pNextFreeList = static_cast<MemLink*>(pBlock);
			pNextFreeList->pNext = m_FreeList;
			m_FreeList = pNextFreeList;
		}
		else {
			MemSys::Dealloc(pBlock, szDealloc);
		}
	}
	void* MemArena::Realloc(void* pBlock, Size szOld, Size szNew) {
		Size szOldInfo = szOld < szNew ? szOld : szNew;
		void* pNewBlock = Alloc(szNew);
		memcpy(pNewBlock, pBlock, szOldInfo);
		Dealloc(pBlock, szOld);
		return pNewBlock;
	}
}