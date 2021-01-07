#ifndef NW_AFRAME_BUFFER_H
#define NW_AFRAME_BUFFER_H

#include <core/nw_data_res.h>

#include <gl_decl.hpp>

#if (defined NW_GRAPHICS)
namespace NW
{
	/// FrameBufInfo struct
	struct NW_API FrameBufInfo
	{
		UInt32 unWidth = 0, unHeight = 0;
		UInt32 unSamples = 1;

		bool bSwapChainTarget = false;
	};
	/// Abstract FrameBuf class
	class NW_API AFrameBuf : public ADataRes
	{
	public:
		AFrameBuf(const char* strName, const FrameBufInfo& rFbInfo);
		virtual ~AFrameBuf();
	
		// --getters
		inline const UInt32 GetRenderId() const { return m_unRId; }
		inline const UInt32 GetRenderIdDepth() const { return m_unRIdDepth; }
		inline ATexture2d* GetColorAttachment() { return m_ColorAttach; }
		inline const FrameBufInfo& GetInfo() const { return m_Info; }
		// --setters
		void SetSizeWH(UInt32 unWidth, UInt32 unHeight);
		// --core_methods
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Remake() = 0;
		virtual void Clear(UInt32 unAttachmentsMask = FB_COLOR | FB_DEPTH | FB_STENCIL) = 0;
		// --core_methods
		virtual bool LoadF(const char* strFPath) = 0;
		virtual bool SaveF(const char* strFPath) = 0;

		static AFrameBuf* Create(const char* strName, const FrameBufInfo& rFbInfo);
	protected:
		UInt32 m_unRId;
		UInt32 m_unRIdDepth;
		ATexture2d* m_ColorAttach;
		FrameBufInfo m_Info;
	};
}
#endif	// NW_GRAPHICS
#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
namespace NW
{
	/// FrameBufOpenGL
	class FrameBufOgl : public AFrameBuf
	{
	public:
		FrameBufOgl(const char* strName, const FrameBufInfo& rFbInfo);
		virtual ~FrameBufOgl();

		// --core_methods
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void Remake() override;
		virtual void Clear(UInt32 unAttachmentsMask = FB_COLOR | FB_DEPTH | FB_STENCIL) override;
		// --data_methods
		virtual bool LoadF(const char* strFPath) override;
		virtual bool SaveF(const char* strFPath) override;
	};
}
#endif // NW_GRAPHICS
#if (NW_GRAPHICS & NW_GRAPHICS_COUT)
namespace NW
{
	class NW_API CoutFrameBuf : public AFrameBuf
	{
		friend class GraphicsApiCout;
	public:
		// Constructor&Destructor
		CoutFrameBuf(CoutWindow* pWindow);
		~CoutFrameBuf();

		// Getters
		inline UInt16 GetSizeWxH() { return m_unSize; }
		inline UInt16 GetSizeW() { return m_xywhRect.Right; }
		inline UInt16 GetSizeH() { return m_xywhRect.Bottom; }
		// Setters
		bool SetSizeWH(Int16 sizeW, Int16 sizeH);
		// --Buffer Chars
		inline void SetCharN(Int16 nPx, UInt16 unColor = 0b0010'0000, wchar_t wcPattern = '#')
		{
			if (nPx < m_unSize)
			{
				m_ciChars[nPx].Attributes = unColor;
	#ifdef UNICODE
				m_ciChars[nPx].Char.UnicodeChar = wcPattern;
	#else
				m_ciChars[nPx].Char.AsciiChar = wcPattern;
	#endif
			}
		}
		inline void SetCharXY(Int16 nX, Int16 nY, UInt16 unColor = 0b0010'0000, wchar_t wcPattern = '#')
		{
			if (IsInBounds(nX, nY))
			{
				SetCharN(m_xywhRect.Right * nY, unColor, wcPattern);
			}
		}
		inline void SetPixelN(Int16 nPx, UInt16 unColor = 0b0010'0000, wchar_t wcPattern = '#')
		{
			if (nPx < m_unSize - 1)
			{
				m_ciChars[nPx].Attributes = unColor;
				m_ciChars[nPx + 1].Attributes = unColor;
	#ifdef UNICODE
				m_ciChars[nPx].Char.UnicodeChar = wcPattern;
				m_ciChars[nPx + 1].Char.UnicodeChar = wcPattern;
	#else
				m_ciChars[nPx].Char.AsciiChar = wcPattern;
				m_ciChars[nPx + 1].Char.AsciiChar = wcPattern;
	#endif
			}
		}
		inline void SetPixelXY(Int16 nX, Int16 nY, UInt16 unColor = 0b0010'0000, wchar_t wcPattern = '#')
		{
			SetPixelN(m_whSize.X * nY + nX * 2, unColor, wcPattern);
		}
		// Predicates
		inline bool IsInBounds(Int32 nX, Int32 nY)
		{
			return (nX >= m_xywhRect.Left && nX <= m_xywhRect.Right &&
				nY >= m_xywhRect.Top && nY <= m_xywhRect.Bottom);
		}
		inline bool IsInBounds(vsi2d xyCoord)
		{
			return (xyCoord.X >= m_xywhRect.Left && xyCoord.Y <= m_xywhRect.Right &&
				xyCoord.Y >= m_xywhRect.Top && xyCoord.Y <= m_xywhRect.Bottom);
		}
		inline bool IsInBounds(vsi4d xywhRectangle)
		{
			return (xywhRectangle.Left >= m_xywhRect.Left && xywhRectangle.Right <= m_xywhRect.Right &&
				xywhRectangle.Top >= m_xywhRect.Top && xywhRectangle.Bottom <= m_xywhRect.Bottom);
		}

		// core_methods
		virtual bool OnInit();
		virtual void OnQuit();
		virtual void OnUpdate(ChInfo* buffer = nullptr);

		void Clear(UInt32 uiClearColor);
	private:
		CoutWindow* m_pWindow;
		ChInfo* m_ciChars;

		USInt m_unSize;
		vsi2d m_whSize;
		vsi2d m_whMaxSize;
		vsi4d& m_xywhRect;

		CSBufInfo m_csbOrigInfo;
		CSBufInfo m_csbInfo;
	};
}
#endif // NW_GRAPHICS

#endif // NW_AFRAME_BUFFER_H