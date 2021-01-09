#include <nw_pch.hpp>
#include "nw_gmaterial.h"

#include <glib/nw_gbuffer.h>
#include <glib/render/nw_texture.h>
#include <glib/vision/nw_shader.h>

#include <sys/nw_data_sys.h>

namespace NW
{
	// --==<GMaterial>==--
	GMaterial::GMaterial(const char* strName) :
		ADataRes(strName)
	{
		SetShader(DataSys::GetDataRes<AShader>("shd_batch_3d"));
		DataSys::AddDataRes<GMaterial>(this);
	}
	GMaterial::~GMaterial() { DataSys::RemoveDataRes<GMaterial>(GetName()); }

	// --setters
	void GMaterial::SetShader(AShader* pShader) {
		m_pShader = pShader;
		m_Textures.clear();
		m_Colors.clear();

		if (pShader != nullptr) {
			for (auto& itGlob : pShader->GetShdLayout().GetGlobals()) {
				switch (itGlob.sdType) {
				case SDT_FLOAT32: if (itGlob.unCount == 4) { m_Colors[itGlob.strName] = V4f{ 1.0f, 1.0f, 1.0f, 1.0f }; } break;
				case SDT_SAMPLER: m_Textures[itGlob.strName] = DataSys::GetDataRes<ATexture2d>("tex_white_solid"); break;
				}
			}
		}
		if (m_Textures.empty()) { m_Textures[""] = DataSys::GetDataRes<ATexture2d>("tex_white_solid"); }
		if (m_Colors.empty()) { m_Colors[""] = V4f{ 1.0f, 1.0f, 1.0f, 1.0f }; }
	}
	void GMaterial::SetTexture(ATexture* pTex, const char* strType) {
		if (pTex == nullptr) { return; }
		if (strcmp(strType, "") == 0) { m_Textures.begin()->second = pTex; }
		if (m_Textures.find(strType) != m_Textures.end()) { m_Textures[strType] = pTex; }
		else { m_pShader->SetInt(strType, pTex->GetTexSlot()); }
	}
	void GMaterial::SetColor(const V4f& rgbaClr, const char* strType) {
		if (strcmp(strType, "") == 0) { m_Colors.begin()->second = rgbaClr; }
		if (m_Colors.find(strType) == m_Colors.end()) {	m_Colors[strType] = rgbaClr; }
		else { m_pShader->SetV4f(strType, rgbaClr); }
	}

	// --core_methods
	void GMaterial::Enable()
	{
		m_pShader->Enable();
		for (auto itClr : m_Colors) { m_pShader->SetV4f(&itClr.first[0], itClr.second); }
		auto itTex = m_Textures.begin();
		for (UInt8 txi = 0; txi < m_Textures.size(); txi++) {
			itTex->second->Bind(txi);
			m_pShader->SetInt(&itTex->first[0], itTex->second->GetTexSlot());
			itTex++;
		}
	}
	void GMaterial::Disable()
	{
		m_pShader->Disable();
	}

	// --data_methods
	bool GMaterial::SaveF(const char* strFPath)
	{
		return true;
	}
	bool GMaterial::LoadF(const char* strFPath)
	{
		return true;
	}
	// --==</GMaterial>==--
}