#include "nw_pch.hpp"
#include "nw_rand_sys.h"

std::mt19937 NW::RandSys::s_RandEngine(time(0));

namespace NW
{
}