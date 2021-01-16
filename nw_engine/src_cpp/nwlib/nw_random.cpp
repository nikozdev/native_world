#include "nw_pch.hpp"
#include "nw_random.h"

std::mt19937 NW::Random::s_RandEngine(time(0));

namespace NW
{
}