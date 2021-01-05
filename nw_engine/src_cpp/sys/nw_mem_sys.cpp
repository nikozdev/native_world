#include <nw_pch.hpp>
#include "nw_mem_sys.h"

#include <sys/nw_log_sys.h>

NW::MemInfo NW::MemSys::s_MemInfo = { 0 };

void* operator new(NW::Size szAlloc) { return NW::MemSys::Alloc(szAlloc); }
void* operator new[](NW::Size szAlloc) { return ::operator new(szAlloc); }
void operator delete(void* pBlock) throw() { NW::MemSys::Dealloc(pBlock, sizeof(int)); }
void operator delete(void* pBlock, NW::Size szDealloc) throw() { NW::MemSys::Dealloc(pBlock, szDealloc); }
void operator delete[](void* pBlock) throw() { ::operator delete(pBlock); }
void operator delete[](void* pBlock, NW::Size szDealloc) throw() { ::operator delete(pBlock); }

namespace NW
{
	// ========<Core Methods>========
	void* MemSys::Alloc(Size szAlloc) {
		SetAllocation(szAlloc, 1);
		return malloc(szAlloc);
	}
	void* MemSys::AllocArr(Size szAlloc, UInt32 unAlloc) {
		SetAllocation(szAlloc, unAlloc);
		return malloc(szAlloc);
	}
	void MemSys::Dealloc(void* pBlock, Size szDealloc) {
		SetDeallocation(szDealloc, 1);
		free(pBlock);
	}
	void MemSys::DeallocArr(void* pBlock, Size szDealloc, UInt32 unDealloc) {
		SetDeallocation(szDealloc, unDealloc);
		free(pBlock);
	}
	// ========</Core Methods>========
}