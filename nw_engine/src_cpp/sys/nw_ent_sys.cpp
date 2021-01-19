#include <nw_pch.hpp>
#include "nw_ent_sys.h"

NW::IdStack NW::EntSys::s_IdStack = IdStack();

namespace NW
{
	// --core_methods
	bool EntSys::OnInit()
	{
		return true;
	}
	void EntSys::OnQuit()
	{
		//
	}
}