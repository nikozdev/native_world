#ifndef NW_GFX_SHADER_H
#define NW_GFX_SHADER_H

#include <gfx/gfx_tools.h>
#include <gfx/gfx_buffer.h>
#include <gfx_core.hpp>

#if (defined NW_GAPI)
#if (NW_GAPI & NW_GAPI_OGL)
namespace NW
{
	/// SubShader Class
	class NW_API SubShader : public ACodeRes
	{
	public:
		friend class Shader;
	public:
		SubShader(const char* strName, ShaderTypes sdType);
		virtual ~SubShader();

		// --getters
		inline UInt32 GetRenderId() const { return m_unRId; }
		inline ShaderTypes GetType() const { return m_shdType; }
		inline const Shader* GetOverShader() const { return m_pOverShader; }
		// --core_methods
		void Attach(Shader* pOverShader);
		void Detach();
		void Remake();
		virtual bool Compile() override;
		// --data_methods
		virtual bool SaveF(const char* strFPath) override;
		virtual bool LoadF(const char* strFPath) override;
	private:
		inline bool CodeProc();
	private:
		UInt32 m_unRId;
		ShaderTypes m_shdType;
		Shader* m_pOverShader;
	};
	/// Shader Class
	class NW_API Shader : public ACodeRes
	{
	public:
		using Globals = HashMap<String, Int32>;
		using Blocks = HashMap<String, Int32>;
		friend class SubShader;
	public:
		Shader(const char* strName);
		~Shader();

		// --getters
		inline UInt32 GetRenderId() const { return m_unRId; }
		inline const VertexBufLayout& GetVtxLayout() const { return m_vtxLayout; }
		inline const ShaderBufLayout& GetShdLayout() const { return m_shdLayout; }
		inline const Globals& GetGlobals() const { return m_Globals; }
		inline const Blocks& GetBlocks() const { return m_Blocks; }
		inline const SubShader* GetSubShader(ShaderTypes sdType);
		// --setters
		// --predicates
		inline Bit IsEnabled() const { return m_bIsEnabled; }
		// --core_methods
		void Enable();
		void Disable();
		void Remake();
		virtual bool Compile() override;
		// --data_methods
		virtual bool SaveF(const char* strFPath) override;
		virtual bool LoadF(const char* strFPath) override;
		// --code_setters
		void SetBool(const char* strName, bool value) const;
		void SetInt(const char* strName, int value) const;
		void SetIntArray(const char *strName, Int32 *pIntArr, UInt32 unCount) const;
		void SetUIntArray(const char *strName, UInt32 *pUIntArr, UInt32 unCount) const;
		void SetFloat(const char* strName, float value) const;
		void SetFloatArray(const char *strName, float *pFloatArr, UInt32 unCount) const;
		void SetV2f(const char* strName, const V2f& value) const;
		void SetV3f(const char* strName, const V3f& value) const;
		void SetV4f(const char* strName, const V4f& value) const;
		void SetM4f(const char* strName, const Mat4f& value) const;
	private:
		inline bool CodeProc();
		inline Int32 GetUniformLoc(const char* strName) const;
		inline Int32 GetBlockIdx(const char* strName) const;
	private:
		mutable Bit m_bIsEnabled;
		UInt32 m_unRId;
		VertexBufLayout m_vtxLayout;
		ShaderBufLayout m_shdLayout;
		mutable Globals m_Globals;
		mutable Blocks m_Blocks;
		DArray<RefKeeper<SubShader>> m_SubShaders;
	};
	inline const SubShader* Shader::GetSubShader(ShaderTypes sdType) {
		auto itSub = NWL_FIND_BY_FUNC(m_SubShaders, RefKeeper<SubShader>&, sdType, ->GetType);
		return itSub == m_SubShaders.end() ? nullptr : itSub->GetRef();
	}
}
#endif
#if (NW_GAPI & NW_GAPI_DX)
namespace NW
{
	/// SubShader Class
	class NW_API SubShader : public ACodeRes
	{
	public:
		friend class Shader;
	public:
		SubShader(const char* strName, ShaderTypes sdType);
		virtual ~SubShader();

		// --getters
		inline UInt32 GetRenderId() const { return m_unRId; }
		inline ShaderTypes GetType() const { return m_shdType; }
		inline const Shader* GetOverShader() const { return m_pOverShader; }
		// --core_methods
		void Attach(Shader* pOverShader);
		void Detach();
		void Remake();
		virtual bool Compile() override;
		// --data_methods
		virtual bool SaveF(const char* strFPath) override;
		virtual bool LoadF(const char* strFPath) override;
	private:
		inline bool CodeProc();
	private:
		UInt32 m_unRId;
		ShaderTypes m_shdType;
		Shader* m_pOverShader;
	};
	/// Shader Class
	class NW_API Shader : public ACodeRes
	{
	public:
		using Globals = HashMap<String, Int32>;
		using Blocks = HashMap<String, Int32>;
		friend class SubShader;
	public:
		Shader(const char* strName);
		~Shader();

		// --getters
		inline UInt32 GetRenderId() const { return m_unRId; }
		inline const VertexBufLayout& GetVtxLayout() const { return m_vtxLayout; }
		inline const ShaderBufLayout& GetShdLayout() const { return m_shdLayout; }
		inline const Globals& GetGlobals() const { return m_Globals; }
		inline const Blocks& GetBlocks() const { return m_Blocks; }
		inline const SubShader* GetSubShader(ShaderTypes sdType);
		// --setters
		// --predicates
		inline Bit IsEnabled() const { return m_bIsEnabled; }
		// --core_methods
		void Enable();
		void Disable();
		void Remake();
		virtual bool Compile() override;
		// --data_methods
		virtual bool SaveF(const char* strFPath) override;
		virtual bool LoadF(const char* strFPath) override;
		// --code_setters
		void SetBool(const char* strName, bool value) const;
		void SetInt(const char* strName, int value) const;
		void SetIntArray(const char* strName, Int32* pIntArr, UInt32 unCount) const;
		void SetUIntArray(const char* strName, UInt32* pUIntArr, UInt32 unCount) const;
		void SetFloat(const char* strName, float value) const;
		void SetFloatArray(const char* strName, float* pFloatArr, UInt32 unCount) const;
		void SetV2f(const char* strName, const V2f& value) const;
		void SetV3f(const char* strName, const V3f& value) const;
		void SetV4f(const char* strName, const V4f& value) const;
		void SetM4f(const char* strName, const Mat4f& value) const;
	private:
		inline bool CodeProc();
		inline Int32 GetUniformLoc(const char* strName) const;
		inline Int32 GetBlockIdx(const char* strName) const;
	private:
		mutable Bit m_bIsEnabled;
		UInt32 m_unRId;
		VertexBufLayout m_vtxLayout;
		ShaderBufLayout m_shdLayout;
		mutable Globals m_Globals;
		mutable Blocks m_Blocks;
		DArray<RefKeeper<SubShader>> m_SubShaders;
	};
	inline const SubShader* Shader::GetSubShader(ShaderTypes sdType) {
		auto itSub = NWL_FIND_BY_FUNC(m_SubShaders, RefKeeper<SubShader>&, sdType, ->GetType);
		return itSub == m_SubShaders.end() ? nullptr : itSub->GetRef();
	}
}
#endif
#endif	// NW_GAPI
#endif // NW_GFX_SHADER_H