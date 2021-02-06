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
		inline OStream& operator<<(OStream& rStream) {
			rStream << "--==<window_info>==--" << std::endl <<
				"title: " << &strTitle[0] << std::endl <<
				"width x height: " << unWidth << " x " << unHeight << std::endl <<
				"api_version: " << &strApiVer[0] << std::endl <<
				"--==</window_info>==--" << std::endl;
			return rStream;
		}
	};
	inline OStream& operator<<(OStream& rStream, WindowInfo& rwInfo) { return rwInfo.operator<<(rStream); }
}
#if (NW_WINDOW & NW_WINDOW_GLFW)
struct GLFWwindow;
struct GLFWimage;
namespace NW
{
	/// Window Class
	class NW_API AppWindow
	{
	public:
		AppWindow(const WindowInfo& rwInfo);
		~AppWindow();

		// --getters
		inline UInt16 GetWidth() const { return m_Info.unWidth; }
		inline UInt16 GetHeight() const { return m_Info.unHeight; }
		inline Ptr GetNative() { return m_pNative; }
		inline const WindowInfo& GetWindowInfo() const { return m_Info; }
		// --setters
		void SetTitle(const char* strTitle);
		void SetEventCallback(const EventCallback& fnEvCallback);
		void SetVSync(bool bEnable);
		void SetIcon(const ImageInfo& rImgInfo);
		void SetOpacity(float nOpacity);
		// --predicates
		inline bool IsVSync() const { return m_Info.bVSync; }

		// --core_methods
		bool Init();
		void OnQuit();
		void Update();

		static AppWindow* Create(const WindowInfo& rWindowInfo);
		static void Create(const WindowInfo& rWindowInfo, RefKeeper<AppWindow>& rWindow);
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
		WindowInfo m_Info;
	};
}
#endif
#endif // NW_WINDOW
#endif	// NW_AWINDOW_H
/*
* Development started 18.10.2020
* --Tutorials used:
* -->Cherno GameEngine series
* -->MihailRis - Minecraft Voxel Engine
*/