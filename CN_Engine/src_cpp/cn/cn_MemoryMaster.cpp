#include <cn_pch.hpp>
#include <cn\cn_MemoryMaster.h>

size_t CN::MemoryMaster::m_blocksCount = 0;
size_t CN::MemoryMaster::m_bytesCount = 0;

void* operator new(size_t size)
{
	CN::MemoryMaster::m_blocksCount++;
	CN::MemoryMaster::m_bytesCount += size;
	return malloc(size);
}
void operator delete(void* ptr, size_t size)
{
	CN::MemoryMaster::m_blocksCount--;
	CN::MemoryMaster::m_bytesCount -= size;
	free(ptr);
}

namespace CN
{
	// Constructors&Destructor
	MemoryMaster::MemoryMaster()
	{
		CN_LOG("Memory master is created");
		printMem();
	}
	MemoryMaster::~MemoryMaster()
	{
		printMem();
		CN_LOG("Memory master is dead");
	}

	// Allocation functions
	void* MemoryMaster::alloc(size_t size)
	{
		return ::operator new(size);
	}
	void MemoryMaster::dealloc(void* ptr, size_t size)
	{
		return ::operator delete(ptr, size);
	}
	void* MemoryMaster::realloc(void* ptr, size_t oldSize, size_t newSize)
	{
		return ptr;
	}
	void* MemoryMaster::l_alloc(void* ud, void* ptr, size_t oldSize, size_t newSize)
	{
		MemoryMaster* pool = static_cast<MemoryMaster*>(ud);
		if (ptr != nullptr)
			if (newSize == 0)
			{
				pool->dealloc(ptr, oldSize);
			}
			else if (oldSize != 0)
			{
				return pool->realloc(ptr, oldSize, newSize);
			}
			else if (oldSize == 0)
			{//We don't have a pointer and any size of data - create new block
				return pool->alloc(newSize);
			}
		return nullptr;
	}

	// Implementation functions
	size_t MemoryMaster::getMinSize(size_t size)
	{
		if (size < MIN_BLOCK_SIZE)
			size = MIN_BLOCK_SIZE;
		return size;
	}

	// Debug functions
	void MemoryMaster::printMem()
	{
		printf("CN::MEMORY_MASTER::HEAP_USAGE: Now is allocated:\n%zd blocks\n%zd bytes\n",
			(m_blocksCount), (m_bytesCount));
	}
}