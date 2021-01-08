#include <nw_pch.hpp>
#include "nw_gmaterial.h"

#include <ecs/nw_scene.h>

#include <gl/vision/nw_gcamera.h>

#include <sys/nw_data_sys.h>

namespace NW
{
	// --==<GMaterial>==--
	GMaterial::GMaterial(const char* strName) :
		ADataRes(strName)
	{
		SetShader(DataSys::GetDataRes<AShader>("shd_batch_3d"));
		m_Textures[""] = DataSys::GetDataRes<ATexture2d>("tex_white_solid");
		DataSys::AddDataRes<GMaterial>(this);
	}
	GMaterial::~GMaterial() { DataSys::RemoveDataRes<GMaterial>(GetName()); }

	// --setters
	void GMaterial::SetShader(AShader* pShader) {
		m_pShader = pShader;
		m_Textures.clear();
		if (pShader == nullptr) { return; }
		for (auto& rBlock : pShader->GetShdLayout().GetBlocks()) {
			for (auto& rElem : rBlock.BufElems) {
				switch (rElem.sdType) {
				case SDT_FLOAT32:
					if (rElem.unCount == 16) {}
					break;
				case SDT_SAMPLER:
					m_Textures[rElem.strName] = DataSys::GetDataRes<ATexture2d>("tex_white_solid");
				}
			}
		}
	}
	void GMaterial::SetTexture(ATexture* pTex, const char* strType) {
		if (m_Textures.find(strType) == m_Textures.end()) { return; }
		m_Textures[&strType[0]] = pTex;
	}

	// --core_methods
	void GMaterial::Enable()
	{
		m_pShader->Enable();
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