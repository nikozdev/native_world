#ifndef NWG_SHADER_H
#define NWG_SHADER_H
#include <gfx/gfx_entity.h>
#include <gfx_core.hpp>
#include <gfx/gfx_buffer.h>
#if (defined NW_GAPI)
namespace NW
{
	/// SubShader Class
	class NW_API SubShader : public ACodeRes, public GfxEntity
	{
	public:
		friend class Shader;
		friend class ShaderOgl;
		friend class ShaderDx;
	public:
		SubShader(const char* strName, ShaderTypes sdType);
		virtual ~SubShader();
		// --getters
		inline ShaderTypes GetType() const { return m_shdType; }
		virtual inline Shader* GetOverShader() = 0;
		// --core_methods
		virtual void Attach(Shader* pOverShader) = 0;
		virtual void Detach() = 0;
		virtual void Remake() = 0;
		virtual bool Compile() = 0;
		// --data_methods
		virtual bool SaveF(const char* strFPath) override;
		virtual bool LoadF(const char* strFPath) override;
	protected:
		ShaderTypes m_shdType;
	};
	/// Shader Class
	class NW_API Shader : public ACodeRes, public GfxEntity
	{
	public:
		using Globals = HashMap<String, Int32>;
		using Blocks = HashMap<String, Int32>;
		friend class SubShader;
		friend class SubShaderOgl;
		friend class SubShaderDx;
	public:
		Shader(const char* strName);
		virtual  ~Shader();
		// --getters
		inline const VertexBufLayout& GetVtxLayout() const { return m_vtxLayout; }
		inline const ShaderBufLayout& GetShdLayout() const { return m_shdLayout; }
		inline const Globals& GetGlobals() const { return m_Globals; }
		inline const Blocks& GetBlocks() const { return m_Blocks; }
		virtual inline const SubShader* GetSubShader(ShaderTypes sdType) = 0;
		// --setters
		virtual void SetBool(const char* strName, bool value) const = 0;
		virtual void SetInt(const char* strName, int value) const = 0;
		virtual void SetIntArray(const char* strName, Int32* pIntArr, UInt32 unCount) const = 0;
		virtual void SetUIntArray(const char* strName, UInt32* pUIntArr, UInt32 unCount) const = 0;
		virtual void SetFloat(const char* strName, float value) const = 0;
		virtual void SetFloatArray(const char* strName, float* pFloatArr, UInt32 unCount) const = 0;
		virtual void SetV2f(const char* strName, const V2f& value) const = 0;
		virtual void SetV3f(const char* strName, const V3f& value) const = 0;
		virtual void SetV4f(const char* strName, const V4f& value) const = 0;
		virtual void SetM4f(const char* strName, const Mat4f& value) const = 0;
		// --core_methods
		virtual void Bind() const = 0;
		virtual void Remake() = 0;
		virtual bool Compile() = 0;
		// --data_methods
		virtual bool SaveF(const char* strFPath) override;
		virtual bool LoadF(const char* strFPath) override;
	protected:
		VertexBufLayout m_vtxLayout;
		ShaderBufLayout m_shdLayout;
		mutable Globals m_Globals;
		mutable Blocks m_Blocks;
	};
}
#if (NW_GAPI & NW_GAPI_OGL)
namespace NW
{
	/// SubShader Class
	class NW_API SubShaderOgl : public SubShader, public GfxEntityOgl
	{
	public:
		friend class ShaderOgl;
	public:
		SubShaderOgl(const char* strName, ShaderTypes sdType, GfxEngineOgl& rGfx);
		virtual ~SubShaderOgl();
		// --getters
		virtual inline Shader* GetOverShader() override { return reinterpret_cast<Shader*>(m_pOverShader); }
		// --core_methods
		virtual void Bind() const override { }
		virtual void Attach(Shader* pOverShader) override;
		virtual void Detach() override;
		virtual void Remake() override;
		virtual bool Compile() override;
	private:
		inline bool CodeProc();
	private:
		ShaderOgl* m_pOverShader;
	};
	/// ShaderOgl Class
	class NW_API ShaderOgl : public Shader, public GfxEntityOgl
	{
	public:
		friend class SubShaderOgl;
	public:
		ShaderOgl(const char* strName, GfxEngineOgl& rGfx);
		virtual ~ShaderOgl();
		// --getters
		virtual inline const SubShader* GetSubShader(ShaderTypes sdType) override;
		// --setters
		virtual void SetBool(const char* strName, bool value) const override;
		virtual void SetInt(const char* strName, int value) const override;
		virtual void SetIntArray(const char* strName, Int32* pIntArr, UInt32 unCount) const override;
		virtual void SetUIntArray(const char* strName, UInt32* pUIntArr, UInt32 unCount) const override;
		virtual void SetFloat(const char* strName, float value) const override;
		virtual void SetFloatArray(const char* strName, float* pFloatArr, UInt32 unCount) const override;
		virtual void SetV2f(const char* strName, const V2f& value) const override;
		virtual void SetV3f(const char* strName, const V3f& value) const override;
		virtual void SetV4f(const char* strName, const V4f& value) const override;
		virtual void SetM4f(const char* strName, const Mat4f& value) const override;
		// --core_methods
		virtual void Bind() const override;
		virtual void Remake() override;
		virtual bool Compile() override;
	private:
		inline bool CodeProc();
		inline Int32 GetUniformLoc(const char* strName) const;
		inline Int32 GetBlockIdx(const char* strName) const;
	private:
		DArray<RefKeeper<SubShaderOgl>> m_SubShaders;
	};
	inline const SubShader* ShaderOgl::GetSubShader(ShaderTypes sdType) {
			auto itSub = std::find_if(m_SubShaders.begin(), m_SubShaders.end(),
				[=](RefKeeper<SubShaderOgl>& rSub)->bool {return rSub->GetType() == sdType; });
			return itSub == m_SubShaders.end() ? nullptr : itSub->GetRef();
		}
}
#endif
#if (NW_GAPI & NW_GAPI_DX)
namespace NW
{
	/// SubShaderDirectX Class
	class NW_API SubShaderDx : public SubShader, public GfxEntityDx
	{
	public:
		friend class ShaderDx;
		friend class GfxEngine;
	public:
		SubShaderDx(const char* strName, ShaderTypes sdType);
		virtual ~SubShaderDx();
		// --core_methods
		virtual void Bind() const override { }
		virtual void Attach(Shader* pOverShader) override;
		virtual void Detach() override;
		virtual void Remake() override;
		virtual bool Compile() override;
	protected:
		virtual inline bool CodeProc() override;
	};
	/// ShaderDirectX Class
	class NW_API ShaderDx : public Shader, , public GfxEntityDx
	{
		friend class SubShaderDx;
		friend class GfxEngine;
	public:
		ShaderDx(const char* strName);
		~ShaderDx();
		// --setters
		virtual void SetBool(const char* strName, bool value) const override;
		virtual void SetInt(const char* strName, int value) const override;
		virtual void SetIntArray(const char* strName, Int32* pIntArr, UInt32 unCount) const override;
		virtual void SetUIntArray(const char* strName, UInt32* pUIntArr, UInt32 unCount) const override;
		virtual void SetFloat(const char* strName, float value) const override;
		virtual void SetFloatArray(const char* strName, float* pFloatArr, UInt32 unCount) const override;
		virtual void SetV2f(const char* strName, const V2f& value) const override;
		virtual void SetV3f(const char* strName, const V3f& value) const override;
		virtual void SetV4f(const char* strName, const V4f& value) const override;
		virtual void SetM4f(const char* strName, const Mat4f& value) const override;
		// --core_methods
		virtual void Bind() const override;
		virtual void Remake() override;
		virtual bool Compile() override;
	protected:
		inline bool CodeProc();
	};
}
#endif
#endif	// NW_GAPI
#endif // NWG_SHADER_H