#ifndef NW_STRING_H
#define NW_STRING_H

#include <nw_core.hpp>

namespace NW
{
	inline UInt32 StrLen(const char* cString);

	inline Int32 StrFindL(const char* cSource, const char cFind);
	inline Int32 StrFindR(const char* cSource, const char cFind);

	inline const char* GetStringPart(const char* cSource, UInt32 unBegin);
	inline const char* GetStringPart(const char* cSource, UInt32 unBegin, UInt32 unEnd);
}

#endif // NW_STRING_H