#ifndef NW_MEMORY_H
#define NW_MEMORY_H
#include <nw_core.hpp>

namespace NW
{
	/// Memory Info struct
	struct NW_API MemInfo
	{
	public:
		Size szAlloc;
		UInt32 unAlloc;
	public:
		OutStream& operator<<(OutStream& rStream) const;
	};
	inline OutStream& operator<<(OutStream& rStream, const MemInfo& rInfo) { return rInfo.operator<<(rStream); }
	/// MemoryLink struct
	/// Description:
	/// -- Stores pointers to some block of memory
	/// -- References list of other MemoryLinks
	/// while next link doesn't have last == nullptr
	struct NW_API MemLink
	{
	public:
		MemLink* pNext = nullptr;
	};
#if (NW_LIBS & NW_LIBS_NATIVE_PTR)
	/// ReferenceOwnerer class (same as unique_ptr)
	/// Interface:
	/// -> Create RefOwner with defined type
	/// and address of an object of that type
	/// -> Use this reference as default object allocated in the heap
	/// ==>To get the object reference - use it as a functor class()
	/// --The main feature is that this is heap allocated object
	/// which will be deleted if it's address is lost
	/// --If this RefOwner is deleted
	/// --> destructor will delete the pointed object
	/// --Local Reference should point on the unique object
	/// which is not pointed by other pointers
	template <typename Type>
	class NW_API RefOwner
	{
		friend class RefOwner<Type>;
	public:
		// Constructors&Destructor
		RefOwner(Type* object)
		{
			SetNew(object);
		}
		RefOwner(const RefOwner& lrCopy) = delete;
		RefOwner(RefOwner&& movedRef)
		{
			Clear();
			m_address = movedRef.m_address;
			movedRef.m_address = nullptr;
		}
		~RefOwner()
		{
			Clear();
		}

		// Getters
		inline void SetNew(Type* object)
		{
			Clear();
			m_address = object;
		}
		// Setters
		inline Type* Get()
		{
			return m_address;
		}
		inline void Clear()
		{
			if (m_address)
			{
				MemSys::DelT<Type>(m_address);
			}
			m_address = nullptr;
		}

		// Methods

		// Overloaded operators
		inline Type* operator->()
		{
			return m_address;
		}
		inline Type& operator*()
		{
			return *m_address;
		}
	private:
		Type* m_address;
	};
	/// RefKeeper class
	/// Interface:
	/// -> Create RefKeeper with defined type -> Set the address of a heap allocated object
	/// -> Share this addres with other RefKeepers
	template <typename Type>
	class NW_API RefKeeper
	{
		friend class RefKeeper<Type>;
	public:
		RefKeeper(Type* object)
		{
			m_object = object;
			m_refCount++;
		}
		RefKeeper(const RefKeeper& globalRef)
		{
			ConnectRefs(this, globalRef);
		}
		~RefKeeper()
		{
			MemSys::DelT<Type>(m_address);
		}
		// Getters
		inline void SetNew(Type* object)
		{
			Clear();
			m_address = object;
		}
		// Setters
		inline Type* Get()
		{
			return m_address;
		}
		inline void Clear()
		{
			if (m_address && m_refCount == 1)
			{
				MemSys::DelT<Type>(m_address);
			}
			m_refCount--;
			m_address = nullptr;
		}

		/// Methods
		inline void Share(RefKeeper<Type> otherRef)
		{
			m_refCount++;
			otherRef.m_address = m_address;
		}

		// Overloaded operators
		inline Type* operator->()
		{
			return m_address;
		}
		inline Type& operator*()
		{
			return *m_address;
		}

		// Static functions
		static void ConnectRefs(const RefKeeper<Type>& emptyRef, const RefKeeper<Type>& objectHandler)
		{
			if (globRef.m_address == nullptr) return;
			emptyRef.m_address = globRef.m_address();
			emptyRef.m_refCount++;
			objectHandler.m_refCount++;
		}
	private:
		Type* m_address;

		uint16_t m_refCount;
	};
	template <typename RefType> class NW_API RefUser
	{
		//
	};
#elif (NW_LIBS & NW_LIBS_STD_PTR)
#endif
	/// MemoryLad(Allocator) class
	/// Description:
	/// --Allocates the memory for uninitialized objects
	/// Interface:
	/// -> Allocate some memory of required type
	/// -> Construct the object that you need in the allocated block
	/// -> When you dont need that object - destruct it
	/// -> After the destruction - deallocate the memory
	template<typename ValType>
	class NW_API MemLad
	{
	public:
		// core_methods
		inline ValType* Alloc(UInt32 InstancesCount) {
			return reinterpret_cast<ValType*>(malloc(sizeof(ValType) * InstancesCount));
		}
		inline void Delloc(ValType* pBlock, UInt32 InstancesCount) {
			free(pBlock);
		}

		inline void Construct(ValType* pBlock, ValType& rValue, UInt32 unInstancesCount = 1) {
			for (UInt32 i = 0; i < unInstancesCount; i++)
				MemSys::NewPlaceT(pBlock[i], rValue);
		}
		inline void Destruct(ValType* pBlock, UInt32 InstancesCount = 1) {
			for (UInt32 i = 0; i < InstancesCount; i++)
			{
				pBlock[i].~ValType();
				pBlock = nullptr;
			}
		}
	};
}
namespace NW
{
	/// MemoryArena class
	/// -- Allocates the memory in the given pool
	/// -- Uses FreeList to track deallocated blocks
	/// -- If there is not enough space - uses glogal allocation
	class NW_API MemArena
	{
	private:
		Byte* m_pCurr;
		Byte* m_pBegin;
		Byte* m_pEnd;
		MemLink* m_FreeList;

		Size m_szAllignment = 8;
		Size m_szBlock = m_szAllignment * 8;
	public:
		MemArena(Size szSpace) :
			m_pBegin(nullptr), m_pEnd(nullptr),
			m_pCurr(nullptr),
			m_FreeList(nullptr)
		{
			Reset(szSpace);
		}
		~MemArena() { Reset(0); }

		// --getters
		inline Size GetMinBlockSize(Size szBlock) { return szBlock < m_szBlock ? m_szBlock : szBlock; }
		// --setters
		void Reset(Size szSpace);

		// --core_methods
		void* Alloc(Size szAlloc);
		void Dealloc(void* pBlock, Size szDealloc);
		void* Realloc(void* pBlock, Size szOld, Size szNew);
	};
}
#endif // NW_MEMORY_H