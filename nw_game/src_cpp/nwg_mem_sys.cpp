#include <nwg_pch.hpp>
#include "nwg_mem_sys.h"

MemInfo NWG::MemSys::s_MemInfo = { 0 };

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