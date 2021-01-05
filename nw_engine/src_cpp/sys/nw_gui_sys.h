#ifndef NW_GUI_SYSTEM_H
#define NW_GUI_SYSTEM_H

#if (NW_GUI & NW_GUI_NATIVE)
#include <gui/nw_gui_widget.h>
#endif	// NW_GUI
#if (NW_GUI & NW_GUI_IMGUI)
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
struct ImGuiIO;
struct ImGuiStyle;
//using GuiIO = ImGuiIO;
//using GuiStyle = ImGuiStyle;
#endif	// NW_GUI

namespace NW
{
	/// GuiSys Static Class
	class NW_API GuiSys
	{
	public:
		// -- Getters
		static inline GuiIO& GetGuiIO() { return s_GuiIO; }
		static inline GuiStyle& GetGuiStyle() { return s_GuiStyle; }
		static inline GuiWindow* GetWindow() { return s_pCurrWindow; }
		// -- Setters
		static void SetWindow(GuiWindow* pWindow);
		// -- Predicates
		static inline bool IsDrawing() { return s_bIsDrawing; }

		// -- Core methods
		static void OnInit();
		static void OnQuit();
		static void Update();
		// -- OnEvent methods
		static void OnEvent(MouseEvent& rmEvt);
		static void OnEvent(KeyboardEvent& rkEvt);
		static void OnEvent(WindowEvent& rwEvt);

		static void BeginDraw();
		static void BeginWindow(const char* strName);
		static void OnDraw(AGuiWidget* pAGuiWidget);
		static void EndWindow();
		static void EndDraw();

		static void OnDrawStr(const char* str, V2f whSize = V2f{ 8.0f, 16.0f }, V2f xyCoord = V2f{ 0.0f, 0.0f }, float zRotation = 0.0f);
		static inline void OnDrawCh(char cCharacter, V2f whSize = V2f{ 8.0f, 16.0f }, V2f xyCoord = V2f{ 0.0f, 0.0f }, float zRotation = 0.0f);

		static void OnDrawTextF(const V2i& xyCoord, const V2i& whSize, const char* sFormat, ...);
		static void OnDrawImage(void* pImgId, const V2i& xyCoord, const V2i& whSize);
		static bool OnDrawSliderI8(Int8* pFirst, UInt16 unCount, Int32 nMin, Int32 nMax, const V2i& xyCoord, const V2i& whSize, const char* sLabel);
		static bool OnDrawSliderI32(Int32* pFirst, UInt16 unCount, Int32 nMin, Int32 nMax, const V2i& xyCoord, const V2i& whSize, const char* sLabel);
		static bool OnDrawSliderF(float* pFirst, UInt16 unCount, float nMin, float nMax, const V2i& xyCoord, const V2i& whSize, const char* sLabel);
		static bool OnDrawInputI8(Int8* pFirst, UInt16 unCount, const V2ui& xyCoord, const V2ui& whSize, const char* sLabel);
		static bool OnDrawInputI32(Int32* pFirst, UInt16 unCount, const V2ui& xyCoord, const V2ui& whSize, const char* sLabel);
		static bool OnDrawInputF(float* pFirst, UInt16 unCount, const V2ui& xyCoord, const V2ui& whSize, const char* sLabel);
		static bool OnDrawButton(const V2ui& xyCoord, const V2ui& whSize, const char* sLabel);
	private:	// Implementation Attribs
		static GuiIO s_GuiIO;
		static GuiStyle s_GuiStyle;
		
		static bool s_bIsDrawing;

		static GuiWindow* s_pCurrWindow;
	};
}

#endif // NW_GUI_SYSTEM_H