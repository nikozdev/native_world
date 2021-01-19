#ifndef NW_CONTAINER_H
#define NW_CONTAINER_H

namespace NW
{
	/// IdStack class
	/// Description:
	/// --It's like a simple stack of unsigned 32-bit integers but for identificators
	/// --Contains one single ID for any class
	/// --You can get new id during construction of and object and put it back into the stack
	/// --If any destroyed instance gives back own ID to the stack - there is no loss
	class IdStack : protected DStack<UInt32>
	{
	public:
		IdStack() : DStack<UInt32>() { push(1); }
		// -- getters
		inline UInt32 GetFreeId() { UInt32 unFreeId = top(); if (size() == 1) { top()++; } else { pop(); } return unFreeId; }
		// -- setters
		inline void SetFreeId(UInt32 unFreeId) { if (unFreeId != top()) { push(unFreeId); } }
	};
#if false
	template <typename Type, int size>
	class NW_API SArray
	{
	public:
		SArray()
		{
			//
		}
		SArray(UInt16 size)
		{
			//
		}
		SArray(SArray& saCopied)
		{
			//
		}
		SArray(SArray&& saMoved)
		{
			//
		}
		~SArray() {}
		
		// Getters
		inline const UInt32 GetCount() const
		{
			return m_unCount;
		}
		inline const UInt32 GetSlots() const
		{
			return m_unSlots;
		}
		// Setters
		inline void SetCount()
		{
			//
		}
		
		// Interface Methods
		inline void PushBack(Type)
		{
			//
		}
		inline void Clear()
		{
			//
		}

		// Operators
		SArray& operator=(SArray& saCopied)
		{
		}
		SArray& operator=(SArray&& saMoved)
		{
		}
		Type& operator[](int index)
		{
		}
		const Type& operator[](int index) const
		{
		}
	private:
		Type* m_tElems;
		UInt m_unCount;
		UInt m_unSlots;
	};
	template <typename Type>
	class NW_API DArray
	{
	public:
		DArray()
		{
			//
		}
		explicit DArray(UInt16 size)
		{
			//
		}
		DArray(DArray& daCopied)
		{
			//
		}
		DArray(DArray&& daMoved)
		{
			//
		}
		~DArray()
		{
			//
		}

		// Getters
		inline const UInt32 GetCount() const
		{
			return m_unCount;
		}
		inline const UInt32 GetSlots() const
		{
			return m_unSlots;
		}
		// Setters
		inline void SetCount()
		{
			//
		}
		inline void SetSlots()
		{
			//
		}

		// Interface Methods
		inline void PushBack(Type)
		{
			//
		}
		inline void Clear()
		{
			//
		}

		// Operators
		DArray& operator=(DArray& daCopied)
		{
		}
		DArray& operator=(DArray&& daMoved)
		{
		}
		DArray& operator[](int index)
		{
		}
	private:
		Type* m_tElems;
		UInt32 m_unCount;
		UInt32 m_unSlots;
	};
#endif
}
#endif // NW_CONTAINER_H