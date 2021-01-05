#include <nw_pch.hpp>
#include "nw_gmaterial.h"

#include <ecs/nw_scene.h>

#include <gl/vision/nw_gcamera.h>

#include <sys/nw_data_sys.h>

namespace NW
{
	// ========<AGMaterial>========
	AGMaterial::AGMaterial(const char* strName) :
		ADataRes(strName)
	{
		DataSys::AddDataRes<AGMaterial>(this);
	}
	AGMaterial::~AGMaterial() { DataSys::RemoveDataRes<AGMaterial>(GetName()); }
	// ========</AGMaterial>========
	
	// ========<GMaterial2d>========
	GMaterial2d::GMaterial2d(const char* strName) :
		AGMaterial(strName)
	{
		SetTexture(DataSys::GetDataRes<ATexture2d>("tex_white_solid"));
	}
	GMaterial2d::~GMaterial2d()
	{
	}

	// -- Core Methods
	void GMaterial2d::Enable()
	{
		//
	}
	void GMaterial2d::Disable()
	{
		//
	}

	// -- Data Methods
	bool GMaterial2d::SaveF(const char* strFPath)
	{
		return true;
	}
	bool GMaterial2d::LoadF(const char* strFPath)
	{
		return true;
	}
	// ========</GMaterial2d>========
}