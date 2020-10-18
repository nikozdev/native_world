#ifndef CN_PCH_H
#define CN_PCH_H
/// PreCompiled header file
/// This file contains the most usable include files
/// cn_pch.cpp file gets it and generates .pch file in the intermediate files
/// .pch header isn't essentially compiles every time we want to run the app
/// .pch header includes in every cpp file of project

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdio.h>

#include <memory>
#include <time.h>
#include <algorithm>
#include <functional>

#include <vector>
#include <stack>
#include <unordered_map>

#ifdef CN_PLATFORM_WINDOWS
#include <windows.h>
#endif

#endif