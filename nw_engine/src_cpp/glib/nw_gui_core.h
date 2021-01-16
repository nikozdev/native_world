#ifndef GUI_CORE_H
#define GUI_CORE_H

#include <glib/nw_texture.h>

#include <nwlib/nw_event.h>

#include <glib_decl.hpp>

namespace NW
{
	/// BitmapFontInfo struct
	/// Description:
	/// -- Has the information for sampling bitmap textures to get glyphs
	/// -- Here are all metrics and sizes: for cell, border and glyph
	struct NW_API BitmapFontInfo
	{
	public:
		struct {
			struct { UInt32 W = 0, H = 0; } Bitmap;
			struct { UInt32 W = 0, H = 0; } Cell;
			struct { UInt32 Lft = 0, Top = 0, Rht = 0, Bot = 0; } Padding;
		} Metrics;

		Char cFirst = ' ';		// default ASCII printable begining - 32
		Char cLast = 'z';		// ASCII last printable character - 126
	};
	/// BitmapFont class
	/// Description:
	/// -- It is used by GuiSys to render text with bitmap textures
	/// -- To use this - set up the bitmap, metrics and encoding
	class NW_API BitmapFont
	{
	private:
		const char* m_strName;
		BitmapFontInfo m_Info;
		ATexture2d* m_pBitmap;
		HashMap<Char, SubTexture2d> m_Glyphs;
	public:
		V4f rgbaClr;
	public:
		BitmapFont(const char* strName);
		~BitmapFont();

		// --getters
		inline const char* GetName() const { return m_strName; }
		inline const BitmapFontInfo& GetInfo() const { return m_Info; }
		inline const ATexture2d* GetBitmap() const { return m_pBitmap; }
		inline const SubTexture2d* GetGlyph(Char cGlyph) {
			auto itGlyph = m_Glyphs.find(cGlyph);
			return itGlyph == m_Glyphs.end() ? nullptr : &(itGlyph->second);
		}
		// --setters
		void SetInfo(const BitmapFontInfo& rFontInfo);
		void SetBitMap(ATexture2d* pBitmap);
		// --core_methods
	};
}
namespace NW
{
	/// GuiIO struct
	/// -- Defines Gui behaviour, input, output and events
	struct NW_API GuiIO
	{
	public:
	};
	/// GuiStyle struct
	/// -- Defines Gui view of GuiSys
	struct NW_API GuiStyle
	{
	public:
		List2<BitmapFont> bmFont;
		GMaterial* pGMtlWidgets = nullptr;
	};
}

#endif	// GUI_CORE_H