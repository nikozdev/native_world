#ifndef NW_STRING_H
#define NW_STRING_H

#include <nw_core.hpp>

namespace NW
{
#if (NW_LIBS & NW_LIBS_NATIVE_STR)
	/// ConstString class
	template <typename CharType>
	class NW_API ConstString
	{
	public:
		ConstString() = default;
		ConstString(const char* str) : m_str((const CharType*)str) {}
		ConstString(const unsigned char* str) : m_str((const CharType*)str) {}
		ConstString(const ConstString& rCpy) = default;

		// --getters
		inline UInt32 GetLen() { return m_unLen; }

		// --operators
		bool operator==(const CharType* str) const { return strcmp(str, m_str) == 0; }
		bool operator==(const ConstString& rStr) const  { return strcmp(rStr.m_str, m_str) == 0; }
		bool operator==(const String& rStr) { return rStr == m_str; }
		const char& operator[](UInt32 unIndex) const { if (unIndex > m_unLen) { return ' '; } return m_str[unIndex]; }
		operator const char*() const { return (const char*)&m_str[0]; }
		operator const unsigned char*() const { return (const unsigned char*)&m_str[0]; }
	private:
		const CharType* m_str = "";
		UInt32 m_unLen = 0;
	};
	using CString = ConstString<Char>;
	using UCString = ConstString<UChar>;
#endif // NW_LIBS
#if (NW_LIBS & NW_LIBS_STD_STR)
#endif // NW_LIBS

	inline UInt32 StrLen(const char* cString);

	inline Int32 StrFindL(const char* cSource, const char cFind);
	inline Int32 StrFindR(const char* cSource, const char cFind);

	inline const char* GetStringPart(const char* cSource, UInt32 unBegin);
	inline const char* GetStringPart(const char* cSource, UInt32 unBegin, UInt32 unEnd);
	/// CStringLad class
	class NW_API CStringLad
	{
	public:
		CStringLad() = default;
		~CStringLad() = default;
	};
}

#endif // NW_STRING_H