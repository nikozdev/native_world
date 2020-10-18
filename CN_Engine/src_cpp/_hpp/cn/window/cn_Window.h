#ifndef CN_WINDOW_H
#define CN_WINDOW_H

#include <string>
#include <functional>

#include <cn_core.hpp>
#include <events/ev_EvSys.h>

// cn_Window class
namespace CN
{
	/// Data for the window creation
	/// Has Title, Hidth and Height set by default
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
	class CN_API Window
	{
	public:
		/// Callback function of this window
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