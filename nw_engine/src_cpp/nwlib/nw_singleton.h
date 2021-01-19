#ifndef NW_ASINGLETON_H
#define NW_ASINGLETON_H

#include <nw_pch.hpp>
#include <nw_core.hpp>

namespace NW
{
	/// ASingleton Interface for unique global classes
	template<class Type>
	class NW_API ASingleton
	{
	public:
		ASingleton(const ASingleton& rCpy) = delete;
		virtual ~ASingleton() = default;
		// --getters
		static inline Type& Get() { static Type s_instance;  return s_instance; }
		
		void operator =(ASingleton& rCpy) = delete;
		void operator =(const ASingleton& rCpy) = delete;
	protected:
		ASingleton() = default;
	};
}

#endif // NW_ASINGLETON_H