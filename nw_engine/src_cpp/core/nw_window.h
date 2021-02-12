#ifndef NW_CORE_WINDOW_H
#define NW_CORE_WINDOW_H
#include <nw_core.hpp>
#if (defined NW_WAPI)
namespace NW
{
	using EventCallback = std::function<void(AEvent&)>;
	struct NW_API WindowInfo
	{
	public:
		String strTitle = "none";
		String strApiVer = "none";
		UInt16 nX = 0, nY = 0, nW = 800, nH = 600;
		Float32 nAspectRatio = 800.0f / 600.0f;
		Float32 nOpacity = 0.0f;
		Bit bVSync = false;
		Bit bIsHovered = false;
		Bit bIsFocused = false;
		Bit bIsEnabled = false;
		EventCallback fnOnEvent = [](AEvent&)->void {return; };
	public:
		WindowInfo(const char* cstTitle = "native_window",
			UInt16 unWidth = 800, UInt16 unHeight = 600,
			Bit bVerticalSync = false) :
			strTitle(cstTitle),
			nX(0), nY(0), nW(unWidth), nH(unHeight),
			bVSync(bVerticalSync), nAspectRatio((Float32)unWidth / (Float32)unHeight) { }
		inline OutStream& operator<<(OutStream& rStream) {
			return rStream << "--==<window_info>==--" << std::endl <<
				"title: " << &strTitle[0] << std::endl <<
				"coordx/coordy: " << nX << "/" << nY << std::endl <<
				"width/height: " << nW << "/" << nH << std::endl <<
				"api_version: " << &strApiVer[0] << std::endl <<
				"--==</window_info>==--" << std::endl;
		}
	};
	inline OutStream& operator<<(OutStream& rStream, WindowInfo& rwInfo) { return rwInfo.operator<<(rStream); }
}
#if (NW_WAPI & NW_WAPI_GLFW)
struct NativeWindow {
public:
	GLFWwindow* pHandle = nullptr;
public:
	inline operator GLFWwindow*() { return pHandle; };
};
struct DeviceContext {
public:
	GLFWcontext* pHandle = nullptr;
public:
	inline operator GLFWcontext*() { return pHandle; };
};
#endif
#if (NW_WAPI & NW_WAPI_WIN)
struct NativeWindow {
public:
	HWND pHandle = nullptr;
public:
	inline operator HWND() { return pHandle; };
};
struct WindowContext {
public:
	HDC pHandle = nullptr;
	HGLRC pGfx = nullptr;
public:
	inline operator HDC() { return pHandle; };
	inline operator HGLRC() { return pGfx; };
};
#endif	// NW_WAPI
namespace NW
{
	/// Window Class
	class NW_API CoreWindow
	{
	public:
		CoreWindow(const WindowInfo& rwInfo);
		CoreWindow(const CoreWindow& rCpy) = delete;
		~CoreWindow();

		// --getters
		inline UInt16 GetSizeW() const		{ return m_wInfo.nW; }
		inline UInt16 GetSizeH() const		{ return m_wInfo.nH; }
		inline UInt16 GetCoordX() const		{ return m_wInfo.nX; }
		inline UInt16 GetCoordY() const		{ return m_wInfo.nY; }
		inline UInt16 GetOpacity() const	{ return m_wInfo.nOpacity; }
		inline const char* GetTitle() const	{ return &m_wInfo.strTitle[0]; }
		inline const WindowInfo& GetWindowInfo() const { return m_wInfo; }
		inline NativeWindow& GetNative()	{ return m_Native; }
		inline WindowContext& GetContext()	{ return m_Context; }
		// --setters
		void SetTitle(const char* strTitle);
		void SetVSync(bool bEnable);
		void SetFocused(bool bFocus);
		void SetEnabled(bool bEnable);
		void SetOpacity(float nOpacity);
		void SetIcon(const ImageInfo& rImgInfo);
		void SetEventCallback(const EventCallback& fnOnEvent);
		void SetKeyboardMode(KeyboardModes kbdMode);
		void SetCursorMode(CursorModes crsMode);
		// --predicates
		inline bool IsVSync() const		{ return m_wInfo.bVSync; }
		inline bool IsHovered() const	{ return m_wInfo.bIsHovered; }
		inline bool IsFocused() const	{ return m_wInfo.bIsFocused; }
		inline bool IsEnabled() const	{ return m_wInfo.bIsEnabled; }
		// --operators
		void operator=(const CoreWindow& rCpy) = delete;
		void operator delete(Ptr pBlock) = delete;
		void operator delete[](Ptr pBlock) = delete;
		// --core_methods
		bool Init();
		void OnQuit();
		void Update();
	private:
		inline bool InitWindow();
		inline void QuitWindow();
		inline bool InitContext();
		inline void QuitContext();
#if (NW_WAPI & NW_WAPI_GLFW)
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
#endif
#if (NW_WAPI & NW_WAPI_WIN)
		static inline LRESULT __stdcall MsgProcInit(HWND pWindow, UINT unMsg, WPARAM wParam, LPARAM lParam);
		static inline LRESULT __stdcall StaticMsgProc(HWND pWindow, UINT unMsg, WPARAM wParam, LPARAM lParam);
		inline LRESULT __stdcall MsgProc(HWND pWindow, UINT unMsg, WPARAM wParam, LPARAM lParam);
#endif
	private:
		WindowInfo m_wInfo;
		NativeWindow m_Native;
		WindowContext m_Context;
#if (NW_WAPI & NW_WAPI_WIN)
		MSG m_wMsg;
		WNDCLASSEX m_wndClass;
		PAINTSTRUCT m_pntStruct;
#endif
	};
}
#endif	// NW_WAPI

#endif	// NW_CORE_WINDOW_H
/*
* Development started 18.10.2020
* --Tutorials used:
* -->Cherno GameEngine series
* -->MihailRis - Minecraft Voxel Engine
*/