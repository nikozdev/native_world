#include <nw_pch.hpp>
#include "nw_data_res.h"

#include <sys/nw_data_sys.h>

NW::ADataRes::IdStack NW::ADataRes::s_unIdStack = DStack<UInt32>();

namespace NW
{
	// ========<ADataRes>========
	ADataRes::ADataRes(const char* strName) :
		m_unId(0), m_strName(strName)
	{
		if (s_unIdStack.size() == 1) { m_unId = s_unIdStack.top(); s_unIdStack.top()++; }
		else if (s_unIdStack.size() > 1) { m_unId = s_unIdStack.top(); s_unIdStack.pop(); }
		else if (s_unIdStack.size() == 0) { s_unIdStack.push(1); m_unId = s_unIdStack.top(); s_unIdStack.top()++; }
		DataSys::AddADataRes(this);
	}
	ADataRes::ADataRes(const ADataRes& rDataRes) :
		m_unId(rDataRes.m_unId), m_strName(rDataRes.m_strName)
	{
		DataSys::AddADataRes(this);
	}
	ADataRes::~ADataRes()
	{
		if (m_unId != s_unIdStack.top()) {
			s_unIdStack.push(m_unId);
		}
		DataSys::RemoveADataRes(GetId());
	}
	void ADataRes::SetName(const char* strName) {
		m_strName = strName;
	}
	// ========</ADataRes>========
}