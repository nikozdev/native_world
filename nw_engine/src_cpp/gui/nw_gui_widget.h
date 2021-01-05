#ifndef NW_AGWIDGET_H
#define NW_AGWIDGET_H

#include <lib/nw_io_lib.h>

#include <gui/nw_gui_core.h>

namespace NW
{
	/// Abstracted GraphicsWidget class
	class NW_API AGuiWidget
	{
		using IdStack = DStack<UInt32>;
	public:
		AGuiWidget();
		virtual ~AGuiWidget();
		
		// -- Getters
		inline UInt32 GetId() const { return m_unId; }
		virtual inline void GetSize() const = 0;
		virtual inline V2f GetCoord() const = 0;
		// -- Setters
		virtual void SetSize(float nWidth, float nHeight) = 0;
	private:
		static IdStack s_IdStack;
		UInt32 m_unId;
	};
}

namespace NW
{
	/// GuiWindow struct
	/// -- Handles widgets and unites them as a block
	/// -- Has own window which includes relative cursor position and events
	struct NW_API GuiWindow
	{
	public:
		String strName = "gui_window";
		UInt32 unId = 0;

		CursorState Cursor;
		ButtonState KeyboardStates[NW_KEY_LAST];
		ButtonState MouseStates[NW_MS_BTN_LAST];

		V2i xyCrd = { 0, 0 };
		V2i whSize = { 0, 0 }, whMaxSize = { 0, 0 };

		struct {
			V2i xyCrd = { 0, 0 };
			V2i whSize = { 0, 0 }, whMaxSize = { 0, 0 };
		} Content;

		DArray<AGuiWidget*> Widgets;

		bool bIsFocused = false;
		bool bIsClosed = false;
	};
}

#endif // NW_AGWIDGET_H