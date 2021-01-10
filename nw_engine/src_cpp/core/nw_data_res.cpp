#include <nw_pch.hpp>
#include "nw_data_res.h"

#include <sys/nw_data_sys.h>

NW::IdStack NW::ADataRes::s_IdStack = IdStack();

namespace NW
{
	// --==<ADataRes>==--
	ADataRes::ADataRes(const char* strName) :
		m_unId(0), m_strName(strName)
	{
		m_unId = s_IdStack.GetFreeId();
		DataSys::AddADataRes(this);
	}
	ADataRes::ADataRes(const char* strName, UInt32 unId) :
		m_unId(unId), m_strName(strName)
	{
		DataSys::AddADataRes(this);
	}
	ADataRes::ADataRes(const ADataRes& rDataRes) :
		m_unId(rDataRes.m_unId), m_strName(rDataRes.m_strName) { DataSys::AddADataRes(this); }
	ADataRes::~ADataRes()
	{
		s_IdStack.SetFreeId(m_unId);
		DataSys::RemoveADataRes(GetId());
	}
	void ADataRes::SetName(const char* strName) {
		m_strName = strName;
	}
	// --==</ADataRes>==--
}