#include <nwg_pch.hpp>
#include "nwg_mem_sys.h"

NWG::MemInfo NWG::MemSys::s_MemInfo = { 0 };

#if false
void* operator new(Size szAlloc) { NWG::MemSys::SetAllocation(szAlloc, 1); return malloc(szAlloc); }
void* operator new[](Size szAlloc) { NWG::MemSys::SetAllocation(szAlloc, 1); return malloc(szAlloc); }
void operator delete(void* pBlock) throw() { NWG::MemSys::SetDeallocation(1, 1); free(pBlock); }
void operator delete[](void* pBlock) throw() { NWG::MemSys::SetDeallocation(1, 1); free(pBlock); }
void operator delete(void* pBlock, NW::Size szDealloc) throw() { NW::MemSys::SetDeallocation(szDealloc, 1); free(pBlock); }
void operator delete[](void* pBlock, NW::Size szDealloc) throw() { NW::MemSys::SetDeallocation(szDealloc, 1); free(pBlock); }
#endif

namespace NWG
{
	// --==<core_methods>==--
	void MemSys::OnInit(Size szMemory) {
		void* pBlock = malloc(szMemory);
		SetAllocation(szMemory, 1);
		GetMemory() = MemArena(pBlock, szMemory);
	}
	void MemSys::OnQuit() {
		for (auto& itAllocer : GetAllocators()) {
			Dealloc(itAllocer.second->GetDataBeg(), itAllocer.second->GetDataSize());
		}
		SetDeallocation(GetMemory().GetDataSize(), 1);
		free(GetMemory().GetDataBeg());
		GetMemory() = MemArena(nullptr, 0);
	}
	// --==</core_methods>==--
}