#ifndef NW_AWINDOW_H
#define NW_AWINDOW_H

#include <glib/gcontext/nw_gcontext.h>

#include <nw_decl.hpp>
#include <nw_glib_decl.hpp>

#if (defined NW_WINDOW)
namespace NW
{
	using EventCallback = std::function<void(AEvent&)>;
	struct NW_API WindowInfo
	{
		String strTitle = "window";
		UInt16 unWidth = 800, unHeight = 600;
		Float32 nAspectRatio = 800.0f / 600.0f;
		bool bVSync = false;
		float nOpacity = 0.0f;

		WApiTypes WApiType = WAPI_NONE;

		EventCallback fnEvCallback = nullptr;

		WindowInfo(const char* cstTitle = "native_window",
			UInt16 unWidth = 800, UInt16 unHeight = 600,
			bool bVerticalSync = false,
			std::function<void(AEvent&)> fnEventCallback = nullptr) :
			strTitle(cstTitle), unWidth(unWidth), unHeight(unHeight),
			bVSync(bVerticalSync), fnEvCallback(fnEventCallback),
		nAspectRatio((Float32)unWidth / (Float32)unHeight) { }
	};
	/// Abstract Window Class
	/// Interface:
	/// -> Make Create method for a particular implementation
	/// -> Create the window by that function
	/// -> Call OnUpdate() and OnRender every frame
	class NW_API AWindow
	{
	public:
		virtual ~AWindow() = default;

		// --getters
		virtual UInt16 GetWidth() const = 0;
		virtual UInt16 GetHeight() const = 0;
		virtual void* GetNative() = 0;
		virtual const WindowInfo& GetWindowInfo() = 0;
		virtual AGContext* GetGContext() = 0;
		// --setters
		virtual void SetTitle(const char* strTitle) = 0;
		virtual void SetEventCallback(const EventCallback& fnEvCallback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual void SetIcon(UByte* pData, UInt16 unWidth, UInt16 unHeight) = 0;
		virtual void SetOpacity(float nOpacity) = 0;
		// --predicates
		virtual bool IsVSync() const = 0;

		// --core_methods
		virtual bool Init() = 0;
		virtual void OnQuit() = 0;
		virtual void Update() = 0;

		static AWindow* Create(const WindowInfo& rWindowInfo, WApiTypes WindowType);
	protected:
		AWindow() = default;
	};
}
#endif // NW_WINDOW
#if (NW_WINDOW & NW_WINDOW_GLFW)
namespace NW
{
	/// WindowOgl class
	/// --Works with GLFW
	/// --Can interact mostly by events but not with the application directly
	class NW_API WindowOgl : public AWindow
	{
	public:
		WindowOgl(const WindowInfo& rWindowInfo);
		virtual ~WindowOgl();

		// --getters
		virtual inline UInt16 GetWidth() const override { return m_WindowInfo.unWidth; }
		virtual inline UInt16 GetHeight() const override { return m_WindowInfo.unHeight; }
		virtual inline const WindowInfo& GetWindowInfo() override { return m_WindowInfo; }
		virtual inline void* GetNative() override { return m_pNative; }
		virtual inline AGContext* GetGContext() override { return m_pGContext; }
		// --setters
		virtual void SetTitle(const char* strTitle) override;
		virtual void SetEventCallback(const EventCallback& fnEvCallback) override;
		virtual void SetVSync(bool enabled) override;
		virtual void SetIcon(UByte* pData, UInt16 unWidth, UInt16 unHeight) override;
		virtual void SetOpacity(float nOpacity) override;
		// --predicates
		virtual inline bool IsVSync() const override { return m_WindowInfo.bVSync; }

		// --core_methods
		virtual bool Init() override;
		virtual void OnQuit() override;
		virtual void Update() override;
	private:
		// --input_callbacks: mouse
		static void CbMouseCoord(GLFWwindow* pWindow, double xCrd, double yCrd);
		static void CbMouseScroll(GLFWwindow* pWindow, double xScroll, double yScroll);
		static void CbMouseButton(GLFWwindow* pWindow, Int32 nButton, Int32 nAction, Int32 nMode);
		// --input_callbacks: keyboard
		static void CbKeyboard(GLFWwindow* pWindow, Int32 nKey, Int32 nScanCode, Int32 nAction, Int32 nMode);
		static void CbKeyboardChar(GLFWwindow* pWindow, UInt32 unChar);
		// --input_callbacks: window
		static void CbWindowClose(GLFWwindow* pWindow);
		static void CbWindowSize(GLFWwindow* pWindow, Int32 nWidth, Int32 nHeight);
		static void CbWindowFocus(GLFWwindow* pWindow, Int32 nFocus);
		// --other_callbacks: window
		static void CbError(Int32 nErrCode, const char* strErrMsg);
	private:
		GContextOgl* m_pGContext;
		WindowInfo m_WindowInfo;
		GLFWwindow* m_pNative;
		GLFWimage* m_pIcon;
	};
}
#endif // NW_WINDOW
#endif	// NW_AWINDOW_H
/*
* Development started 18.10.2020
* --Tutorials used:
* -->Cherno GameEngine series
* -->MihailRis - Minecraft Voxel Engine
*/