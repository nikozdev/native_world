#ifndef NW_TIME_SYSTEM_H
#define NW_TIME_SYSTEM_H

#include <nw_core.hpp>

namespace NW
{
	/// TimeSystem static class
	/// Description:
	/// -- The class for getting time. Update Method updates values
	class NW_API TimeSys
	{
	public:
		friend class EvSys;
	public:
		// --getters
		static inline float GetRealLast() { return s_nRealLast; }
		static inline float GetRealTime() { return s_nRealTime; }
		static inline float GetRealDelta() { return s_nRealDelta; }
		static inline TimeCounter& GetRealTimeCounter() { static TimeCounter s_tc; return s_tc; }

		static inline float GetAppTime() { return s_nAppTime; }
		static inline float GetAppLast() { return s_nAppLast; }
		static inline float GetAppDelta() { return s_nAppDelta; }
		static inline float GetAppSpeed() { return s_nTimeSpeed; }
		static inline TimeCounter& GetAppTimeCounter() { static TimeCounter s_tc; return s_tc; }

		static const TimeInfo& GetTimeInfo();
		static const char* GetTimeString();
		// --setters
		static inline void SetAppSpeed(float nSpeed) { s_nTimeSpeed = nSpeed; }

		// --core_methods
		static void Update();
	private:
		// Time: current frame, last frame and the difference between them
		static float s_nAppTime, s_nAppDelta, s_nAppLast;
		static float s_nRealTime, s_nRealDelta, s_nRealLast;
		static float s_nTimeSpeed;

		static TimeInfo s_TimeInfo;
	};
}

#endif // NW_TIME_SYSTEM_H