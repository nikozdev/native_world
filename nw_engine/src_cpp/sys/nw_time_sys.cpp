#include <nw_pch.hpp>
#include "nw_time_sys.h"

float NW::TimeSys::s_nRealTime(0.0f), NW::TimeSys::s_nRealDelta(0.0f), NW::TimeSys::s_nRealLast(0.0f);
float NW::TimeSys::s_nAppTime(0.0f), NW::TimeSys::s_nAppDelta(0.0f), NW::TimeSys::s_nAppLast(0.0f);
float NW::TimeSys::s_nTimeSpeed(1.0f);

NW::RealTimeCounter NW::TimeSys::s_RealTimeCounter = NW::RealTimeCounter();
NW::AppTimeCounter NW::TimeSys::s_AppTimeCounter = NW::AppTimeCounter();

NW::TimeInfo NW::TimeSys::s_TimeInfo;

#if (NW_WINDOW & NW_WINDOW_GLFW)
#include <GLFW/glfw3.h>
#elif (defined NW_PLATFORM_WINDOWS)   // chrono library timing
    static std::chrono::steady_clock::time_point s_tpCurrTime;
    static std::chrono::steady_clock::time_point s_tpLastTime;
    static std::chrono::duration<float> s_dfDeltaTime;
#endif  // NW_WINDOW

namespace NW
{
    // --getters
    const TimeInfo& TimeSys::GetTimeInfo() { std::mktime(&s_TimeInfo); return s_TimeInfo; }
    const char* TimeSys::GetTimeString() { return std::asctime(&s_TimeInfo); }
    // --setters

    // --==<core_methods>==--
    void TimeSys::Update()
    {
    #if (NW_WINDOW & NW_WINDOW_GLFW)
        s_nRealTime = static_cast<float>(glfwGetTime());
        s_nRealDelta = s_nRealTime - s_nRealLast;
        s_nRealLast = s_nRealTime;

        s_nAppDelta = s_nRealDelta * s_nTimeSpeed;
        s_nAppLast = s_nAppTime;
        s_nAppTime += s_nAppDelta;

        s_RealTimeCounter.EndCount();
        s_AppTimeCounter.EndCount();
    #elif (defined NW_PLATFORM_WINDOWS)
        s_tpCurrTime = std::chrono::steady_clock::now();
        s_dfDeltaTime = s_tpCurrTime - s_tpLastTime;
        s_tpLastTime = s_tpCurrTime;
        s_fTimeDelta = s_dfDeltaTime.count();
    #endif // NW_WINDOW_GLFW
    }
    // --==</core_methods>==--
}