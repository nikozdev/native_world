#ifndef EV_EVENT_H
#define EV_EVENT_H

#include <cn_core.hpp>

namespace CN
{
	// Predeclarations
	namespace EV
	{
		class CN_API EvDys;
	}
	// Enums
	namespace EV
	{
		/// Get type of event to define the response
		enum class EvTypes
		{
			None = 0,
			Wnd_Close, Wnd_Resize,
			Wnd_Move,
			Wnd_Focused, Wnd_Defocused,
			App_Tick, App_Update, App_Render,
			Key_Pressed, Key_Released,
			MsBtn_Pressed, MsBtn_Released, Ms_Move, Ms_Scroll
		};
		/// This enumeration is defined by bits
		/// So we can unite categories in one event: Mouse | Input
		enum EvCategories
		{
			None			= BIT_MV(0),
			Application		= BIT_MV(1),
			Input			= BIT_MV(2),
			KeyBoard		= BIT_MV(3),
			Mouse			= BIT_MV(4),
			MouseButton		= BIT_MV(5),
		};
	}
	// Event
	namespace EV
	{
		/// Event interface for any window event
		/// --Has type and category. Categories can blend as flags
		/// --Is destinated for callback functions and event dispatcher
		/// --Inherits need to override getType and getCategoryFlags
		/// --Every possible event inherit is described in the "EvTypes" enumeration
		class CN_API Event
		{
			friend class EvDis;
		public:
			// Accessors
			virtual EvTypes getType() const = 0;
			virtual int getCategoryFlags() const = 0;

			// Functions
			/// Find intersection between own caterogy type and given arg
			inline bool isInCategory(EvCategories category)
			{ return (getCategoryFlags() & category); }
		protected:
			/// Flag for checking whether an event is used or not
			bool m_handled = false;
		};
	}
	// EventDispatcher
	namespace EV
	{
		class CN_API EvDis
		{
			template<typename T>
			/// The function that we call whenever we need to do some stuff with event
			using EventFn = std::function<bool(T&)>;
		public:
			/// Every new event dispatcher takes in an event and dispatches it to check all types
			EvDis(Event& event) : m_event(event) {};
			
			template <typename T>
			bool Dispatch(EventFn<T> function)
			{
				if (m_event.getType() == T::getStaticType())
				{// cast own event to the given template
					m_event.m_handled = function(*(T*)&m_event);
					return true;
				}
				return false;
			};
		private:
			Event& m_event;
		};
	}
}

#endif