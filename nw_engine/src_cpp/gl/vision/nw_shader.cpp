#include <nw_pch.hpp>
#include "nw_shader.h"

#include <sys/nw_data_sys.h>
#include <sys/nw_mem_sys.h>
#include <sys/nw_log_sys.h>

#if (defined NW_GRAPHICS)
#include <gl/control/nw_drawer.h>
#include <gl/vision/nw_light_source.h>
namespace NW
{
	AShader::AShader(const char* strName) :
		ACodeChunk(strName),
		m_unRId(0)
	{
	}
	AShader::~AShader()
	{
	}

	AShader* AShader::Create(const char* strName)
	{
		AShader* pShader = nullptr;
		switch (Drawer::GetGApi()->GetType()) {
	#if (NW_GRAPHICS & NW_GRAPHICS_COUT)
		case GAPI_COUT:
			pShader = MemSys::NewT<ShaderCout>(strName);
			break;
	#elif (NW_GRAPHICS & NW_GRAPHICS_OGL)
		case GAPI_OPENGL:
			pShader = MemSys::NewT<ShaderProg>(strName);
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
// ShaderOgl
namespace NW
{
	// Constructor&Destructor
	ShaderOgl::ShaderOgl(const char* strName, ShaderTypes sType) :
		AShader(strName),
		m_pShaderProg(nullptr),
		m_sType(sType)
	{
		GL_CALL(Reset());
	}
	ShaderOgl::~ShaderOgl()
	{
		Reset();
	}

	// Interface Methods
	bool ShaderOgl::Compile()
	{
		const char* cstrSource = &m_strCode[0];
		GL_CALL(glShaderSource(m_unRId, 1, &cstrSource, nullptr));
		GL_CALL(glCompileShader(m_unRId));
		if (OGL_ErrLog_Shader(m_sType, m_unRId) != 0) return false;

		return true;
	}
	void ShaderOgl::Reset()
	{
		Disable();
		if (m_unRId != 0) glDeleteShader(m_unRId);
		m_unRId = glCreateShader(m_sType);
		m_strCode = "";
	}
	void ShaderOgl::Enable()
	{
		if (m_pShaderProg)
			glAttachShader(m_pShaderProg->GetRenderId(), m_unRId);
	}
	void ShaderOgl::Disable()
	{
		if (m_pShaderProg)
			glDetachShader(m_pShaderProg->GetRenderId(), m_unRId);
	}

	// -- DataRes Methods
	bool ShaderOgl::SaveF(const char* strFPath)
	{
		return true;
	}
	bool ShaderOgl::LoadF(const char* strFPath)
	{
		return true;
	}
}
// ShaderProg
namespace NW
{
	ShaderProg::ShaderProg(const char* strName) :
		AShader(strName)
	{
		Reset();
		DataSys::AddDataRes<AShader>(this);
	}
	ShaderProg::~ShaderProg() {
		Reset();
		DataSys::RemoveDataRes<AShader>(GetName());
	}

	// -- Getters
	// -- Setters

	// ========<Interface Methods>========
	void ShaderProg::Enable()
	{
		glUseProgram(m_unRId);
	}
	void ShaderProg::Disable()
	{
		glUseProgram(0);
	}
	bool ShaderProg::Compile()
	{
		if (m_Shaders.size() != 0) { Reset(); }
		if (!SourceCodeProc()) { NW_ERR("ShaderProg is not linked"); return false; }

		for (auto& shader : m_Shaders) {
			shader.SetShaderProg(this);
			shader.Enable();
			if (!shader.Compile()) return false;
		}

		GL_CALL(glLinkProgram(m_unRId));
		if (GL_DEBUG_ERR_LOG(ST_SHADER, m_unRId) != 0) return false;
		
		return true;
	}
	void ShaderProg::Reset()
	{
		if (m_unRId != 0) glDeleteProgram(m_unRId);
		m_unRId = glCreateProgram();
		m_Shaders.clear();
		m_Attribs.clear();
		m_BufLayout.BufElems.clear();
	}

	// -- DataRes methods
	bool ShaderProg::SaveF(const char* strFPath)
	{
		String strFile = m_strCode;
		if (!DataSys::SaveF_string(strFPath, &strFile[0], sizeof(char) * strFile.size())) {
			return false;
		}
		return true;
	}
	bool ShaderProg::LoadF(const char* strFPath)
	{
		bool bSuccess = false;
		if (DataSys::LoadF_string(strFPath, m_strCode)) {	// try to load source code from file
			if (!Compile()) {									// try to use the string as a formated cn_file
				String strFile = m_strCode;
				bSuccess = false;
			}
			else { Compile(); }
		}
		return bSuccess;
	}
	// ========</Interface Methods>========

	// ========<Uniforms Setting>========
	// -- Booleans
	void ShaderProg::SetBool(const char* name, bool value) const { glUniform1i(GetUniformLoc(name), value); }
	// -- Integers
	void ShaderProg::SetInt(const char* name, int value) const { glUniform1i(GetUniformLoc(name), value); }
	void ShaderProg::SetIntArray(const char* name, Int32* pIntArr, UInt32 unCount) const { glUniform1iv(GetUniformLoc(name), unCount, pIntArr); }
	void ShaderProg::SetUIntArray(const char* name, UInt32* pUIntArr, UInt32 unCount) const { glUniform1uiv(GetUniformLoc(name), unCount, pUIntArr); }
	// -- Floats
	void ShaderProg::SetFloat(const char* name, float value) const { glUniform1f(GetUniformLoc(name), value); }
	void ShaderProg::SetFloatArray(const char* name, float* pFloatArr, UInt32 unCount) const { glUniform1fv(GetUniformLoc(name), unCount, pFloatArr); }
	void ShaderProg::SetV2f(const char* name, const V2f& value) const { glUniform2fv(GetUniformLoc(name), 1, &(value[0])); }
	void ShaderProg::SetV3f(const char* name, const V3f& value) const { glUniform3fv(GetUniformLoc(name), 1, &(value[0])); }
	void ShaderProg::SetV4f(const char* name, const V4f& value) const { glUniform4fv(GetUniformLoc(name), 1, &(value[0])); }
	void ShaderProg::SetM4f(const char* name, const Mat4f& value) const { glUniformMatrix4fv(GetUniformLoc(name), 1, GL_FALSE, &value[0][0]); }

	// -- Light Sources
	void ShaderProg::SetLight(const String& name, const DirectLight3d& dtLight) const {
		SetV3f((name + ".color").c_str(), dtLight.GetColor());
		SetV3f((name + ".direction").c_str(), dtLight.m_v3Direction);
	}
	void ShaderProg::SetLight(const String& name, const PointLight3d& ptLight) const {
		SetV3f((name + ".color").c_str(), ptLight.GetColor());
		SetV3f((name + "coord").c_str(), ptLight.m_v3Coord);
		SetFloat((name + ".atn_const").c_str(), ptLight.m_nAtn_Const);
		SetFloat((name + ".atn_linear").c_str(), ptLight.m_nAtn_Linear);
		SetFloat((name + ".atn_quad").c_str(), ptLight.m_nAtn_Quad);
	}
	void ShaderProg::SetLight(const String& name, const SpotLight3d& stLight) const {
		SetV3f((name + ".color").c_str(), stLight.GetColor());
		SetV3f((name + ".direction").c_str(), stLight.m_v3Direction);
		SetV3f((name + ".coord").c_str(), stLight.m_v3Coord);
		SetFloat((name + ".atn_const").c_str(), stLight.m_nAtn_Const);
		SetFloat((name + ".atn_linear").c_str(), stLight.m_nAtn_Linear);
		SetFloat((name + ".atn_quad").c_str(), stLight.m_nAtn_Quad);
		SetFloat((name + ".cutOff_in").c_str(), std::cosf(stLight.m_nAngleIn * 3.14f / 180.0f));
		SetFloat((name + ".cutOff_out").c_str(), std::cosf(stLight.m_nAngleOut * 3.14f / 180.0f));
	}
	// ========</Uniforms Setting>========

	// ========<Implementation Methods>========
	bool ShaderProg::SourceCodeProc()
	{
		std::stringstream strStream, strCodeStream;
		strStream << m_strCode;
		String strToken = "", strLine = "";
		UInt32 unLastUniform = 0;

		Reset();
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
			if (strToken == "vertex") {
				ShaderOgl shdVert = ShaderOgl(&(m_strName + "_vertex")[0], ShaderTypes::ST_VERTEX);
				m_Shaders.push_back(shdVert);
				while (!strCodeStream.eof()) {	// Read the entire vertex shader code
					strCodeStream >> strToken;
					if (strToken == "#version") {
						String strName = shdVert.GetName();
						strCodeStream >> strToken;
						strName += "_ver" + strToken;
						strCodeStream >> strToken;
						if (strToken == "core") strName += strToken;
						shdVert.SetName(&strName[0]);
					}
					else if (strToken == "in") {	// layout(location = {loc}) in {type} {name};
						strCodeStream >> strToken;
						if (strToken == "float") {
							m_BufLayout.AddElement(BufferElement{ SDT_FLOAT32, 1, false });
						}
						else if (strToken == "vec2") {
							m_BufLayout.AddElement(BufferElement{ SDT_FLOAT32, 2, false });
						}
						else if (strToken == "vec3") {
							m_BufLayout.AddElement(BufferElement{ SDT_FLOAT32, 3, false });
						}
						else if (strToken == "vec4") {
							m_BufLayout.AddElement(BufferElement{ SDT_FLOAT32, 4, false });
						}
						else if (strToken == "mat2") {
							m_BufLayout.AddElement(BufferElement{ SDT_FLOAT32, 2, false });
							m_BufLayout.AddElement(BufferElement{ SDT_FLOAT32, 2, false });
						}
						else if (strToken == "mat3") {
							m_BufLayout.AddElement(BufferElement{ SDT_FLOAT32, 3, false });
							m_BufLayout.AddElement(BufferElement{ SDT_FLOAT32, 3, false });
							m_BufLayout.AddElement(BufferElement{ SDT_FLOAT32, 3, false });
						}
						else if (strToken == "mat4") {
							m_BufLayout.AddElement(BufferElement{ SDT_FLOAT32, 4, false });
							m_BufLayout.AddElement(BufferElement{ SDT_FLOAT32, 4, false });
							m_BufLayout.AddElement(BufferElement{ SDT_FLOAT32, 4, false });
							m_BufLayout.AddElement(BufferElement{ SDT_FLOAT32, 4, false });
						}
					}
					else if (strToken == "uniform") {	// uniform {type} {name}[count];
						strCodeStream >> strToken;		// Get the type
						strCodeStream >> strToken;		// Get the name
						m_Attribs[&strToken[0]] = unLastUniform++;
						strCodeStream >> strToken;		// Get the count
					}
				}
			}
			else if (strToken == "geometry") {
				ShaderOgl shdGeom = ShaderOgl(&(m_strName + "_geometry")[0], ShaderTypes::ST_GEOMETRY);
				m_Shaders.push_back(shdGeom);
				while (!strCodeStream.eof()) {
					strCodeStream >> strToken;
					if (strToken == "version") {
						String strName = shdGeom.GetName();
						strCodeStream >> strToken;
						strName += "_ver" + strToken;
						strCodeStream >> strToken;
						if (strToken == "core") strName += strToken;
						shdGeom.SetName(&strName[0]);
					}
					else if (strToken =="uniform") {	// uniform {type} {name}[count];
						strCodeStream >> strToken;		// Get the type
						strCodeStream >> strToken;		// Get the name
						m_Attribs[&strToken[0]] = unLastUniform++;
						strCodeStream >> strToken;		// Get the count
					}
				}
			}
			else if (strToken == "pixel" || strToken == "fragment") {
				ShaderOgl shdPixel = ShaderOgl(&(m_strName + "_pixel")[0], ShaderTypes::ST_PIXEL);
				m_Shaders.push_back(shdPixel);
				while (!strCodeStream.eof()) {
					strCodeStream >> strToken;
					if (strToken == "#version") {
						String strName = shdPixel.GetName();
						strCodeStream >> strToken;
						strName += "_ver" + strToken;
						strCodeStream >> strToken;
						if (strToken == "core") strName += strToken;
						shdPixel.SetName(&strName[0]);
					}
					else if (strToken == "uniform") {	// uniform {type} {name}[count];
						strCodeStream >> strToken;		// Get the type
						strCodeStream >> strToken;		// Get the name
						m_Attribs[&strToken[0]] = unLastUniform++;
						strCodeStream >> strToken;		// Get the count
					}
				}
			}
			else { continue; }
			m_Shaders[m_Shaders.size() - 1].SetCode(&strCodeStream.str()[0]);
			strCodeStream = std::stringstream();
		}
		return true;
	}

	Int32 ShaderProg::GetUniformLoc(const char* uniformName) const
	{ // If we already have the same uniform name - return it's location, otherwise - add to cashe
		for (auto& name_loc : m_Attribs)
			if (uniformName == name_loc.first)
				return name_loc.second;
		//Get location, save it with appropriate name and return
		Int32 nLoc = glGetUniformLocation(m_unRId, uniformName);
		m_Attribs[uniformName] = nLoc;
		return nLoc;
	}
	// ========</Implementation Methods>========
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

	// -- Getters
	// -- Setters

	// ========<Interface Methods>========
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
	// ========</Interface Methods>========

	// ========<Implementation Methods>========
	bool ShaderCout::SourceCodeProcess()
	{
		return true;
	}
	// ========</Implementation Methods>========
}
#endif // NW_GRAPHICS