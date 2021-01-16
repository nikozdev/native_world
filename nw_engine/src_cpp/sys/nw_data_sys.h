#ifndef NW_DATA_SYSTEM_H
#define NW_DATA_SYSTEM_H
#include <core/nw_data_res.h>

#include <nw_decl.hpp>
#include <ecs_decl.hpp>
#include <glib_decl.hpp>

namespace NW
{
	/// DataSys ASingleton class
	/// -- Can load any CN format
	/// -- Saves and loads binary files of accesible format
	class NW_API DataSys
	{
	public:
		using FStream = std::fstream;
		using ADRs = HashMap<UInt32, ADataRes*>;
		template <class DRType> using DRs = HashMap<UInt32, DRType*>;
	public:
		// --getters
		static inline const char* GetDirectory() { return &s_strRscDir[0]; }
		
		static inline ADRs& GetADataResources() { return s_ADRs; }
		static inline ADataRes* GetADataRes(UInt32 unId);

		template <class DRType> static inline DRs<DRType>& GetDataResources() { static DRs<DRType> s_DRs; return s_DRs; }
		template <class DRType> static inline DRType* GetDataRes(UInt32 unId);
		template <class DRType> static inline DRType* GetDataRes(const char* strName);
		// --setters
		static void SetDirectory(const char* strDir);
		static void AddADataRes(ADataRes* pDataRes);
		static void RmvADataRes(UInt32 unId);
		
		template <class DRType> static inline void AddDataRes(DRType* pDataRes);
		template <class DRType> static inline void RmvDataRes(UInt32 unId);
		template <class DRType> static inline void RmvDataRes(const char* strName);
		
		// --core_methods
		static bool OnInit();
		static void OnQuit();
		// --file_dialogs
		static String FDialog_save(const char* strFilter);
		static String FDialog_load(const char* strFilter);
		// --binary_data
		static bool SaveF_data(const char *strFPath, void* pData, UInt64 unBytes);
		static bool SaveF_data(const char *directory, const char *name, const char *noPointformat,
			void* pData, UInt64 unBytes);
		static inline bool SaveF_data(void* pData, UInt64 unBytes) { return SaveF_data(&FDialog_save("all_files(*.*)\0*.*\0")[0], pData, unBytes); }
		static bool LoadF_data(const char *strFPath, void* pData, UInt64 unBytes);
		static bool LoadF_data(const char *directory, const char *name, const char *noPointformat,
			void *pData, UInt64 unBytes);
		static inline bool LoadF_data(void* pData, UInt64 unBytes) { return LoadF_data(&FDialog_load("all_files(*.*)\0*.*\0")[0], pData, unBytes); }
		// --strings
		static bool SaveF_string(const char* strFPath, const char* strSrc, UInt64 unBytes);
		static bool LoadF_string(const char* strFPath, String& strDest);
		// --images
		static UInt8* LoadF_image(const char* strFPath, Int32* pnW, Int32* pnH, Int32* pnChannels);
		static bool LoadF_image(const char* strFPath, UByte* pClrDataBuf, Int32* pnW, Int32* pnH, Int32* pnChannels);
		static bool LoadF_image(const char* strFilePath, ImageInfo* pImage);
		// --meshes
		static bool SaveF_mesh(const String& strFilePath, DArray<VertexShape3d>* pVtxData, DArray<UInt32>* punIndData);
		static bool LoadF_mesh(const String& strFilePath, DArray<VertexShape3d>* pVtxData, DArray<UInt32>* punIndData);
		// --shaders
	private:
		static ADRs s_ADRs;
		static String s_strRscDir;
	private:
		static bool LoadF_mesh_dae(const String& strFileData, DArray<UInt32>& arrIndices,
			DArray<V3f>& vtxCoords, DArray<V2f>& texCoords, DArray<V3f>& normalCoords);
		static bool LoadF_mesh_dae(const String& strFileData, DArray<UInt32>& arrIndices,
			DArray<float>& vtxCoords, DArray<float>& texCoords, DArray<float>& normalCoords);
		static bool LoadF_mesh_obj(const String& strFileData, DArray<UInt32>& arrIndices,
			DArray<V3f>& vtxCoords, DArray<V2f>& texCoords, DArray<V3f>& normalCoords);
		static bool LoadF_mesh_obj(const String& strFileData, DArray<UInt32>& arrIndices,
			DArray<float>& vtxCoords, DArray<float>& texCoords, DArray<float>& normalCoords);
		static bool LoadF_mesh_obj(const String& strFileData, DArray<VertexShape3d>& rVtxData, DArray<UInt32>& runIndData);
	};
	// --implementation
	inline ADataRes* DataSys::GetADataRes(UInt32 unId) {
		ADRs::iterator itDR = s_ADRs.find(unId);
		return itDR == s_ADRs.end() ? nullptr : itDR->second;
	}
	template <class DRType> inline DRType* DataSys::GetDataRes(UInt32 unId) {
		return static_cast<DRType*>(GetADataRes(unId));
	}
	template <class DRType> inline DRType* DataSys::GetDataRes(const char* strName) {
		DRs<DRType>& s_DRs = GetDataResources<DRType>();
		DRs<DRType>::iterator itDR = std::find_if(s_DRs.begin(), s_DRs.end(),
			[=](std::pair<const UInt32, DRType*>& rObj)->bool {return strcmp(&rObj.second->GetName()[0], strName) == 0; });
		return itDR == s_DRs.end() ? s_DRs.begin()->second : itDR->second;
	}
	template <class DRType> inline void DataSys::AddDataRes(DRType* pDataRes) {
		if (pDataRes == nullptr) return;
		GetDataResources<DRType>()[pDataRes->GetId()] = (pDataRes);
	}
	template <class DRType> inline void DataSys::RmvDataRes(const char* strName) {
		DRs<DRType>& s_DRs = GetDataResources<DRType>();
		DRs<DRType>::iterator itDR = std::find_if(s_DRs.begin(), s_DRs.end(),
			[=](std::pair<const UInt32, DRType*>& rObj)->bool {return strcmp(&rObj.second->GetName()[0], strName) == 0; });
		if (itDR == s_DRs.end()) { return; }
		s_DRs.erase(itDR);
	}
	template <class DRType> inline void DataSys::RmvDataRes(UInt32 unId) {
		DRs<DRType>& s_DRs = GetDataResources<DRType>();
		DRs<DRType>::iterator itDR = s_DRs.find(unId);
		if (itDR == s_DRs.end()) { return; }
		s_DRs.erase(itDR);
	}
}

#endif // NW_DATA_SYSTEM_H