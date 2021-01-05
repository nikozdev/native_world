#ifndef NW_ASHADER_H
#define NW_ASHADER_H

#include <gl/nw_gl_core.h>

#include <lib/utils/nw_code_chunk.h>

#include <lib/utils/math_vector.h>
#include <lib/utils/math_matrix.h>

#if (defined NW_GRAPHICS)
namespace NW
{
	/// Abstract Shader Class
	/// --Interface:
	/// -> Create -> AddSource Code with defined type -> Compile/Link
	/// -> Enable -> {Render} -> Disable
	/// --It is abstraction of current graphical API for GPU programms
	/// --So GPU can do any work without implicit API specification in the engine
	///	Shader Source Code loading rules:
	/// --> Load string as a source code(can be loaded a from file throught loadMaster)
	/// --> Load the source code with the beginning "#shader type" on the top of the particular shader
	/// --> SetSource of entire file with the appropriate type which is written there
	/// --> It is allowed to write shader code in separate and in the whole file.
	/// --> The code string has to be finished as "#shader type" or just "\0"(end of string)
	/// Future:
	/// -> Shader code preprocessing detects specific uniforms (lights/transform_matricies/...)
	/// And uses them for setting without giving a particular sstrName
	class NW_API AShader : public ACodeChunk
	{
	public:
		using Attribs = HashMap<const char*, UInt32>;
	public:
		AShader(const char* strName);
		virtual ~AShader();

		// -- Getters
		UInt32 GetRenderId() const { return m_unRId; }
		inline const BufferLayout& GetBufferLayout() { return m_BufLayout; }
		virtual inline Attribs& GetAttribs() { return m_Attribs; }
		// -- Setters

		// -- Interface methods
		virtual void Enable() = 0;
		virtual void Disable() = 0;
		virtual bool Compile() = 0;
		virtual void Reset() = 0;
		// -- DataRes methods
		virtual bool SaveF(const char* strFPath) = 0;
		virtual bool LoadF(const char* strFPath) = 0;

		// --Attributes&Settings
		virtual void SetBool(const char* strName, bool value) const {}
		virtual void SetInt(const char* strName, int value) const {}
		virtual void SetIntArray(const char *strName, Int32 *pIntArr, UInt32 unCount) const {}
		virtual void SetUIntArray(const char *strName, UInt32 *pUIntArr, UInt32 unCount) const {}

		virtual void SetFloat(const char* strName, float value) const {}
		virtual void SetFloatArray(const char *strName, float *pFloatArr, UInt32 unCount) const {}
		virtual void SetV2f(const char* strName, const V2f& value) const {}
		virtual void SetV3f(const char* strName, const V3f& value) const {}
		virtual void SetV4f(const char* strName, const V4f& value) const {}

		virtual void SetM4f(const char* strName, const Mat4f& value) const {}
		// -- Light Sources
		virtual void SetLight(const String& strName, const DirectLight3d& drLight) const {};
		virtual void SetLight(const String& strName, const PointLight3d& ptLight) const {};
		virtual void SetLight(const String& strName, const SpotLight3d& stLight) const {};
		// --Attributes&Settings
		
		static AShader* Create(const char* strName);
	protected:
		UInt32 m_unRId;
		BufferLayout m_BufLayout;
		mutable Attribs m_Attribs;
	};
}
#endif	// NW_GRAPHICS
#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
// Shader
namespace NW
{
	class NW_API ShaderProg;
	/// Shader class OpenGL abstraction
	/// Interface:
	/// -> Create the shader
	/// -> Set the entire shader source code with written comment "#shader type" on the top
	/// -> Compile(Load) -> Set the shaderProgram -> link that ShaderProgram
	class NW_API ShaderOgl : public AShader
	{
	public:
		ShaderOgl(const char* strName, ShaderTypes sType);
		~ShaderOgl();
		
		// -- Setters
		inline void SetShaderProg(ShaderProg* pShaderProg) { Disable(); m_pShaderProg = pShaderProg; }

		// -- Interface Methods
		virtual void Enable() override;
		virtual void Disable() override;
		virtual bool Compile() override;
		virtual void Reset() override;
		// -- DataRes methods
		virtual bool SaveF(const char* strFPath) override;
		virtual bool LoadF(const char* strFPath) override;
	private: // Implementation Attributes
		ShaderProg* m_pShaderProg;
		ShaderTypes m_sType;
	};
	/// Shader class - handler for shader program OpenGL
	/// Interface:
	/// -> Create ->
	/// -> Set the source code for both shader types: vertex and fragment shader
	/// -> Load the program
	/// Description
	/// -- Instead of own source code, shader programm has shader objects with that code
	class NW_API ShaderProg : public AShader
	{
	public: // Interface Methods
		ShaderProg(const char* strName);
		~ShaderProg();

		// -- Interface Methods
		virtual void Enable() override;
		virtual void Disable() override;
		virtual bool Compile() override;
		virtual void Reset() override;
		// -- DataRes Methods
		virtual bool SaveF(const char* strFPath) override;
		virtual bool LoadF(const char* strFPath) override;

		// --Attributes&Settings
		virtual void SetBool(const char* strName, bool value) const override;

		virtual void SetInt(const char* strName, int value) const override;
		virtual void SetIntArray(const char* strName, Int32* pIntArr, UInt32 unCount) const override;
		virtual void SetUIntArray(const char* strName, UInt32* pIntArr, UInt32 unCount) const override;

		virtual void SetFloat(const char* strName, float value) const override;
		virtual void SetFloatArray(const char* strName, float* pFloatArr, UInt32 unCount) const override;
		virtual void SetV2f(const char* strName, const V2f& value) const override;
		virtual void SetV3f(const char* strName, const V3f& value) const override;
		virtual void SetV4f(const char* strName, const V4f& value) const override;

		void SetM4f(const char* strName, const Mat4f& value) const override;

		// -- Light Sources
		virtual void SetLight(const String& strName, const DirectLight3d& drLight) const override;
		virtual void SetLight(const String& strName, const PointLight3d& ptLight) const override;
		virtual void SetLight(const String& strName, const SpotLight3d& stLight) const override;
	private: // Implementation Attributes
		DArray<ShaderOgl> m_Shaders;
	private: // Implementation Methods
		/// We have gotten a whole source code file
		/// Iterate throught all the lines in that code
		/// If it suits the format:
		/// Create new ShaderOgl of appropriate type in the own vector, set it's source
		bool SourceCodeProc();
		// Optimization
		/// Gets uniforms from the program, or from the uniforms cashe if they are there
		inline Int32 GetUniformLoc(const char* uniformName) const;
	};
}
#endif // NW_GRAPHICS
#if (NW_GRAPHICS & NW_GRAPHICS_COUT)
strNamespace CN
{
	/// Shader class - handler for shader program OpenGL
	/// Interface:
	/// -> Create ->
	/// -> Set the source code for both shader types: vertex and fragment shader
	/// -> Load the program
	/// Description
	/// -- Instead of own source code, shader programm has shader objects with that code
	class NW_API ShaderCout : public AShader
	{
	public: // Interface Methods
		ShaderCout(const String& strName);
		~ShaderCout();

		// Getters
		virtual UInt GetRenderId() const { return m_unRId; }
		virtual const String& GetstrName() const override
		{
			return m_strName;
		}
		virtual inline const String& GetSource() const override
		{
			return m_strSource;
		}
		// Setters
		virtual void SetstrName(const String& strName) { m_strName = strName; }
		virtual void SetSource(const String& shaderCode) {
			if (m_strSource != "") Reset();
			m_strSource = shaderCode;
		}

		// Interface Methods
		virtual void Enable() override;
		virtual void Disable() override;

		virtual bool Setup() override;
		virtual void Reset() override;

		// --Attributes&Settings
		void SetBool(const char* strName, bool value) const override;
		void SetInt(const char* strName, int value) const override;
		void SetFloat(const char* strName, float value) const override;

		void SetV2f(const char* strName, const V2f& value) const override;
		void SetV3f(const char* strName, const V3f& value) const override;
		void SetV4f(const char* strName, const V4f& value) const override;

		void SetM4f(const char* strName, const Mat4f& value) const override;

		// Light Sources
		virtual void SetLight(const String& strName, const DirectLight3d& drLight) const override;
		virtual void SetLight(const String& strName, const PointLight3d& ptLight) const override;
		virtual void SetLight(const String& strName, const SpotLight3d& stLight) const override;
		// --Attributes&Settings
	private: // Implementation Attributes
		UInt m_unRId;
		String m_strName;
		String m_strSource;
	private: // Implementation Methods
		bool SourceCodeProcess();
	};
}
#endif // NW_GRAPHICS

#endif // NW_ASHADER_H