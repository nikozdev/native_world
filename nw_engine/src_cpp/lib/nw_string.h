#ifndef NW_STRING_H
#define NW_STRING_H

#include <nw_core.hpp>

namespace NW
{
#if (NW_LIBS & NW_LIBS_NATIVE_STR)
	class NW_API String
	{
	public:
	private:
	};
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