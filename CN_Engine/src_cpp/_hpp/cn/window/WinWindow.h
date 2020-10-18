#ifndef GL_CWINDOW_H
#define GL_CWINDOW_H

#include <cn/window/cn_Window.h>

namespace CN
{
	/// Window for The "Windows" platform
	class CN_API WinWindow : public Window
	{
	public:
		// Constructor&Destructor
		WinWindow(const WndDescript&);
		virtual ~WinWindow();
		
		// Accessors
		inline UInt getWidth() const override { return m_data.Width; }
		inline UInt getHeight() const override { return m_data.Height; }
		bool isVSync() const override { return m_data.vSync; }
		
		// Modificators
		inline void setEventCallback(const ev_Callback& callback) override
		{ m_data.ev_Callback = callback; }
		void setVSync(bool enabled) override {
			if (enabled) glfwSwapInterval(1);
			else glfwSwapInterval(0);
			m_data.vSync = enabled; }

		// Main functions
		void onUpdate() override;
		void closeWindow() override;
	private:
		/// Initialize glfw if it isn't still set
		/// Create window instance
		/// Initilize glew if it isn't still set
		virtual void init(const CN::WndDescript& description);
		/// Destroy window and release all resources
		virtual void shutDown();
	private:
		GLFWwindow* m_wnd;
		/// Window data for window config and callback function
		struct WndData
		{
			std::string Title;
			UInt Width, Height;
			bool vSync;
			
			ev_Callback ev_Callback;
		};
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