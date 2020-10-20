#ifndef CN_WINDOW_H
#define CN_WINDOW_H

#include <cn_pch.hpp>

#include <cn_core.hpp>

namespace CN
{
	namespace EV
	{
		class CN_API Event;
	}
}

// cn_Window class
namespace CN
{
	/// Default data for the window creation
	struct WndDescript
	{
		std::string Title;
		UInt Width;
		UInt Height;
		/// Automatically set title "CheerNik", Width = 1200, Height = 800
		WndDescript(const std::string& title = "CheerNik",
			UInt width = 1200, UInt height = 800) :
			Title(title), Width(width), Height(height) {}
	};

	/// The main interface class of a window class for any platform
	/// --Interface contains: 3 accessors, 2 modifiers(setEventCallback as well)
	/// --"onUpdate" function and createWnd for the new window of the derrived class
	class CN_API Window
	{
	public:
		/// Callback function of this window
		/// It is the name of std::function<void(Event&)>;
		using ev_Callback = std::function<void(EV::Event&)>;
		
		// Virtual destructor for inherits
		virtual ~Window() {};

		// Accessors
		virtual UInt getWidth() const = 0;
		virtual UInt getHeight() const = 0;
		virtual bool isVSync() const = 0;
		// Modificators
		virtual void setEventCallback(const ev_Callback& event) = 0;
		virtual void setVSync(bool enabled) = 0;

		// Main Functions
		/// Call it each frame: PollEvents and SwapBuffers are recomended
		virtual void onUpdate() = 0;
		virtual void closeWindow() = 0;
		/// Create windows of derrived classes for any platform
		static Window* createWnd(const WndDescript& description = WndDescript());
	};
}

#endif