#ifndef NWG_PCH_H
#define NWG_PCH_H
/// PreCompiled header file
/// This file contains the most usable include files
/// cn_pch.cpp file gets it and generates .pch file in the intermediate files
/// .pch header isn't essentially compiles every time we want to run the app
/// .pch header includes in every cpp file of project

#include <cstdint>
#include <string>
#include <cstring>

#include <streambuf>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <conio.h>

#include <memory>
#include <typeindex>

#include <time.h>
#include <chrono>

#include <thread>
#include <mutex>

#include <algorithm>
#include <functional>

#include <list>
#include <array>
#include <vector>
#include <stack>
#include <unordered_map>
#include <map>

#ifdef NW_PLATFORM_WINDOWS
#include <windows.h>
#endif

#endif	// NWG_PCH