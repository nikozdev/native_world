#include <nw_pch.hpp>
#include "nw_ent_sys.h"

namespace NW
{
	AEntity::AEntity(EntId eId) :
		m_eId(eId), m_bIsEnabled(true) { }
	AEntity::AEntity(const AEntity& rCpy) : AEntity(rCpy.m_eId) { }
	AEntity::~AEntity()
	{
		std::cout << "ent_" << m_eId << " has been destroyed" << std::endl;
	}
}

namespace NW
{
	void CmpSys::OnInit(Size szMem)
	{
		GetMemory() = MemArena(CoreEngine::Get().GetMemory().Alloc(szMem), szMem);
	}
	void CmpSys::OnQuit()
	{
		CoreEngine::Get().GetMemory().Dealloc(GetMemory().GetDataBeg(), GetMemory().GetDataSize());
		GetMemory() = MemArena(nullptr, 0);
	}
}

namespace NW
{
	// --core_methods
	void EntSys::OnInit(Size szMem)
	{
		GetMemory() = MemArena(CoreEngine::Get().GetMemory().Alloc(szMem), szMem);
	}
	void EntSys::OnQuit()
	{
		CoreEngine::Get().GetMemory().Dealloc(GetMemory().GetDataBeg(), GetMemory().GetDataSize());
		GetMemory() = MemArena(nullptr, 0);
	}
}