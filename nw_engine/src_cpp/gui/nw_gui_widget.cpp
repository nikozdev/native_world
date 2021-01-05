#include <nw_pch.hpp>
#include "nw_gui_widget.h"

#include <sys/nw_gui_sys.h>

NW::AGuiWidget::IdStack NW::AGuiWidget::s_IdStack = DStack<UInt32>();

namespace NW
{
	AGuiWidget::AGuiWidget() :
		m_unId(0)
	{
		if (s_IdStack.size() == 1) { m_unId = s_IdStack.top()++; }
		else if (s_IdStack.size() > 1) { m_unId = s_IdStack.top(); s_IdStack.pop(); }
		else if (s_IdStack.size() == 0) { s_IdStack.push(m_unId = 0); }
	}
	AGuiWidget::~AGuiWidget()
	{
		if (m_unId != s_IdStack.top()) {
			s_IdStack.push(m_unId);
		}
	}

	// -- Getters
	// -- Setters
}