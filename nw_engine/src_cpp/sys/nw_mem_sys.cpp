#include <nw_pch.hpp>
#include "nw_mem_sys.h"

NW::MemArena<NW::Byte> NW::MemSys::s_Memory = NW::MemArena<Byte>(0);
NW::MemInfo NW::MemSys::s_MemInfo = { 0 };

#if true
void* operator new(NW::Size szAlloc)								{ NW::MemSys::SetAllocation(szAlloc, 1); return malloc(szAlloc); }
void* operator new[](NW::Size szAlloc)								{ return ::operator new(szAlloc); }
void operator delete(void* pBlock) throw()							{ NW::MemSys::SetDeallocation(1, 1); free(pBlock); }
void operator delete[](void* pBlock) throw()						{ NW::MemSys::SetDeallocation(1, 1); free(pBlock); }
void operator delete(void* pBlock, NW::Size szDealloc) throw()		{ NW::MemSys::SetDeallocation(szDealloc, 1); free(pBlock); }
void operator delete[](void* pBlock, NW::Size szDealloc) throw()	{ NW::MemSys::SetDeallocation(szDealloc, 1); free(pBlock); }
#else
void* operator new(NW::Size szAlloc)								{ return NW::MemSys::Alloc(szAlloc); }
void* operator new[](NW::Size szAlloc)								{ return NW::MemSys::Alloc(szAlloc); }
void operator delete(void* pBlock) throw()							{ NW::MemSys::Dealloc(pBlock, sizeof(int)); }
void operator delete[](void* pBlock) throw()						{ NW::MemSys::Dealloc(pBlock, sizeof(int)); }
void operator delete(void* pBlock, NW::Size szDealloc) throw()		{ NW::MemSys::Dealloc(pBlock, szDealloc); }
void operator delete[](void* pBlock, NW::Size szDealloc) throw()	{ NW::MemSys::Dealloc(pBlock, szDealloc); }
#endif

namespace NW
{
	// --==<core_methods>==--
	void MemSys::OnInit(Size szMemory) {
		MemSys::s_Memory.Reset(szMemory);
	}
	void MemSys::OnQuit() {
		s_Memory.Reset(0);
	}
	// --==</core_methods>==--
}