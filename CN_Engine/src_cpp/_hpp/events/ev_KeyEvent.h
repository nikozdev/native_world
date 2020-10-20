#ifndef EV_KET_EVENT_H
#define EV_KET_EVENT_H

#include "ev_Event.h"

namespace CN
{
	// KeyEvent
	namespace EV
	{
		/// KeyEvent Interface
		/// --KeyEvents are similar, and they all have keycode
		/// --This abstraction makes derrived classes to have a keycode and define it
		/// --Here are also defined category "Input"
		class CN_API KeyEvent : public Event
		{
		public:
			inline UInt getKeyCode() { return m_keyCode; }
			
			virtual int getCategoryFlags() const override
			{
				return EvCategories::KeyBoard | EvCategories::Input;
			}
		protected:
			KeyEvent(int keyCode) :
				m_keyCode(keyCode) {}
			UInt m_keyCode;
		};
		/// KeyPressedEvent class
		/// --Has keyCode member of KeyEvent interface
		/// --Here is also the member "m_repeatCount"
		class CN_API KeyPressedEvent : public KeyEvent
		{
		private:
			int m_repeatCount;
		public:
			KeyPressedEvent(int keyCode, int repeatCount) :
				KeyEvent(keyCode), m_repeatCount(repeatCount) {}

			inline int getRepeat() { return m_repeatCount; }

			static EvTypes getStaticType() { return EvTypes::Key_Pressed; }
			virtual EvTypes getType() const override { return getStaticType(); }
		};
		/// KeyReleasedEvent class
		/// --Has keyCode member of KeyEvent interface
		class CN_API KeyReleasedEvent : public KeyEvent
		{
		private:
		public:
			KeyReleasedEvent(int keyCode) :
				KeyEvent(keyCode) {}

			static EvTypes getStaticType() { return EvTypes::Key_Released; }
			virtual EvTypes getType() const override { return getStaticType(); }
		};
	}
}

#endif