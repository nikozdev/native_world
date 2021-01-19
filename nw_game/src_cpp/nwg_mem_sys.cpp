#include <nwg_pch.hpp>
#include "nwg_mem_sys.h"

NWG::MemArena<Byte> NWG::MemSys::s_Memory = NWG::MemArena<Byte>(0);
NWG::MemInfo NWG::MemSys::s_MemInfo = { 0 };
/*
void* operator new(Size szAlloc) { NWG::MemSys::SetAllocation(szAlloc, 1); return malloc(szAlloc); }
void* operator new[](Size szAlloc) { return ::operator new(szAlloc); }
void operator delete(void* pBlock) throw() { NWG::MemSys::SetDeallocation(1, 1); free(pBlock); }
void operator delete(void* pBlock, Size szDealloc) throw() { NWG::MemSys::SetDeallocation(szDealloc, 1); free(pBlock); }
void operator delete[](void* pBlock) throw() { ::operator delete(pBlock); }
void operator delete[](void* pBlock, Size szDealloc) throw() { ::operator delete(pBlock, szDealloc); }
*/
namespace NWG
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