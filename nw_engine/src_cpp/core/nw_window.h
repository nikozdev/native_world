#ifndef NW_CORE_WINDOW_H
#define NW_CORE_WINDOW_H
#include <nw_core.hpp>
namespace NW
{
	struct NW_API WindowInfo
	{
	public:
		String strTitle = "default";
		String strApiVer = "default";
		UInt16 nX = 0, nY = 0, nW = 800, nH = 600;
		Float32 nAspectRatio = 800.0f / 600.0f;
		Float32 nOpacity = 0.0f;
		Bit bVSync = false;
		Bit bIsHovered = false;
		Bit bIsFocused = false;
		Bit bIsEnabled = false;
		EventCallback fnOnEvent = [](AEvent&)->void { return; };
	public:
		WindowInfo(const char* cstTitle = "default", UInt16 unWidth = 800, UInt16 unHeight = 600, Bit bVSync = false);
		OutStream& operator<<(OutStream& rStream);
	};
}
#if (defined NW_PLATFORM_WINDOWS)
namespace NW
{
	/// Window Class
	class NW_API CoreWindow : public AMemUser
	{
	public:
		CoreWindow(const WindowInfo& rwInfo);
		CoreWindow(const CoreWindow& rCpy) = delete;
		virtual ~CoreWindow();
		// --getters
		inline UInt16 GetSizeW() const		{ return m_wInfo.nW; }
		inline UInt16 GetSizeH() const		{ return m_wInfo.nH; }
		inline UInt16 GetCoordX() const		{ return m_wInfo.nX; }
		inline UInt16 GetCoordY() const		{ return m_wInfo.nY; }
		inline UInt16 GetOpacity() const	{ return m_wInfo.nOpacity; }
		inline const char* GetTitle() const	{ return &m_wInfo.strTitle[0]; }
		inline const WindowInfo& GetWindowInfo() const { return m_wInfo; }
		inline HWND& GetNative()	{ return m_pNative; }
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
		// --core_methods
		void Update();
	private:
		static inline LRESULT __stdcall MsgProcInit(HWND pWindow, UINT unMsg, WPARAM wParam, LPARAM lParam);
		static inline LRESULT __stdcall StaticMsgProc(HWND pWindow, UINT unMsg, WPARAM wParam, LPARAM lParam);
		inline LRESULT __stdcall MsgProc(HWND pWindow, UINT unMsg, WPARAM wParam, LPARAM lParam);
	private:
		WindowInfo m_wInfo;
		HWND m_pNative;
		MSG m_wMsg;
		WNDCLASSEX m_wndClass;
		PAINTSTRUCT m_pntStruct;
	};
}
#endif	// NW_PLATFORM

#endif	// NW_CORE_WINDOW_H
/*
* Development started 18.10.2020
* --Tutorials used:
* -->Cherno GameEngine series
* -->MihailRis - Minecraft Voxel Engine
*/