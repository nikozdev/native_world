#ifndef NW_CORE_WINDOW_H
#define NW_CORE_WINDOW_H
#include <nw_core.hpp>
#if (defined NW_WAPI)
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

		EventCallback fnOnEvent = [](AEvent&)->void {return; };
	public:
		WindowInfo(const char* cstTitle = "native_window",
			UInt16 unWidth = 800, UInt16 unHeight = 600,
			bool bVerticalSync = false,
			std::function<void(AEvent&)> fnEventCallback = nullptr) :
			strTitle(cstTitle), unWidth(unWidth), unHeight(unHeight),
			bVSync(bVerticalSync), fnOnEvent(fnEventCallback),
			nAspectRatio((Float32)unWidth / (Float32)unHeight) { }
		inline OutStream& operator<<(OutStream& rStream) {
			return rStream << "--==<window_info>==--" << std::endl <<
				"title: " << &strTitle[0] << std::endl <<
				"width x height: " << unWidth << " x " << unHeight << std::endl <<
				"api_version: " << &strApiVer[0] << std::endl <<
				"--==</window_info>==--" << std::endl;
		}
	};
	inline OutStream& operator<<(OutStream& rStream, WindowInfo& rwInfo) { return rwInfo.operator<<(rStream); }
}
#if (NW_WAPI & NW_WAPI_GLFW)
struct GLFWwindow;
struct GLFWimage;
struct GLFWcursor;
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
		inline UInt16 GetWidth() const		{ return m_wInfo.unWidth; }
		inline UInt16 GetHeight() const		{ return m_wInfo.unHeight; }
		inline UInt16 GetOpacity() const	{ return m_wInfo.nOpacity; }
		inline GLFWwindow* GetNative()		{ return m_pNative; }
		inline const WindowInfo& GetWindowInfo() const { return m_wInfo; }
		// --setters
		void SetTitle(const char* strTitle);
		void SetEventCallback(const EventCallback& fnOnEvent);
		void SetVSync(bool bEnable);
		void SetIcon(const ImageInfo& rImgInfo);
		void SetOpacity(float nOpacity);
		// --predicates
		inline bool IsVSync() const { return m_wInfo.bVSync; }
		// --operators
		void operator=(const CoreWindow& rCpy) = delete;
		void operator delete(Ptr pBlock) = delete;
		void operator delete[](Ptr pBlock) = delete;
		// --core_methods
		bool Init();
		void OnQuit();
		void Update();
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
		WindowInfo m_wInfo;
		GLFWwindow* m_pNative;

		GLFWimage* m_pIcon;
		GLFWcursor* m_pCursor;
	};
}
#endif
#if (NW_WAPI & NW_WAPI_WIN)
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
		inline UInt16 GetWidth() const { return m_wInfo.unWidth; }
		inline UInt16 GetHeight() const { return m_wInfo.unHeight; }
		inline UInt16 GetOpacity() const { return m_wInfo.nOpacity; }
		inline HWND GetNative() { return m_pNative; }
		inline const char* GetTitle() { return &m_wInfo.strTitle[0]; }
		inline const WindowInfo& GetWindowInfo() const { return m_wInfo; }
		// --setters
		void SetTitle(const char* strTitle);
		void SetEventCallback(const EventCallback& fnOnEvent);
		void SetVSync(bool bEnable);
		void SetIcon(const ImageInfo& rImgInfo);
		void SetOpacity(float nOpacity);
		// --predicates
		inline bool IsVSync() const { return m_wInfo.bVSync; }
		// --operators
		void operator=(const CoreWindow& rCpy) = delete;
		void operator delete(Ptr pBlock) = delete;
		void operator delete[](Ptr pBlock) = delete;
		// --core_methods
		bool Init();
		void OnQuit();
		void Update();
	private:
		static inline LRESULT __stdcall MsgProcInit(HWND pWindow, UINT unMsg, WPARAM wParam, LPARAM lParam);
		static inline LRESULT __stdcall StaticMsgProc(HWND pWindow, UINT unMsg, WPARAM wParam, LPARAM lParam);
		inline LRESULT __stdcall MsgProc(HWND pWindow, UINT unMsg, WPARAM wParam, LPARAM lParam);
	private:
		WindowInfo m_wInfo;
		HWND m_pNative;
		HDC m_devContext;
		HGLRC m_oglContext;

		MSG m_wMsg;
		WNDCLASSEX m_wndClass;
		PIXELFORMATDESCRIPTOR m_pxfDesc;
		PAINTSTRUCT m_pntStruct;
	};
}
#endif
#endif // NW_WAPI
#endif	// NW_CORE_WINDOW_H
/*
* Development started 18.10.2020
* --Tutorials used:
* -->Cherno GameEngine series
* -->MihailRis - Minecraft Voxel Engine
*/