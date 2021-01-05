#ifndef NW_AWINDOW_H
#define NW_AWINDOW_H

#include <gl/gcontext/nw_gcontext.h>

#include <nw_decl.hpp>
#include <gl_decl.hpp>

#if (defined NW_WINDOW)
namespace NW
{
	enum WApiTypes : UInt32 {
		WAPI_NONE = 0,
		WAPI_COUT = NW_WINDOW_CONSOLE,
		WAPI_GLFW = NW_WINDOW_GLFW,
		WAPI_WIN = NW_WINDOW_WIN
	};
	using EventCallback = std::function<void(AEvent&)>;
	struct NW_API WindowInfo
	{
		String strTitle = "window";
		UInt16 unWidth = 800, unHeight = 600;
		Float32 nAspectRatio = 800.0f / 600.0f;
		bool bVSync = false;

		WApiTypes WApiType = WAPI_NONE;

		EventCallback fnEvCallback = nullptr;

		WindowInfo(const char* cstTitle = "CheerNikWindow",
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

		// -- Getters
		virtual UInt16 GetWidth() const = 0;
		virtual UInt16 GetHeight() const = 0;
		virtual void* GetNativeWindow() = 0;
		virtual const WindowInfo& GetWindowInfo() = 0;
		virtual AGContext* GetGContext() = 0;
		// -- Setters
		virtual void SetTitle(const char* strTitle) = 0;
		virtual void SetEventCallback(const EventCallback& fnEvCallback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		// Predicates
		virtual bool IsVSync() const = 0;

		// Interface Functions
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
	/// WindowOgl class for The "Windows" platform
	/// --Works with GLFW
	/// --Can interact mostly by events but not with the application directly
	class NW_API WindowOgl : public AWindow
	{
	public:
		// Constructor&Destructor
		/// Has default window description
		WindowOgl(const WindowInfo& rWindowInfo);
		virtual ~WindowOgl();

		// Getters
		virtual inline UInt16 GetWidth() const override { return m_WindowInfo.unWidth; }
		virtual inline UInt16 GetHeight() const override { return m_WindowInfo.unHeight; }
		virtual inline const WindowInfo& GetWindowInfo() override { return m_WindowInfo; }
		virtual inline void* GetNativeWindow() override { return m_pNative; }
		virtual inline AGContext* GetGContext() override { return m_pGContext.get(); }
		// Setters
		virtual void SetTitle(const char* strTitle) override;
		inline void SetEventCallback(const EventCallback& fnEvCallback) override { m_WindowInfo.fnEvCallback = fnEvCallback; }
		// Predicates
		inline bool IsVSync() const override { return m_WindowInfo.bVSync; }

		void SetVSync(bool enabled) override;

		// Main functions
		virtual bool Init() override;
		virtual void OnQuit() override;
		virtual void Update() override;
	private: // Implementation functions
	private: // Implementation Attributes
		GLFWwindow* m_pNative;
		RefOwner<AGContext> m_pGContext;
		WindowInfo m_WindowInfo;
	};
}
#endif // NW_WINDOW
#if (NW_WINDOW & NW_WINDOW_CONSOLE)
namespace NW
{
	class NW_API CoutWindow : public AWindow
	{
	public:
		CoutWindow(const WindowInfo& rWindowInfo = { "Console Window", 50, 30 });
		~CoutWindow();

		// -- Getters
		virtual inline USInt GetWidth() const override { return m_WindowInfo.usiWidth; };
		virtual inline USInt GetHeight() const override { return m_WindowInfo.usiHeight; };
		virtual inline const WindowInfo& GetWindowInfo() override { return m_WindowInfo; }
		virtual inline void* GetNativeWindow() const override { return m_hOrigCout; }
		inline void* GetHCin() const { return m_hCin; }
		inline void* GetHCout() const { return m_hCout; }
		inline GContextCout* GetGContextCout() { return m_rGContext.get(); }
		inline vsi4d& GetRect() { return m_xywhRect; }
		// -- Setters
		virtual void SetEventCallback(const ev_Callback& rOnEventFn) override { m_WindowInfo.ev_Callback = rOnEventFn; };
		virtual void SetVSync(bool bEnabled) override;
		// -- Predicates
		virtual bool IsVSync() const override { return false; };

		// -- Core Methods
		/// Entire Initialization Implementation:
		/// -> Get Console Handlers -> Check them for INVALID_VALUE
		/// -> SetConsoleWindowInfoRect to 0,0,1,1
		/// -> SetConsoleScreenBufferSize(rect-1)
		/// -> SetConsoleActiveScreenBuffer -> SetCurrentConsoleFontEx
		/// -> GetConsoleScreenBufferInfo -> Check max size
		/// -> SetConsoleWindowInfo with the needed size
		/// -> SetConsoleMode -> SetConsoleCtrlHandler
		virtual bool OnInit() override;
		virtual void OnQuit() override;
		virtual void OnUpdate() override;

	private: // Implementation Methods
	private: // Implementation Attributes
		void* m_hOrigCout, * m_hCout, * m_hCin;

		vsi4d m_xywhRect;
		RefOwner<GContextCout> m_rGContext;

		WindowInfo m_WindowInfo;
	};
}
#endif	// NW_AWINDOW_H

#endif	// NW_AWINDOW_H
/*
* Development started 18.10.2020
* --Tutorials used:
* -->Cherno GameEngine series
* -->MihailRis - Minecraft Voxel Engine
*/
/// WINAPI Functionality for the console
/// ===>BOOL WINAPI ReadFile(
///		_In_ void* hFile,
///		_Out_ LPVOID lpBuffer,
///		_In_ DWORD lpNumberOfBytes_TO_Read,
///		_Out_ LPDWORD lpNumberOfBytesRead,
///		_InOut_ LPOVERLAPPED);
///	===>BOOL WINAPI WriteFile(
///		_In_ void* hFile,
///		_In_ LPVOID lpBuffer,
///		_In_ DWORD lpNumberOfBytes_TO_Read,
///		_Out_ LPDWORD lpNumberOfBytesRead,
///		_InOut_ LPOVERLAPPED);
/// ===>BOOL WINAPI SetConsoleTitle(wchar_t* pointerToTitle);
/// ===>BOOL WINAPI SetConsoleTextAttribute(
///		void* consoleHandler,
///		uint16_t 0b0000'0000); // Set text and background color by flags
/// ===>BOOL WINAPI FillConsoleOutputAttribute(
///		void* consoleHandler,	//
///		uint16_t colorFlags,	//
///		uint32_t length,		// How much cells will be filled
///		COORD dwWriteCoord,		// 
///		uint32_t* lpNumbersOfAttrsWritten);
/// ===>BOOL WINAPI SetConsoleCursorPosition(
///		void* consoleHandler,
///		COORD mousePos);
/// ===>BOOL WINAPI GetConsoleScreenBufferInfo(
///		void* consoleHandler,
///		CONSOLE_SCREEN_BUFFER_INFO* bufInfo);
/// --CONSOLE_SCREEN_BUFFER_INFO{srWindow{Left, Right, Top, Bottom}, color{}}
/// ===>BOOL WINAPI FillConsoleOutputCharacter(
///		void* hConsole,
///		WCHAR char,
///		LENGTH charCount,
///		COORD fromWhere,
///		LPDWORD howMuchWritten);
/// ===>BOOL WINAPI FillConsoleOutputAttribute(
///		void* hConsole,
///		WORD attrib,
///		LENGTH charCount,
///		COORD fromWhere,
///		LPDWORD howMuchWritten);