#ifndef NW_DATA_SYSTEM_H
#define NW_DATA_SYSTEM_H

#include <nw_decl.hpp>
#include <glib_core.hpp>

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
		static String FDialogSave(const char* strFilter);
		static String FDialogLoad(const char* strFilter);
		// --binary_data
		static bool SaveFData(const char *strFPath, void* pData, UInt64 unBytes);
		static bool SaveFData(const char *directory, const char *name, const char *noPointformat,
			void* pData, UInt64 unBytes);
		static inline bool SaveFData(void* pData, UInt64 unBytes) { return SaveFData(&FDialogSave("all_files(*.*)\0*.*\0")[0], pData, unBytes); }
		static bool LoadFData(const char *strFPath, void* pData, UInt64 unBytes);
		static bool LoadFData(const char *directory, const char *name, const char *noPointformat,
			void *pData, UInt64 unBytes);
		static inline bool LoadFData(void* pData, UInt64 unBytes) { return LoadFData(&FDialogLoad("all_files(*.*)\0*.*\0")[0], pData, unBytes); }
		// --strings
		static bool SaveFString(const char* strFPath, const char* strSrc, UInt64 unBytes);
		static bool LoadFString(const char* strFPath, String& strDest);
		// --images
		static UInt8* LoadFImage(const char* strFPath, Int32* pnW, Int32* pnH, Int32* pnChannels);
		static bool LoadFImage(const char* strFPath, UByte* pClrDataBuf, Int32* pnW, Int32* pnH, Int32* pnChannels);
		static bool LoadFImage(const char* strFilePath, GLIB::ImageInfo* pImage);
	private:
		static ADRs s_ADRs;
		static String s_strRscDir;
	};
}

#endif // NW_DATA_SYSTEM_H