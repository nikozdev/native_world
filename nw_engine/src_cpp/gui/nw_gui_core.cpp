#include <nw_pch.hpp>
#include "nw_gui_core.h"

#include <gl/render/nw_texture.h>
#include <gl/render/nw_drawable.h>

namespace NW
{
	BitmapFont::BitmapFont(const char* strName) :
		m_strName(strName), 
		m_pBitmap(nullptr) { }
	BitmapFont::~BitmapFont() { }

	// --setters
	void BitmapFont::SetInfo(const BitmapFontInfo& rFontInfo) {
		m_Info = rFontInfo;
		m_Glyphs.clear();
		if (m_pBitmap == nullptr) return;


		for (Int32 ci = m_Info.cFirst; ci <= m_Info.cLast; ci++) {
			SubTexture2d texGlyph = SubTexture2d();
			texGlyph.pOverTex = m_pBitmap;
			texGlyph.whTexSize.x = m_Info.Metrics.Cell.W + m_Info.Metrics.Padding.Lft + m_Info.Metrics.Padding.Rht;
			texGlyph.whTexSize.y = m_Info.Metrics.Cell.H + m_Info.Metrics.Padding.Lft + m_Info.Metrics.Padding.Rht;
			texGlyph.xyTexCrd.x = (ci * texGlyph.whTexSize.x) % m_Info.Metrics.Bitmap.W;
			texGlyph.xyTexCrd.y = (ci * texGlyph.whTexSize.y) / m_Info.Metrics.Bitmap.W;
			m_Glyphs[ci] = texGlyph;
		}
	}
	void BitmapFont::SetBitMap(ATexture2d* pBitmap) { m_pBitmap = pBitmap; }

	// --core_methods
}