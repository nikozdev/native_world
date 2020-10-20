#ifndef GL_CWINDOW_H
#define GL_CWINDOW_H

#include <cn/window/cn_Window.h>

#include <gl/gl_declarations.hpp>

#pragma comment(lib, "opengl32.lib")

namespace CN
{
	/// Window for The "Windows" platform
	/// --Works with GLFW
	/// --Can interact mostly by events but not with the application directly
	class CN_API WinWindow : public Window
	{
	public:
		// Constructor&Destructor
		/// Has default window description
		WinWindow(const WndDescript& description);
		virtual ~WinWindow();
		
		// Accessors
		inline UInt getWidth() const override { return m_data.Width; }
		inline UInt getHeight() const override { return m_data.Height; }
		bool isVSync() const override { return m_data.vSync; }
		
		// Modificators
		inline void setEventCallback(const ev_Callback& callback) override
		{ m_data.ev_Callback = callback; }

		void setVSync(bool enabled) override;

		// Main functions
		void onUpdate() override;
		void closeWindow() override;
	public: // Members and struct
		/// Special structure for "windows window"
		/// Stores default window data event function for processing all events
		struct WndData
		{
			std::string Title;
			UInt Width, Height;
			bool vSync;

			ev_Callback ev_Callback;
		};
	private: // Implementation functions
		/// Initialize glfw if it isn't still set
		/// Create window instance
		/// Initilize GL functions if ther aren't still set
		virtual void init(const CN::WndDescript& description);
		/// Destroy window and release all resources
		virtual void shutDown();
	private:
		GLFWwindow* m_wnd;
		/// Window data can be called by GLFW
		WndData m_data;
	};
}

#endif

/*
* Development started 18.10.2020
* --Tutorials used:
* -->Cherno GameEngine series
* -->MihailRis - Minecraft Voxel Engine
*/