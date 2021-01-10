#ifndef NW_ADATA_RES_H
#define NW_ADATA_RES_H

#include <nw_pch.hpp>
#include <nw_core.hpp>

namespace NW
{
	/// Abstract DataResource class
	/// Interface:
	/// -> Derrive any class you want from the ADataRes class
	/// -> Implement Save() and Load() methods
	/// Description:
	/// -- Every object in the engine can be saved in a binary file or some format
	/// -- Class which is inherit of ADataRes can be: Saved | Loaded
	/// -- Everything we need - save/load methods implementation for this
	class NW_API ADataRes
	{
		friend class DataSys;
	protected:
		ADataRes(const char* strName, UInt32 unId);
	public:
		ADataRes(const char* strName);
		ADataRes(const ADataRes& rDataRes);
		virtual ~ADataRes();

		// --getters
		inline UInt32 GetId() const { return m_unId; }
		inline const char* GetName() const { return &m_strName[0]; }
		// --setters
		virtual void SetName(const char* strName);

		// --core_methods
		virtual bool SaveF(const char* strFPath) = 0;
		virtual bool LoadF(const char* strFPath) = 0;
	protected:
		UInt32 m_unId;
		String m_strName;

		static IdStack s_IdStack;
	};
}
namespace NW
{
	/// DataResourceBuffer struct
	struct NW_API DRBuffer : std::streambuf
	{
		DRBuffer(std::ifstream& inFileStream, UInt32 offset, UInt32 size);
		DArray<char> m_daMemory;
	};
	/// DataResourcePack class
	class NW_API DataResPack : std::streambuf
	{
	public:
		DataResPack();
		~DataResPack();

		// Getters
		DRBuffer GetDataBuffer(const String& strFile)
		{
			//
		}
		// Setters
		bool SetFile(const String& strFile) {}
		// Interface Methods
		bool SavePack(const String& strFile, const String& strKey);
		bool LoadPack(const String& strFile, const String& strKey);
	private:
		struct ResFileStr { UInt32 unSize; UInt32 unOffset; };
		std::map<String, ResFileStr> m_mapFiles;
		std::ifstream baseFile;
		DArray<char> Collect(const DArray<char>& dacData, String strKey);
		String MakePosIX(const String& path);
	};
}

#endif // NW_ADATA_RES_H