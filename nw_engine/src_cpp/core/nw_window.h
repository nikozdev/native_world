#ifndef NW_AWINDOW_H
#define NW_AWINDOW_H

#include <nw_decl.hpp>

#if (defined NW_WINDOW)
namespace NW
{
	using EventCallback = std::function<void(AEvent&)>;
	struct NW_API WindowInfo
	{
		String strTitle = "none";
		String strApiVer = "none";
		UInt16 unWidth = 800, unHeight = 600;
		Float32 nAspectRatio = 800.0f / 600.0f;
		Float32 nOpacity = 0.0f;
		Bit bVSync = false;

		WApiTypes wapiType = WAPI_NONE;

		EventCallback fnEvCallback = nullptr;
	public:
		WindowInfo(const char* cstTitle = "native_window",
			UInt16 unWidth = 800, UInt16 unHeight = 600,
			bool bVerticalSync = false,
			std::function<void(AEvent&)> fnEventCallback = nullptr) :
			strTitle(cstTitle), unWidth(unWidth), unHeight(unHeight),
			bVSync(bVerticalSync), fnEvCallback(fnEventCallback),
		nAspectRatio((Float32)unWidth / (Float32)unHeight) { }
		inline OutStream& operator<<(OutStream& rStream) {
			rStream << "WINDOW_INFO" << std::endl <<
				"title: " << &strTitle[0] << std::endl <<
				"width x height: " << unWidth << " x " << unHeight << std::endl <<
				"api_version: " << &strApiVer[0] << std::endl;
			return rStream;
		}
	};
	inline OutStream& operator<<(OutStream& rStream, WindowInfo& rwInfo) { return rwInfo.operator<<(rStream); }
	/// Abstract Window Class
	/// Interface:
	/// -> Make Create method for a particular implementation
	/// -> Create the window by that function
	/// -> Call OnUpdate() and OnRender every frame
	class NW_API AWindow
	{
	public:
		AWindow(const WindowInfo& rwInfo);
		virtual ~AWindow() = default;

		// --getters
		inline UInt16 GetWidth() const { return m_Info.unWidth; }
		inline UInt16 GetHeight() const { return m_Info.unHeight; }
		inline const WindowInfo& GetWindowInfo() const { return m_Info; }
		
		virtual Ptr GetNative() = 0;
		// --setters
		virtual void SetTitle(const char* strTitle) = 0;
		virtual void SetEventCallback(const EventCallback& fnEvCallback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual void SetIcon(UByte* pData, UInt16 unWidth, UInt16 unHeight) = 0;
		virtual void SetOpacity(float nOpacity) = 0;
		// --predicates
		inline bool IsVSync() const { return m_Info.bVSync; }

		// --core_methods
		virtual bool Init() = 0;
		virtual void OnQuit() = 0;
		virtual void Update() = 0;

		static AWindow* Create(const WindowInfo& rWindowInfo);
		static void Create(const WindowInfo& rWindowInfo, RefKeeper<AWindow>& rWindow);
	protected:
		WindowInfo m_Info;
	};
}
#endif // NW_WINDOW
#if (NW_WINDOW & NW_WINDOW_GLFW)
struct GLFWwindow;
struct GLFWimage;
namespace NW
{
	/// WindowOgl class
	/// --Works with GLFW
	/// --Can interact mostly by events but not with the application directly
	class NW_API WindowOgl : public AWindow
	{
	public:
		WindowOgl(const WindowInfo& rwInfo);
		virtual ~WindowOgl();

		// --getters
		virtual inline Ptr GetNative() override { return m_pNative; }
		// --setters
		virtual void SetTitle(const char* strTitle) override;
		virtual void SetEventCallback(const EventCallback& fnEvCallback) override;
		virtual void SetVSync(bool enabled) override;
		virtual void SetIcon(UByte* pData, UInt16 unWidth, UInt16 unHeight) override;
		virtual void SetOpacity(float nOpacity) override;
		
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