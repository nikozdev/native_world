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
		printf("CN::MEMORY_MASTER::CONSTRUCTOR: Memory master is created\n");
		printMem();
	}
	MemoryMaster::~MemoryMaster()
	{
		printMem();
		printf("CN::MEMORY_MASTER::DESTRUCTOR: Memory master is dead\n");
	}

	// Allocation functions
	void* MemoryMaster::l_alloc(void* ud, void* ptr, size_t oldSize, size_t newSize)
	{
		return nullptr;
	}
	// Debug functions
	void MemoryMaster::printMem()
	{
		printf("CN::MEMORY_MASTER::HEAP_USAGE: Now is allocated:\n%zd blocks\n%zd bytes\n",
			(m_blocksCount), (m_bytesCount));
	}
}