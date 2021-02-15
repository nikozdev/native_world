#ifndef NWG_ENTITY_H
#define NWG_ENTITY_H

#include <gfx_core.hpp>

#if (defined NW_GAPI)
namespace NW
{
	/// GraphicsEntity class
	class NW_API GfxEntity
	{
		friend class GfxEngine;
	public:
		GfxEntity() = default;
		virtual ~GfxEntity() = default;
		// --core_methods
		virtual void Bind() const = 0;
	};
}
#if (NW_GAPI & NW_GAPI_OGL)
namespace NW
{
	/// GraphicsEntityOpenGL class
	class NW_API GfxEntityOgl : public GfxEntity
	{
	public:
		GfxEntityOgl(GfxEngineOgl& rGfx) : m_rGfx(rGfx), m_unRId(0) {}
		virtual ~GfxEntityOgl() = default;
		// --getters
		inline UInt32 const GetRenderId() const { return m_unRId; }
		// --core_methods
		virtual void Bind() const = 0;
	protected:
		GfxEngineOgl& m_rGfx;
		UInt32 m_unRId;
	};
}
#endif
#if (NW_GAPI & NW_GAPI_DX)
namespace NW
{
	/// GraphicsEntityDirectX class
	class NW_API GfxEntityDx
	{
	public:
		GfxEntityDx(GfxEngineDx& rGfx) : m_rGfx(rGfx) {}
		virtual ~GfxEntityOgl();
		// --core_methods
		virtual void Bind() const = 0;
	protected:
		GfxEngineDx& m_rGfx;
	};
}
#endif
#endif	// NW_GAPI
#endif	// NWG_ENTITY_H