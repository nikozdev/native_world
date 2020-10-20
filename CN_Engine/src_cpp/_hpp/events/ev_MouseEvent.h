#ifndef EV_MOUSE_EVENT_H
#define EV_MOUSE_EVENT_H

#include "ev_Event.h"

namespace CN
{
	// MouseEvents
	namespace EV
	{
		/// MousePosEvent class
		/// --Has x and y coordinates
		class CN_API MouseMoveEvent : public Event
		{
		public:
			// Constructor
			MouseMoveEvent(double posX, double posY) :
				m_posX(posX), m_posY(posY) {}

			// Accessors
			inline double getX() const { return m_posX; };
			inline double getY() const { return m_posY; };
		public: // Inherited functions
			static EvTypes getStaticType() { return EvTypes::Ms_Move; }
			virtual EvTypes getType() const override { return getStaticType(); }

			virtual int getCategoryFlags() const override
			{
				return EvCategories::Mouse | EvCategories::Input;
			}
		private:
			double m_posX, m_posY;
		};
		/// MousePosEvent class
		/// --Has x and y coordinates of the mouse offset
		class CN_API MouseScrollEvent : public Event
		{
		public:
			MouseScrollEvent(double offsetX, double offsetY) :
				m_offsetX(offsetX), m_offsetY(offsetY) {}
		
			// Accessors
			inline double getOffsetX() const { return m_offsetX; };
			inline double getOffsetY() const { return m_offsetY; };
		public: // Inherited functions
			static EvTypes getStaticType() { return EvTypes::Ms_Scroll; }
			virtual EvTypes getType() const override { return getStaticType(); }

			virtual int getCategoryFlags() const override
			{
				return EvCategories::Mouse | EvCategories::Input;
			}
		private:
			double m_offsetX, m_offsetY;
		};
		/// MouseButtonEvent interface
		/// --Has the keycode of mouse button
		class CN_API MouseButtonEvent : public Event
		{
		public: // Inherited functions
			virtual int getCategoryFlags() const override
			{
				return EvCategories::Mouse | EvCategories::Input;
			}
		protected:
			MouseButtonEvent(int msButton) :
				m_Button(msButton) {}
			
			int m_Button;
		};
		/// MousePressedEvent class
		/// --Inherit of MouseButtonEvent. Has button keycode
		class CN_API MousePressedEvent : public MouseButtonEvent
		{
		public:
			MousePressedEvent(int msButton) :
				MouseButtonEvent(msButton) {}
		public: // Inherited functions
			static EvTypes getStaticType() { return EvTypes::MsBtn_Pressed; }
			virtual EvTypes getType() const override { return getStaticType(); }

			virtual int getCategoryFlags() const override
			{
				return EvCategories::Mouse | EvCategories::Input;
			}
		};
		/// MouseReleasedEvent class
		/// --Inherit of MouseButtonEvent. Has button keycode
		class CN_API MouseReleasedEvent : public MouseButtonEvent
		{
		public:
			MouseReleasedEvent(int msButton) :
				MouseButtonEvent(msButton) {}
		public: // Inherited functions
			static EvTypes getStaticType() { return EvTypes::MsBtn_Released; }
			virtual EvTypes getType() const override { return getStaticType(); }

			virtual int getCategoryFlags() const override
			{
				return EvCategories::Mouse | EvCategories::Input;
			}
		};
	}
}

#endif