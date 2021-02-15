#ifndef NWG_TEXTURE_H
#define NWG_TEXTURE_H
#include <gfx_core.hpp>
#if (defined NW_GAPI)
#include <gfx/gfx_entity.h>
#include <gfx/gfx_tools.h>
namespace NW
{
	/// TextureInfo struct
	struct NW_API TextureInfo
	{
	public:
		TextureWraps WrapTypeS = TXW_REPEAT, WrapTypeT = TXW_REPEAT, WrapTypeR = TXW_REPEAT;
		TextureFilters FilterMin = TXF_NEAREST, FilterMag = TXF_NEAREST;
		TextureFormats texFormat = TXF_RGBA;
		TextureInterFormats texInterFormat = TXFI_RGBA8;
		PixelFormats pxFormat = PXF_UINT8;
		Bit bGenMipmap = true;
		Bit bGenSubImage = true;
		UInt32 unSamples = 1;
	};
}
namespace NW
{
	/// Texture class
	/// Description:
	/// -> Set props and data -> LoadData -> MakeTexture -> Bind drawing stuff
	/// -> Enable -> Draw -> Disable
	/// --It's a wrapping image which has to wrap a mesh
	class NW_API Texture : public TDataRes<Texture>, public GfxEntity
	{
	public:
		Texture(const char* strName, TextureTypes texTypes);
		virtual ~Texture();
		// --getters
		inline UInt32 GetTexSlot() const { return m_unTexSlot; }
		inline TextureTypes GetType() const { return m_texType; }
		inline const TextureInfo& GetTexInfo() const { return m_texInfo; }
		inline const ImageInfo& GetImgInfo() const { return m_imgInfo; }
		// --setters
		void SetSlot(UInt32 unSlot) { m_unTexSlot = unSlot; }
		void SetInfo(const TextureInfo& rTexInfo);
		void SetInfo(const ImageInfo& rImgInfo);
		// --core_methods
		virtual void Bind() const = 0;
		virtual void Remake() = 0;
		virtual void Clear(Ptr pValue) = 0;
		// --data_methods
		virtual bool SaveF(const char* strFPath) override;
		virtual bool LoadF(const char* strFPath) override;
	protected:
		TextureTypes m_texType;
		UInt32 m_unTexSlot;
		TextureInfo m_texInfo;
		ImageInfo m_imgInfo;
	};
}
#if (NW_GAPI & NW_GAPI_OGL)
namespace NW
{
	/// Texture class
	/// Description:
	/// -> Set props and data -> LoadData -> MakeTexture -> Bind drawing stuff
	/// -> Enable -> Draw -> Disable
	/// --It's a wrapping image which has to wrap a mesh
	class NW_API TextureOgl : public Texture, public GfxEntityOgl
	{
	public:
		TextureOgl(const char* strName, TextureTypes texTypes, GfxEngineOgl& rGfx);
		~TextureOgl();
		// --core_methods
		virtual void Bind() const override;
		virtual void Remake() override;
		virtual void Clear(Ptr pValue) override;
	};
}
#endif
#if (NW_GAPI & NW_GAPI_DX)
namespace NW
{
	/// Texture class
	/// Description:
	/// -> Set props and data -> LoadData -> MakeTexture -> Bind drawing stuff
	/// -> Enable -> Draw -> Disable
	/// --It's a wrapping image which has to wrap a mesh
	class NW_API TextureDx : public Texture, public GfxEntityDx
	{
	public:
		TextureDx(const char* strName, TextureTypes texTypes, GfxEngineDx& rGfx);
		~TextureDx();
		// --core_methods
		virtual void Bind(UInt32 unTexSlot = 0u) override;
		virtual void Unbind() override;
		virtual void Remake() override;
		virtual void Clear(Ptr pValue) override;
	};
}
#endif
#endif	// NW_GAPI
#endif // GFX_TEXTURE_H