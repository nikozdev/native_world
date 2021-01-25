#ifndef NWG_MEMORY_H
#define NWG_MEMORY_H

#include <nwg_pch.hpp>

// --==<RefKeeper>==--
namespace NWG
{
	/// RefKeeper class
	/// Description:
	/// -- Smart "shared" pointer in nw implementation
	/// -- Allocates object due to given allocator
	/// -- The reference gets deleted if there is no any other RefKeepers for it
	/// Interface:
	/// -> Create RefKeeper -> MakeRef with particular allocator -> use as a pointer
	template <typename MType>
	class RefKeeper
	{
	public:
		RefKeeper() :
			m_pAllocator(nullptr), m_pRef(nullptr), m_pRefCounter(nullptr), m_szData(0){ }
		RefKeeper(RefKeeper& rCpy) :
			m_pAllocator(rCpy.m_pAllocator), * m_pRef(*rCpy.m_pRef), m_szData(rCpy.m_szData) { }
		~RefKeeper() { Reset(); }

		// --getters
		inline AMemAllocator* GetAllocator() { return m_pAllocator; }
		inline MType* GetRef() { return m_pRef; }
		inline UInt16* GetRefCounter() { return m_pRefCounter; }
		inline Size GetSize() const { return m_szData; }
		// --setters
		inline void SetRef(RefKeeper<MType>& rRefKeeper);
		inline void Reset();
		// --core_methods
		template <typename VType, typename...Args>
		inline void MakeRef(AMemAllocator& rAllocator, Args...Arguments) {
			Reset();
			m_pAllocator = &rAllocator;
			m_szData = GetAligned(sizeof(VType), __alignof(VType));
			m_pRef = NewT<VType>(rAllocator, Arguments...);
			m_pRefCounter = NewT<UInt16>(rAllocator);
			*m_pRefCounter = 1;
		}
		template <typename VType>
		inline void MakeRef(AMemAllocator& rAllocator, VType& rCpy) {
			Reset();
			m_pAllocator = &rAllocator;
			m_szData = GetAligned(sizeof(VType), __alignof(VType));
			m_pRef = NewT<VType>(rAllocator, rCpy);
			m_pRefCounter = NewT<UInt16>(rAllocator);
			*m_pRefCounter = 1;
		}
		// --operators
		inline MType* operator->() { return m_pRef; }
		inline MType& operator*() { return *m_pRef; }
		inline void operator=(RefKeeper& rCpy) { SetRef(rCpy); }
	private:
		AMemAllocator* m_pAllocator;
		MType* m_pRef;
		Size m_szData;
		UInt16* m_pRefCounter;
	};
	// --setters
	template <typename MType>
	inline void RefKeeper<MType>::SetRef(RefKeeper<MType>& rRefKeeper) {
		Reset();
		m_pAllocator = rRefKeeper.m_pAllocator;
		m_pRef = rRefKeeper.m_pRef;
		m_pRefCounter = rRefKeeper.m_pRefCounter;
		if (m_pRefCounter != nullptr) { (*m_pRefCounter) += 1; }
		m_szData = rRefKeeper.m_szData;
	}
	template <typename MType>
	inline void RefKeeper<MType>::Reset() {
		if (m_pRef != nullptr && m_pAllocator != nullptr && m_pRefCounter != nullptr) {
			if (*m_pRefCounter == 1) {
				m_pRef->~MType();
				m_pAllocator->Dealloc(m_pRef, m_szData);
				DelT<UInt16>(*m_pAllocator, m_pRefCounter);
			}
			else { (*m_pRefCounter) -= 1; }
		}
		m_pAllocator = nullptr;
		m_pRef = nullptr;
		m_pRefCounter = nullptr;
		m_szData = 0;
	}
}
// --==</RefKeeper>==--

#endif // NWG_MEMORY_H