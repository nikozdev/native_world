#include <nw_pch.hpp>
#include "nw_shader.h"

#include <sys/nw_data_sys.h>
#include <sys/nw_mem_sys.h>
#include <sys/nw_log_sys.h>

#if (defined NW_GRAPHICS)
#include <glib/nw_gbuffer.h>
#include <glib/control/nw_draw_engine.h>
#include <glib/vision/nw_light_source.h>
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
		switch (DrawEngine::GetGApi()->GetType()) {
	#if (NW_GRAPHICS & NW_GRAPHICS_COUT)
		case GAPI_COUT: break;
	#elif (NW_GRAPHICS & NW_GRAPHICS_OGL)
		case GAPI_OPENGL: pSubShader = MemSys::NewT<SubShaderOgl>(strName, sdType); break;
	#endif // NW_GRAPHICS
		default: NW_ERR("Graphics Api is not defined"); break;
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
		switch (DrawEngine::GetGApi()->GetType()) {
	#if (NW_GRAPHICS & NW_GRAPHICS_COUT)
		case GAPI_COUT: break;
	#elif (NW_GRAPHICS & NW_GRAPHICS_OGL)
		case GAPI_OPENGL: pShader = MemSys::NewT<ShaderOgl>(strName); break;
	#endif // NW_GRAPHICS
		default: NW_ERR("Graphics Api is not defined"); break;
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
		m_pOverShader(nullptr) { Reset(); }
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
		if (!CodeProc()) { return false; }
		const char* strSource = &m_strCode[0];
		glShaderSource(m_unRId, 1, &strSource, nullptr);
		glCompileShader(m_unRId);
		if (OGL_ErrLog_Shader(m_shdType, m_unRId) != 0) { return false; }
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
		StrStream strCodeStream(m_strCode);
		String strToken("", 256);
		String strLine("", 256);
		String strName("", 128);
		Int32 nCurr = 0;

		while (!strCodeStream.eof()) {		// parse the entire shader code
			strCodeStream >> strToken;
			if (strToken == "#version") {	// #version %num[ %core]
				strCodeStream >> strToken;
				m_strName += "_ver" + strToken;
				strCodeStream >> strToken;
				if (strToken == "core") m_strName += strToken;
			}
			else if (strToken.find("layout") != -1) {		// layout(location=%x/%std140)in /uniform{
				if ((nCurr = strToken.find(')')) != -1) {}
				else { std::getline(strCodeStream, strToken, ')'); }
				strCodeStream >> strToken;
				if (strToken == "in") {		// layout(location=%loc)in %type %name;
					strCodeStream >> strToken;
				/// --<macro_helper>--
#define MAKE_BUF_ELEM(expr, type, count, command)				\
	if (expr) { if (nCurr < 0) { nCurr = 0; }					\
		UInt32 unCount = 1;										\
		strCodeStream >> strName;								\
		if ((nCurr = strName.find("[", nCurr)) != -1) {			\
			unCount *= atoi(&strName[nCurr + 1]);				\
			strName = strName.substr(0, nCurr);	}				\
		BufferElement BufElem(&strName[0], type, count, false);	\
		while(unCount-- > 0) { command(BufElem); } }
				/// --<macro_helper>--
					MAKE_BUF_ELEM(strToken == "bool", SDT_BOOL, 1,			m_pOverShader->m_vtxLayout.AddElement)
					MAKE_BUF_ELEM(strToken == "short", SDT_INT16, 1,		m_pOverShader->m_vtxLayout.AddElement)
					MAKE_BUF_ELEM(strToken == "int", SDT_INT32, 1,			m_pOverShader->m_vtxLayout.AddElement)
					MAKE_BUF_ELEM(strToken == "float", SDT_FLOAT32, 1,		m_pOverShader->m_vtxLayout.AddElement)
					MAKE_BUF_ELEM(strToken == "vec2", SDT_FLOAT32, 2,		m_pOverShader->m_vtxLayout.AddElement)
					MAKE_BUF_ELEM(strToken == "vec3", SDT_FLOAT32, 3,		m_pOverShader->m_vtxLayout.AddElement)
					MAKE_BUF_ELEM(strToken == "vec4", SDT_FLOAT32, 4,		m_pOverShader->m_vtxLayout.AddElement)
					if (strToken == "mat4") {
						strCodeStream >> strName;
						m_pOverShader->m_vtxLayout.AddElement(BufferElement{&strName[0], SDT_FLOAT32, 4, false});
						m_pOverShader->m_vtxLayout.AddElement(BufferElement{&strName[0], SDT_FLOAT32, 4, false});
						m_pOverShader->m_vtxLayout.AddElement(BufferElement{&strName[0], SDT_FLOAT32, 4, false});
						m_pOverShader->m_vtxLayout.AddElement(BufferElement{&strName[0], SDT_FLOAT32, 4, false});
					}
				}
				else if (strToken == "uniform") {	// uniform %name {%elements};
					strCodeStream >> strName;
					if ((nCurr = strName.find("{")) != -1) { strName = strName.substr(0, nCurr); }
					ShaderBlock shdBlock(&strName[0], m_pOverShader->m_shdLayout.GetBlocks().size());
					while (!strCodeStream.eof()) {
						strCodeStream >> strToken;
						if (strToken.find('{') != -1) { continue; }
						if (strToken.find('}') != -1) { break; }
						MAKE_BUF_ELEM(strToken == "bool", SDT_BOOL, 1, shdBlock.BufElems.push_back)
						MAKE_BUF_ELEM(strToken == "short", SDT_INT16, 1, shdBlock.BufElems.push_back)
						MAKE_BUF_ELEM(strToken == "int", SDT_INT32, 1, shdBlock.BufElems.push_back)
						MAKE_BUF_ELEM(strToken == "float", SDT_FLOAT32, 1, shdBlock.BufElems.push_back)
						MAKE_BUF_ELEM(strToken == "vec2", SDT_FLOAT32, 2, shdBlock.BufElems.push_back)
						MAKE_BUF_ELEM(strToken == "vec3", SDT_FLOAT32, 4, shdBlock.BufElems.push_back)
						MAKE_BUF_ELEM(strToken == "vec4", SDT_FLOAT32, 4, shdBlock.BufElems.push_back)
						MAKE_BUF_ELEM(strToken == "mat2", SDT_FLOAT32, 2 * 2, shdBlock.BufElems.push_back)
						MAKE_BUF_ELEM(strToken == "mat3", SDT_FLOAT32, 3 * 4, shdBlock.BufElems.push_back)
						MAKE_BUF_ELEM(strToken == "mat4", SDT_FLOAT32, 4 * 4, shdBlock.BufElems.push_back)
					}
					m_pOverShader->m_shdLayout.AddBlock(shdBlock);
					m_pOverShader->m_Blocks[&shdBlock.strName[0]] = shdBlock.unBindPoint;
				}
			}
			else if (strToken == "uniform") {	// uniform {type} {name};
				std::getline(strCodeStream, strToken, ';');
				auto fnMakeElems = [&](const char* strComp, ShaderDataTypes sdType, UInt8 unCount = 1)->bool {
					if ((nCurr = strToken.find(strComp)) != -1) {
						Int32 nCount = 1;
						Int32 nBeg = (nCurr += strlen(strComp) + 1);
						strName = strToken.substr(nBeg, strToken.size());
						if ((nCurr = strName.find("[")) != -1) {
							if (sscanf(&strName[nCurr], "[%d]", &nCount) <= 0) { return false; }
							strName = strName.substr(0, nCurr);
						}
						if ((nCurr = strName.find(" ")) != -1) { strName = strName.substr(0, nCurr - 1); }
						if (nCount > 1) {
							for (UInt32 bi = 0; bi < nCount; bi++) {
								Char strCompleteName[128];
								sprintf(&strCompleteName[0], "%s[%d]", &strName[0], bi);
								m_pOverShader->m_shdLayout.AddGlobalElem(BufferElement(&strCompleteName[0], sdType, unCount, false));
							}
						}
						else {
							m_pOverShader->m_shdLayout.AddGlobalElem(BufferElement(&strName[0], sdType, unCount, false));
						}
						return true;
					}
				};
				fnMakeElems("sampler1D", SDT_SAMPLER); fnMakeElems("sampler2D", SDT_SAMPLER); fnMakeElems("sampler3D", SDT_SAMPLER);
				fnMakeElems("bool", SDT_BOOL); fnMakeElems("char", SDT_INT8); fnMakeElems("short", SDT_INT16); fnMakeElems("int", SDT_INT32);
				fnMakeElems("float", SDT_FLOAT32); fnMakeElems("vec2", SDT_FLOAT32, 2);
				fnMakeElems("vec3", SDT_FLOAT32, 3); fnMakeElems("vec4", SDT_FLOAT32, 4);
				fnMakeElems("mat2", SDT_FLOAT32, 2 * 2); fnMakeElems("mat3", SDT_FLOAT32, 3 * 3); fnMakeElems("mat4", SDT_FLOAT32, 4 * 4);
				//m_pOverShader->m_Globals[strName] = m_pOverShader->m_Globals.size();
			}
		}
		return true;
	}
}
// ShaderOgl
namespace NW
{
	ShaderOgl::ShaderOgl(const char* strName) :
		AShader(strName) { }
	ShaderOgl::~ShaderOgl() { }

	// --==<core_methods>==--
	void ShaderOgl::Enable() {
		glUseProgram(m_unRId);
		for (UInt8 bi = 0; bi < m_shdLayout.GetBlocks().size(); bi++) {
			glUniformBlockBinding(m_unRId, bi, m_shdLayout.GetBlock(bi).unBindPoint);
		}
	}
	void ShaderOgl::Disable() { glUseProgram(0); }

	bool ShaderOgl::Compile()
	{
		Reset();
		if (!CodeProc()) { return false; }
		for (auto& rSub : m_SubShaders) {
			rSub.Attach(this);
			if (!rSub.Compile()) { return false; }
		}
		glLinkProgram(m_unRId);
		if (OGL_ErrLog_Shader(ST_SHADER, m_unRId) != 0) return false;
		
		return true;
	}
	void ShaderOgl::Reset()
	{
		if (m_unRId != 0) { glDeleteProgram(m_unRId); m_unRId = 0; }
		m_unRId = glCreateProgram();
		m_SubShaders.clear();
		m_Globals.clear();
		m_vtxLayout.Reset();
		m_shdLayout.Reset();
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
			if (!Compile()) {								// try to use the string as a formated cn_file
				String strFile = m_strCode;
				bSuccess = false;
			}
			else { bSuccess = true; }
		}
		return bSuccess;
	}
	// --==</data_methods>==--

	// --==<setters>==--
	void ShaderOgl::SetBool(const char* name, bool value) const { glUniform1i(GetUniformLoc(name), value); }
	void ShaderOgl::SetInt(const char* name, int value) const { glUniform1i(GetUniformLoc(name), value); }
	void ShaderOgl::SetIntArray(const char* name, Int32* pIntArr, UInt32 unCount) const { glUniform1iv(GetUniformLoc(name), unCount, pIntArr); }
	void ShaderOgl::SetUIntArray(const char* name, UInt32* pUIntArr, UInt32 unCount) const { glUniform1uiv(GetUniformLoc(name), unCount, pUIntArr); }
	void ShaderOgl::SetFloat(const char* name, float value) const { glUniform1f(GetUniformLoc(name), value); }
	void ShaderOgl::SetFloatArray(const char* name, float* pFloatArr, UInt32 unCount) const { glUniform1fv(GetUniformLoc(name), unCount, pFloatArr); }
	void ShaderOgl::SetV2f(const char* name, const V2f& value) const { glUniform2fv(GetUniformLoc(name), 1, &(value[0])); }
	void ShaderOgl::SetV3f(const char* name, const V3f& value) const { glUniform3fv(GetUniformLoc(name), 1, &(value[0])); }
	void ShaderOgl::SetV4f(const char* name, const V4f& value) const { glUniform4fv(GetUniformLoc(name), 1, &(value[0])); }
	void ShaderOgl::SetM4f(const char* name, const Mat4f& value) const { glUniformMatrix4fv(GetUniformLoc(name), 1, GL_FALSE, &value[0][0]); }
	// --==</setters>==--

	// --==<core_methods>==--
	inline bool ShaderOgl::CodeProc()
	{
		StrStream strStream(m_strCode), strCodeStream;
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

			if (strToken == "vertex") { m_SubShaders.push_back(SubShaderOgl(&(m_strName + "_" + strToken)[0], ST_VERTEX)); }
			else if (strToken == "geometry") { m_SubShaders.push_back(SubShaderOgl(&(m_strName + "_" + strToken)[0], ST_GEOMETRY)); }
			else if (strToken == "pixel" || strToken == "fragment") { m_SubShaders.push_back(SubShaderOgl(&(m_strName + "_" + strToken)[0], ST_PIXEL)); }
			else { continue; }

			auto& rSub = m_SubShaders.back();
			rSub.SetCode(&strCodeStream.str()[0]);
			strCodeStream = std::stringstream();
		}

		return true;
	}

	inline Int32 ShaderOgl::GetUniformLoc(const char* strName) const {
		for (auto& itPar : m_Globals) { if (strcmp(itPar.first.c_str(), strName) == 0) { return itPar.second; } }
		
		Int32 nLoc = glGetUniformLocation(m_unRId, strName);
		m_Globals[strName] = nLoc;
		return nLoc;
	}
	inline Int32 ShaderOgl::GetBlockIdx(const char* strName) const {
		for (auto& itBlk : m_Blocks) { if (strcmp(itBlk.first.c_str(), strName) == 0) { return itBlk.second; } }
		
		Int32 nIdx = glGetUniformBlockIndex(m_unRId, &strName[0]);
		m_Blocks[strName] = nIdx;
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