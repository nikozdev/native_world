#ifndef NW_DATA_SYSTEM_H
#define NW_DATA_SYSTEM_H
#include <core/nw_data_res.h>

#include <nw_decl.hpp>
#include <ecs_decl.hpp>
#include <glib_decl.hpp>

#include <lib/utils/math_vector.h>

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
		template <class DRType> using DRs = HashMap<String, DRType*>;
	public:
		// --getters
		static inline const char* GetDirectory() { return &s_strRscDir[0]; }
		
		static inline ADRs& GetADataResources() { return s_ADRs; }
		static inline ADataRes* GetADataRes(UInt32 unId) {
			ADRs::iterator itDR = s_ADRs.find(unId);
			return itDR == s_ADRs.end() ? nullptr : itDR->second;
		}

		template <class DRType> static inline DRs<DRType>& GetDataResources() { static DRs<DRType> s_DRs; return s_DRs; }
		template <class DRType> static inline DRType* GetDataRes(UInt32 unId) { return dynamic_cast<DRType*>(GetADataRes(unId)); }
		template <class DRType> static inline DRType* GetDataRes(const char* strName) {
			DRs<DRType>& s_DRs = GetDataResources<DRType>();
			DRs<DRType>::iterator& itDR = s_DRs.find(strName);
			return itDR == s_DRs.end() ? nullptr : itDR->second;
		}
		// --setters
		static void SetDirectory(const char* strDir);
		static void AddADataRes(ADataRes* pDataRes);
		static void RemoveADataRes(UInt32 unId);
		
		template <class DRType> static inline void AddDataRes(DRType* pDataRes) {
			if (pDataRes == nullptr) return;
			GetDataResources<DRType>()[pDataRes->GetName()] = (pDataRes);
		}
		template <class DRType> static inline void RemoveDataRes(const char* strName) {
			DRs<DRType>& s_DRs = GetDataResources<DRType>();
			if (s_DRs.size() == 0) return;
			DRs<DRType>::iterator itDR = s_DRs.find(strName);
			if (itDR == s_DRs.end()) return;
			s_DRs.erase(itDR);
		}
		
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
}

#endif // NW_DATA_SYSTEM_H