#include <nwg_pch.hpp>

#define NWG_LAUNCH_TEST		1 << 0
#define NWG_LAUNCH_ENGINE	1 << 1
#define NWG_LAUNCH			NWG_LAUNCH_ENGINE

#if (NWG_LAUNCH & NWG_LAUNCH_ENGINE)
#include <nwg_core_states.h>
#endif	// NWG_LAUNCH
#if (NWG_LAUNCH & NWG_LAUNCH_TEST)

using ValType = String;

template<typename Type1, typename Type2>
struct NWL_API Pair
{
public:
	Type1 First;
	Type2 Second;
public:
	Pair(const Type1& rVal1, const Type2& rVal2) : First(rVal1, rVal2) { }
	Pair(const Pair<Type1, Type2>& rCpy) : First(rCpy.First, rCpy.Second) { }
};
/// Iterator with two links: prev-next iterators
//template<typename ValType>
class NWL_API Iter2Link
{
public:
	Iter2Link(Iter2Link* itPrev = nullptr, Iter2Link* itNext = nullptr) : m_pRef(nullptr), m_itPrev(itPrev), m_itNext(itNext)
	{
		if (m_itPrev != nullptr) { m_itPrev->m_itNext = this; }
		if (m_itNext != nullptr) { m_itNext->m_itPrev = this; }
	}
	~Iter2Link()
	{
		if (m_itNext != nullptr) { m_itNext->m_itPrev = m_itPrev;}
		if (m_itPrev != nullptr) { m_itPrev->m_itNext = m_itNext; }
	}

	// --getters
	inline ValType* GetRef() const			{ return m_pRef; }
	inline Iter2Link* GetNext() const		{ return m_itNext; }
	inline Iter2Link* GetPrev() const		{ return m_itPrev; }
	// --setters
	inline void SetRef(ValType* pRef)		{ m_pRef = pRef; }
	inline void SetNext(Iter2Link* itNext)	{ if (m_itNext != nullptr) { m_itNext->m_itPrev = nullptr; } m_itNext = itNext; if (itNext != nullptr) { itNext->m_itPrev = this; } }
	inline void SetPrev(Iter2Link* itPrev)	{ if (m_itPrev != nullptr) { m_itPrev->m_itNext = nullptr; } m_itPrev = itPrev; if (itPrev != nullptr) { itPrev->m_itNext = this; } }
	// --operators
	inline ValType* operator->()			{ return m_pRef; }
	inline ValType& operator*()				{ return *m_pRef; }
	inline bool operator==(Iter2Link& iter)	{ return (iter.m_pRef == m_pRef && iter.m_itNext == m_itNext && iter.m_itPrev == m_itPrev); }
	inline bool operator!=(Iter2Link& iter)	{ return (iter.m_pRef != m_pRef || iter.m_itNext != m_itNext || iter.m_itPrev != m_itPrev); }
	// --movement
	inline void operator++() {
		if (m_itNext == nullptr) { NWL_ERR("Out of bounds"); return; }
		
		m_itNext->m_itPrev = m_itPrev;
		if (m_itPrev != nullptr) { m_itPrev->m_itNext = m_itNext; }
		m_itPrev = m_itNext;

		m_itNext = m_itPrev->m_itNext;
		if (m_itNext != nullptr) { m_itNext->m_itPrev = this; }
		m_itPrev->m_itNext = this;
	}
	inline void operator--() {
		if (m_itPrev == nullptr) { NWL_ERR("Out of bounds"); return; }

		m_itPrev->m_itNext = m_itNext;
		if (m_itNext != nullptr) { m_itNext->m_itPrev = m_itPrev; }
		m_itNext = m_itPrev;
		m_itPrev = m_itNext->m_itPrev;
		if (m_itPrev != nullptr) { m_itPrev->m_itNext = this; }
		m_itNext->m_itPrev = this;
	}
	inline void operator+=(UInt32 unSteps) { while (unSteps-- != 0) { operator++(); } }
	inline void operator-=(UInt32 unSteps) { while (unSteps-- != 0) { operator--(); } }
private:
	ValType* m_pRef;
	Iter2Link* m_itPrev;
	Iter2Link* m_itNext;
};

/// List2Link container class
class NWL_API List2Link
{
	using Iter = Iter2Link;
public:
	List2Link() : m_itBeg(Iter()), m_itEnd(m_itBeg) { }
	~List2Link() { while (m_itBeg != m_itEnd) { ; } }

	// --getters
	inline Iter& GetBeg() { return m_itBeg; }
	inline Iter& GetEnd() { return m_itEnd; }
	// --setters
	// --predicates
	// --operators
	inline Iter& begin() { return m_itBeg; }
	inline Iter& end() { return m_itEnd; }
	// --core_methods
	inline void PushBack(ValType& rVal) {
		m_itEnd.SetRef(NewT<ValType>(rVal));
	}
	inline void Erase(Iter& rIter) {
		for (Iter iter = m_itBeg; iter != m_itEnd; iter++) {
			if (iter == rIter) {
				break;
			}
		}
		DelT<ValType>(rIter.GetRef());
	}
private:
	Iter m_itBeg;
	Iter m_itEnd;
};
#endif	// NWG_LAUNCH

int main(int nArgs, char* strArgs[])
{
	try {
#if (NWG_LAUNCH & NWG_LAUNCH_ENGINE)
		NW::CoreEngine* pGameEngine = &NW::CoreEngine::Get();
		NWG::CreatorState crtState;
		pGameEngine->AddState(crtState);
		if (!pGameEngine->Init(1 << 24)) { return -1; }
		pGameEngine->SwitchState("creator_state");
		while (pGameEngine->IsRunning()) { pGameEngine->Update(); }
		pGameEngine->Quit();
		//pGameEngine->Run(1 << 24);
#endif
#if (NWG_LAUNCH & NWG_LAUNCH_TEST)
		NW::TimeSys::Update();
		{
			Byte Bytes[1 << 12];
			MemArena s_Memory(Bytes, 1 << 12);
			SetGlobalMemory(s_Memory);
			String str0 = "str0";
			String str1 = "hello1";
			String str2 = "world2";
			List2Link StrList;
			StrList.PushBack(str0);
			StrList.PushBack(str1);
			StrList.PushBack(str2);
			for (auto& itStr : StrList) { std::cout << itStr; }
		}
		NW::TimeSys::Update();
		NW::LogSys::WriteStr("The test takes {flt} milliseconds\n", NW::TimeSys::GetRealDelta());
#endif
	}
	catch (std::exception Exc) {
		NWL_ERR(Exc.what());
	}

	return 0;
}