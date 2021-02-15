#ifndef NW_PCH_HPP
#define NW_PCH_HPP
/// PreCompiled header file
/// This file contains the most usable include files
/// nw_pch.cpp file gets it and generates .pch file in the intermediate files
/// .pch header isn't essentially compiles every time we want to run the app
/// .pch header includes in every cpp file of project

#include <native_library.hpp>
using namespace NWL;
#include <native_graphics.hpp>
using namespace NWG;

#endif	// NW_PCH_HPP