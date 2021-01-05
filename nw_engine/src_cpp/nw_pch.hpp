#ifndef NW_PCH_HPP
#define NW_PCH_HPP
/// PreCompiled header file
/// This file contains the most usable include files
/// nw_pch.cpp file gets it and generates .pch file in the intermediate files
/// .pch header isn't essentially compiles every time we want to run the app
/// .pch header includes in every cpp file of project

// --<standard>--
// --stl
#include <array>
#include <vector>
#include <stack>
#include <queue>
#include <list>
#include <map>
#include <set>
#include <utility>
#include <functional>
#include <algorithm>
// --streams
//#include <iomanip>
//#include <streambuf>
#include <iostream>
//#include <ostream>
//#include <istream>
#include <sstream>
#include <fstream>
//#include <iosfwd>
//#include <ios>
// --numbers
#include <random>
//#include <complex>
//#include <valarray>
//#include <numeric>
//#include <limits>
// --strings
#include <string>
//#include <regex>
// --support
#include <exception>
//#include <stdexcept>
//#include <locale>
#include <typeinfo>
#include <typeindex>
// --memory
#include <new>
#include <memory>
// --parallel computation
#include <thread>
#include <future>
#include <mutex>
// --console
#include <conio.h>
// --</standard>--

// --<native>--
#include <cstddef>

#include <cstdarg>
#include <cstdio>
#include <ctime>

//#include <cassert>
//#include <clocale>
//#include <climits>
//#include <cerrno>

//#include <cfloat>
//#include <cmath>

//#include <cstring>
//#include <cctype>
//#include <cwchar>
//#include <cwctype>
// --</native>--

// --<platform>--
#ifdef NW_PLATFORM_WINDOWS
#include <Windows.h>
#endif
// --</platform>--

// --<CheerNik_lib>--
#include <lib/nw_string.h>
#include <lib/nw_container.h>
#include <lib/nw_algorithm.h>
#include <lib/nw_memory.h>
// --</CheerNik_lib>--

#endif	// NW_PCH_HPP