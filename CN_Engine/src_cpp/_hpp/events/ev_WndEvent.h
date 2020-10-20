#ifndef EV_WINDOW_EVENT_H
#define EV_WINDOW_EVENT_H

#include "ev_Event.h"

namespace CN
{
	// WindowEvent
	namespace EV
	{
		
		/// WndEvent interface for events of windowEv type
		/// --Has width and height of the window
		class CN_API WndEvent : public Event
		{
		public:
			inline UInt getWidth() const { return m_width; }
			inline UInt getHeight() const { return m_height; }
		protected:
			WndEvent(UInt width, UInt height) :
				m_width(width), m_height(height) {}
			UInt m_width, m_height;
		};
		/// WndCloseEvent class
		class WndCloseEvent : public WndEvent
		{
		public:
			WndCloseEvent() :
				WndEvent(0, 0) {}

			static EvTypes getStaticType() { return EvTypes::Wnd_Close; }
			virtual EvTypes getType() const override { return getStaticType(); }
			virtual int getCategoryFlags() const override
			{
				return EvCategories::Application;
			}
		};
		/// WndResizeEvent class
		class WndResizeEvent : public WndEvent
		{
		public:
			WndResizeEvent(int width, int height) :
				WndEvent(width, height) {}

			static EvTypes getStaticType() { return EvTypes::Wnd_Resize; }
			virtual EvTypes getType() const override { return getStaticType(); }
			virtual int getCategoryFlags() const override
			{
				return EvCategories::Application;
			}
		};
		/// WndFocusEvent class
		class WndFocusEvent : public WndEvent
		{
		public:
			WndFocusEvent() :
				WndEvent(0, 0) {}

			static EvTypes getStaticType() { return EvTypes::Wnd_Focused; }
			virtual EvTypes getType() const override { return getStaticType(); }
			virtual int getCategoryFlags() const override
			{
				return EvCategories::Application;
			}
		};
		/// WndDefocusEvent class
		class WndDefocusEvent : public WndEvent
		{
		public:
			WndDefocusEvent() :
				WndEvent(0, 0) {}

			static EvTypes getStaticType() { return EvTypes::Wnd_Defocused; }
			virtual EvTypes getType() const override { return getStaticType(); }
			virtual int getCategoryFlags() const override
			{
				return EvCategories::Application;
			}
		};
	}
}

#endif