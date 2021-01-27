#ifndef NW_DATA_SYSTEM_H
#define NW_DATA_SYSTEM_H

#include <nw_decl.hpp>
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
		
		// --setters
		static void SetDirectory(const char* strDir);
		
		// --core_methods
		static bool OnInit();
		static void OnQuit();
		// --file_dialogs
		static String FDialog_save(const char* strFilter);
		static String FDialog_load(const char* strFilter);
		// --binary_data
		static bool SaveFData(const char *strFPath, void* pData, UInt64 unBytes);
		static bool SaveFData(const char *directory, const char *name, const char *noPointformat,
			void* pData, UInt64 unBytes);
		static inline bool SaveFData(void* pData, UInt64 unBytes) { return SaveFData(&FDialog_save("all_files(*.*)\0*.*\0")[0], pData, unBytes); }
		static bool LoadFData(const char *strFPath, void* pData, UInt64 unBytes);
		static bool LoadFData(const char *directory, const char *name, const char *noPointformat,
			void *pData, UInt64 unBytes);
		static inline bool LoadFData(void* pData, UInt64 unBytes) { return LoadFData(&FDialog_load("all_files(*.*)\0*.*\0")[0], pData, unBytes); }
		// --strings
		static bool SaveFString(const char* strFPath, const char* strSrc, UInt64 unBytes);
		static bool LoadFString(const char* strFPath, String& strDest);
		// --images
		static UInt8* LoadFImage(const char* strFPath, Int32* pnW, Int32* pnH, Int32* pnChannels);
		static bool LoadFImage(const char* strFPath, UByte* pClrDataBuf, Int32* pnW, Int32* pnH, Int32* pnChannels);
		static bool LoadFImage(const char* strFilePath, ImageInfo* pImage);
		// --meshes
		static bool SaveFMesh(const String& strFilePath, DArray<VertexShape3d>* pVtxData, DArray<UInt32>* punIndData);
		static bool LoadFMesh(const String& strFilePath, DArray<VertexShape3d>* pVtxData, DArray<UInt32>* punIndData);
		// --shaders
	private:
		static ADRs s_ADRs;
		static String s_strRscDir;
	private:
		static bool LoadFMeshDae(const String& strFileData, DArray<UInt32>& arrIndices,
			DArray<V3f>& vtxCoords, DArray<V2f>& texCoords, DArray<V3f>& normalCoords);
		static bool LoadFMeshDae(const String& strFileData, DArray<UInt32>& arrIndices,
			DArray<float>& vtxCoords, DArray<float>& texCoords, DArray<float>& normalCoords);
		static bool LoadFMeshObj(const String& strFileData, DArray<UInt32>& arrIndices,
			DArray<V3f>& vtxCoords, DArray<V2f>& texCoords, DArray<V3f>& normalCoords);
		static bool LoadFMeshObj(const String& strFileData, DArray<UInt32>& arrIndices,
			DArray<float>& vtxCoords, DArray<float>& texCoords, DArray<float>& normalCoords);
		static bool LoadFMeshObj(const String& strFileData, DArray<VertexShape3d>& rVtxData, DArray<UInt32>& runIndData);
	};
}

#endif // NW_DATA_SYSTEM_H