#include <nw_pch.hpp>
#include "nw_time_sys.h"

float NW::TimeSys::s_nRealTime(0.0f), NW::TimeSys::s_nRealDelta(0.0f), NW::TimeSys::s_nRealLast(0.0f);
float NW::TimeSys::s_nAppTime(0.0f), NW::TimeSys::s_nAppDelta(0.0f), NW::TimeSys::s_nAppLast(0.0f);
float NW::TimeSys::s_nTimeSpeed(1.0f);

NWL::TimeInfo NW::TimeSys::s_TimeInfo;

#if (NW_WINDOW & NW_WINDOW_GLFW)
#include <GLFW/glfw3.h>
#endif

namespace NW
{
    // --getters
    const TimeInfo& TimeSys::GetTimeInfo() { std::mktime(&s_TimeInfo); return s_TimeInfo; }
    const char* TimeSys::GetTimeString() { return std::asctime(&s_TimeInfo); }
    // --setters

    // --==<core_methods>==--
    void TimeSys::Update()
    {
    #if (NW_WINDOW & NW_WINDOW_GLFW & false)
        s_nRealTime = static_cast<float>(glfwGetTime());
    #else
        s_nRealTime = clock();
    #endif // NW_WINDOW_GLFW
        s_nRealDelta = s_nRealTime - s_nRealLast;
        s_nRealLast = s_nRealTime;

        s_nAppDelta = s_nRealDelta * s_nTimeSpeed;
        s_nAppLast = s_nAppTime;
        s_nAppTime += s_nAppDelta;

        GetRealTimeCounter().EndCount();
        GetAppTimeCounter().EndCount();
    }
    // --==</core_methods>==--
}