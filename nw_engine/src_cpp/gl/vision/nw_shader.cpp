#include <nw_pch.hpp>
#include "nw_shader.h"

#include <sys/nw_data_sys.h>
#include <sys/nw_mem_sys.h>
#include <sys/nw_log_sys.h>

#if (defined NW_GRAPHICS)
#include <core/nw_graph_engine.h>
#include <gl/vision/nw_light_source.h>
namespace NW
{
	ASubShader::ASubShader(const char* strName, ShaderTypes sdType) :
		ACodeChunk(strName),
		m_unRId(0), m_shdType(sdType) {
		DataSys::AddDataRes<ASubShader>(this);
	}
	ASubShader::~ASubShader() { DataSys::RemoveDataRes<ASubShader>(GetName()); }
	ASubShader* ASubShader::Create(const char* strName, ShaderTypes sdType)
	{
		ASubShader* pSubShader = nullptr;
		switch (GraphEngine::GetGApi()->GetType()) {
	#if (NW_GRAPHICS & NW_GRAPHICS_COUT)
		case GAPI_COUT:
			pSubShader = MemSys::NewT<ShaderCout>(strName, sdType);
			break;
	#elif (NW_GRAPHICS & NW_GRAPHICS_OGL)
		case GAPI_OPENGL:
			pSubShader = MemSys::NewT<SubShaderOgl>(strName, sdType);
			break;
	#endif // NW_GRAPHICS
		default:
			NW_ERR("Graphics Api is not defined");
		}
		return pSubShader;
	}
}
namespace NW
{
	AShader::AShader(const char* strName) :
		ACodeChunk(strName),
		m_unRId(0) { DataSys::AddDataRes<AShader>(this); }
	AShader::~AShader() { DataSys::RemoveDataRes<AShader>(GetName()); }

	AShader* AShader::Create(const char* strName)
	{
		AShader* pShader = nullptr;
		switch (GraphEngine::GetGApi()->GetType()) {
	#if (NW_GRAPHICS & NW_GRAPHICS_COUT)
		case GAPI_COUT:
			pShader = MemSys::NewT<ShaderCout>(strName);
			break;
	#elif (NW_GRAPHICS & NW_GRAPHICS_OGL)
		case GAPI_OPENGL:
			pShader = MemSys::NewT<ShaderOgl>(strName);
			break;
	#endif // NW_GRAPHICS
		default:
			NW_ERR("Graphics Api is not defined");
		}
		return pShader;
	}
}
#endif	// NW_GRAPHICS
#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
#include <glad/glad.h>
// SubShaderOgl
namespace NW
{
	// Constructor&Destructor
	SubShaderOgl::SubShaderOgl(const char* strName, ShaderTypes sdType) :
		ASubShader(strName, sdType),
		m_pOverShader(nullptr) { m_unRId = glCreateShader(m_shdType); }
	SubShaderOgl::~SubShaderOgl(){ Reset(); }

	// getters
	const AShader* SubShaderOgl::GetOverShader() const { return dynamic_cast<const AShader*>(m_pOverShader); }
	// core_methods
	void SubShaderOgl::Attach(AShader* pOverShader) {
		Detach();
		m_pOverShader = dynamic_cast<ShaderOgl*>(pOverShader);
		if (m_pOverShader == nullptr) return;
		glAttachShader(m_pOverShader->GetRenderId(), m_unRId);
	}
	void SubShaderOgl::Detach() { if (m_pOverShader) { glDetachShader(m_pOverShader->GetRenderId(), m_unRId); } }

	bool SubShaderOgl::Compile()
	{
		const char* cstrSource = &m_strCode[0];
		glShaderSource(m_unRId, 1, &cstrSource, nullptr);
		glCompileShader(m_unRId);
		if (OGL_ErrLog_Shader(m_shdType, m_unRId) != 0) return false;
		return true;
	}
	void SubShaderOgl::Reset() {
		Detach();
		if (m_unRId != 0) { glDeleteShader(m_unRId); m_unRId = 0; }
		m_unRId = glCreateShader(m_shdType);
		m_strCode = "";
	}

	// -- DataRes Methods
	bool SubShaderOgl::SaveF(const char* strFPath)
	{
		return true;
	}
	bool SubShaderOgl::LoadF(const char* strFPath)
	{
		return true;
	}

	// -- Implementation Methods
	bool SubShaderOgl::CodeProc() {
		StringStream strCodeStream(m_strCode);
		String strToken("", 256);
		String strLine("", 256);

		while (!strCodeStream.eof()) {	// Read the entire shader code
			strCodeStream >> strToken;
			if (strToken == "#version") {	// #version {ver_num}[ core]
				strCodeStream >> strToken;
				m_strName += "_ver" + strToken;
				strCodeStream >> strToken;
				if (strToken == "core") m_strName += strToken;
			}
			else if (strToken.find("layout") != -1) {		// layout(...) )in/)uniform {
				std::getline(strCodeStream, strToken, ')');	// everything in there doesn't really matter
				strCodeStream >> strToken;
				Int32 nCurr = 0;
				if (strToken == "in") {		// layout(location = {loc}) in {type} {name};
					strCodeStream >> strToken;
					String strName("", 128);
				/// --<macro_helper>--
#define MAKE_BUF_ELEM(expr, type, count)										\
	if (expr) { if (nCurr < 0) { nCurr = 0; }	strCodeStream >> strName;		\
	BufferElement BufElem(&strName[0], type, count, false);						\
	if ((nCurr = strToken.find("[", nCurr)) != -1)								\
	{ BufElem.unCount *= atoi(&strToken[nCurr]); }								\
	m_pOverShader->m_vtxLayout.AddElement(BufElem); }
				/// --<macro_helper>--
					MAKE_BUF_ELEM(strToken == "bool", SDT_BOOL, 1)
					MAKE_BUF_ELEM(strToken == "short", SDT_INT16, 1)
					MAKE_BUF_ELEM(strToken == "int", SDT_INT32, 1)
					MAKE_BUF_ELEM(strToken == "sampler1D" || strToken == "sampler2D" || strToken == "sampler3D", SDT_INT32, 1)
					MAKE_BUF_ELEM(strToken == "float", SDT_FLOAT32, 1)
					MAKE_BUF_ELEM(strToken == "vec2", SDT_FLOAT32, 2)
					MAKE_BUF_ELEM(strToken == "vec3", SDT_FLOAT32, 3)
					MAKE_BUF_ELEM(strToken == "vec4", SDT_FLOAT32, 4)
					MAKE_BUF_ELEM(strToken == "mat2", SDT_FLOAT32, 2 * 2)
					MAKE_BUF_ELEM(strToken == "mat3", SDT_FLOAT32, 3 * 3)
					MAKE_BUF_ELEM(strToken == "mat4", SDT_FLOAT32, 4 * 4)
				}
				else if (strToken == "uniform") {	// uniform {name} {elements};
					m_pOverShader->m_Blocks[strToken] = m_pOverShader->m_Blocks.size();
				}
			}
			else if (strToken == "uniform") {	// uniform {type} {name};
				Int32 nCurr = 0;
				strCodeStream >> strToken;
				std::getline(strCodeStream, strToken, '\n');
				if (strToken.find(';') != -1) {		// simple uniform
					String strName = strToken.substr(0, nCurr - 2);
					if ((nCurr = strToken.find("[")) == -1) {	// it is not array
						m_pOverShader->m_Attribs[strName] = m_pOverShader->m_Attribs.size();
					}
					else {		// here is an array
						Int32 nCount = atoi(&strToken[nCurr + 1]);
						m_pOverShader->m_Attribs[strName] = m_pOverShader->m_Attribs.size();
						m_pOverShader->m_shdLayout.AddElement(&strName[0], SDT_SAMPLER, 1, nCount);
					}
				}
			}
		}
		return true;
	}
}
// ShaderOgl
namespace NW
{
	ShaderOgl::ShaderOgl(const char* strName) :
		AShader(strName)
	{ m_unRId = glCreateProgram(); }
	ShaderOgl::~ShaderOgl() { }

	// --==<core_methods>==--
	void ShaderOgl::Enable() {
		glUseProgram(m_unRId);
		auto itBlock = m_Blocks.begin();
		for (Int32 nBind = 0; itBlock != m_Blocks.end(); nBind++, itBlock++) {
			glUniformBlockBinding(m_unId, itBlock->second, nBind);
		}
	}
	void ShaderOgl::Disable() { glUseProgram(0); }

	bool ShaderOgl::Compile()
	{
		if (m_SubShaders.size() != 0) { Reset(); }
		if (!CodeProc()) { NW_ERR("ShaderOgl is not linked"); return false; }

		GL_CALL(glLinkProgram(m_unRId));
		if (GL_DEBUG_ERR_LOG(ST_SHADER, m_unRId) != 0) return false;

		return true;
	}
	void ShaderOgl::Reset()
	{
		if (m_unRId != 0) glDeleteProgram(m_unRId);
		m_unRId = glCreateProgram();
		m_SubShaders.clear();
		m_Attribs.clear();
		m_vtxLayout.Reset();
	}
	// --==</core_methods>==--

	// --==<data_methods>==--
	bool ShaderOgl::SaveF(const char* strFPath)
	{
		String strFile = m_strCode;
		if (!DataSys::SaveF_string(strFPath, &strFile[0], sizeof(char) * strFile.size())) {
			return false;
		}
		return true;
	}
	bool ShaderOgl::LoadF(const char* strFPath)
	{
		bool bSuccess = false;
		if (DataSys::LoadF_string(strFPath, m_strCode)) {	// try to load source code from file
			if (!Compile()) {									// try to use the string as a formated cn_file
				String strFile = m_strCode;
				bSuccess = false;
			}
			else { bSuccess = false; }
		}
		return bSuccess;
	}
	// --==</data_methods>==--

	// --==<setters>==--
	void ShaderOgl::SetBool(const char* name, bool value) const { glUniform1i(GetAttribLoc(name), value); }
	void ShaderOgl::SetInt(const char* name, int value) const { glUniform1i(GetAttribLoc(name), value); }
	void ShaderOgl::SetIntArray(const char* name, Int32* pIntArr, UInt32 unCount) const { glUniform1iv(GetAttribLoc(name), unCount, pIntArr); }
	void ShaderOgl::SetUIntArray(const char* name, UInt32* pUIntArr, UInt32 unCount) const { glUniform1uiv(GetAttribLoc(name), unCount, pUIntArr); }
	void ShaderOgl::SetFloat(const char* name, float value) const { glUniform1f(GetAttribLoc(name), value); }
	void ShaderOgl::SetFloatArray(const char* name, float* pFloatArr, UInt32 unCount) const { glUniform1fv(GetAttribLoc(name), unCount, pFloatArr); }
	void ShaderOgl::SetV2f(const char* name, const V2f& value) const { glUniform2fv(GetAttribLoc(name), 1, &(value[0])); }
	void ShaderOgl::SetV3f(const char* name, const V3f& value) const { glUniform3fv(GetAttribLoc(name), 1, &(value[0])); }
	void ShaderOgl::SetV4f(const char* name, const V4f& value) const { glUniform4fv(GetAttribLoc(name), 1, &(value[0])); }
	void ShaderOgl::SetM4f(const char* name, const Mat4f& value) const { glUniformMatrix4fv(GetAttribLoc(name), 1, GL_FALSE, &value[0][0]); }
	// --==</setters>==--

	// --==<core_methods>==--
	bool ShaderOgl::CodeProc()
	{
		StringStream strStream(m_strCode), strCodeStream;
		String strToken = "", strLine = "";

		if (m_SubShaders.size() > 0) { Reset(); }
		while (strToken != "#shader_type" && !strStream.eof()) { strStream >> strToken; }
		while (!strStream.eof()) {	// Last token has to be the type: "#shader_type|{type}"
			strStream >> strToken;
			while (!strStream.eof()) {
				strStream >> strLine;						// write a type token from strStream 
				if (strLine == "#shader_type") { break; }	// if it's a new shader - stop
				else {	//	-- write that token in the code and all other line too
					strCodeStream << strLine;
					std::getline(strStream, strLine, '\n');
					strCodeStream << strLine << '\n';
				}
			}

			if (strToken == "vertex") { m_SubShaders.push_back(SubShaderOgl(&(m_strName + "_" + strToken)[0], ShaderTypes::ST_VERTEX)); }
			else if (strToken == "geometry") { m_SubShaders.push_back(SubShaderOgl(&(m_strName + "_" + strToken)[0], ShaderTypes::ST_GEOMETRY)); }
			else if (strToken == "pixel" || strToken == "fragment") { m_SubShaders.push_back(SubShaderOgl(&(m_strName + "_" + strToken)[0], ShaderTypes::ST_PIXEL)); }
			else { continue; }

			auto& rSub = m_SubShaders.back();
			rSub.SetCode(&strCodeStream.str()[0]);
			rSub.Attach(this);
			if (!rSub.CodeProc()) { return false; }
			if (!rSub.Compile()) { return false; }
			
			strCodeStream = std::stringstream();
		}

		return true;
	}

	Int32 ShaderOgl::GetAttribLoc(const char* strName) const
	{
		for (auto& name_loc : m_Attribs) { if (&name_loc.first[0] == strName) { return name_loc.second; } }
		
		Int32 nLoc = glGetAttribLocation(m_unRId, &strName[0]);
		m_Attribs[&strName[0]] = nLoc;
		return nLoc;
	}
	inline Int32 ShaderOgl::GetBlockIdx(const char* strName) const
	{
		for (auto& name_loc : m_Blocks) { if (&name_loc.first[0] == strName) { return name_loc.second; } }
		
		Int32 nIdx = glGetUniformBlockIndex(m_unRId, &strName[0]);
		m_Attribs[&strName[0]] = nIdx;
		return nIdx;
	}
	// --==</core_methods>==--
}

#endif // NW_GRAPHICS
#if (NW_GRAPHICS & NW_GRAPHICS_COUT)
#include <app_ecs/cn_ALightSource.h>

#include <math/math_mtx.h>

namespace NW
{
	ShaderCout::ShaderCout(const std::string& name) :
		m_unRId(0), m_strName(name)
	{
		Reset();
	}
	ShaderCout::~ShaderCout()
	{
		Reset();
	}

	// --getters
	// --setters

	// --==<Interface Methods>==--
	void ShaderCout::Enable()
	{
	}
	void ShaderCout::Disable()
	{
	}
	bool ShaderCout::Setup()
	{
		if (!SourceCodeProcess());
		return false;

		return true;
	}
	void ShaderCout::Reset()
	{
	}

	// -- Attributes&Settings
	void ShaderCout::SetBool(const char* name, bool value) const
	{
	}
	void ShaderCout::SetInt(const char* name, int value) const
	{
	}
	void ShaderCout::SetFloat(const char* name, float value) const
	{
	}
	void ShaderCout::SetV2f(const char* name, const V2f& value) const
	{
	}
	void ShaderCout::SetV3f(const char* name, const V3f& value) const
	{
	}
	void ShaderCout::SetV4f(const char* name, const V4f& value) const
	{
	}
	void ShaderCout::SetM4f(const char* name, const Mat4f& value) const
	{
	}

	// -- Light Sources
	void ShaderCout::SetLight(const String& name, const DirectLight3d& dtLight) const
	{
		SetV3f((name + ".color").c_str(), dtLight.GetColor());
		SetV3f((name + ".direction").c_str(), dtLight.m_v3Direction);
	}
	void ShaderCout::SetLight(const String& name, const PointLight3d& ptLight) const
	{
		SetV3f((name + ".color").c_str(), ptLight.GetColor());
		SetV3f((name + "coord").c_str(), ptLight.m_v3Coord);
		SetFloat((name + ".atn_const").c_str(), ptLight.m_nAtn_Const);
		SetFloat((name + ".atn_linear").c_str(), ptLight.m_nAtn_Linear);
		SetFloat((name + ".atn_quad").c_str(), ptLight.m_nAtn_Quad);
	}
	void ShaderCout::SetLight(const String& name, const SpotLight3d& stLight) const
	{
		SetV3f((name + ".color").c_str(), stLight.GetColor());
		SetV3f((name + ".direction").c_str(), stLight.m_v3Direction);
		SetV3f((name + ".coord").c_str(), stLight.m_v3Coord);
		SetFloat((name + ".atn_const").c_str(), stLight.m_nAtn_Const);
		SetFloat((name + ".atn_linear").c_str(), stLight.m_nAtn_Linear);
		SetFloat((name + ".atn_quad").c_str(), stLight.m_nAtn_Quad);
		SetFloat((name + ".cutOff_in").c_str(), glm::cos(glm::radians(stLight.m_nAngleIn)));
		SetFloat((name + ".cutOff_out").c_str(), glm::cos(glm::radians(stLight.m_nAngleOut)));
	}
	// --==</Interface Methods>==--

	// --==<Implementation Methods>==--
	bool ShaderCout::SourceCodeProcess()
	{
		return true;
	}
	// --==</Implementation Methods>==--
}
#endif // NW_GRAPHICS