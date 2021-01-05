#include <nw_pch.hpp>
#include "nw_time.h"

#include <sys/nw_time_sys.h>

namespace NW
{
    // ========<RealTimeCounter>========
    RealTimeCounter::RealTimeCounter() :
        m_nBeginTime(TimeSys::GetRealTime()),
        m_nEndTime(TimeSys::GetRealTime()),
        m_nCreationTime(TimeSys::GetRealTime()) { }

    // -- Getters
    // -- Setters
    void RealTimeCounter::BeginCount() { m_nEndTime = TimeSys::GetRealTime(); }
    void RealTimeCounter::EndCount() { m_nEndTime = TimeSys::GetRealTime(); }
    // -- Interface Methods
    // ========</RealTimeCounter>========

    // ========<AppTimeCounter>========
    AppTimeCounter::AppTimeCounter() :
        m_nBeginTime(TimeSys::GetAppTime()),
        m_nEndTime(TimeSys::GetAppTime()),
        m_nCreationTime(TimeSys::GetAppTime()) { }

    // -- Getters
    // -- Setters
    void AppTimeCounter::BeginCount() { m_nEndTime = TimeSys::GetAppTime(); }
    void AppTimeCounter::EndCount() { m_nEndTime = TimeSys::GetAppTime(); }
    // -- Interface Methods
    // ========</AppTimeCounter>========
}