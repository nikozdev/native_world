#ifndef CN_MEMORY_MASTER_H
#define CN_MEMORY_MASTER_H

#include <cn_core.hpp>

namespace CN
{
	/// MemoryMaster singleton class
	/// --Manipulates memory allocation
	/// --Counts bytes and blocks - prints memory usage info
	/// --Has static function for memory arena allocator
	class CN_API MemoryMaster
	{
	public:
		static size_t m_blocksCount;
		static size_t m_bytesCount;
	
		const size_t MIN_BLOCK_SIZE = 16;
	public:
		// Accessors
		//inline static MemoryMaster* get() { static MemoryMaster s_instance; return &s_instance; }
		
		// Functions
		/// Print currently used blocks and bytes counters in the console
		static void printMem();
		/// Memory allocation function where ud is the arena class
		static void* l_alloc(void* ud, void* ptr, size_t oldSize, size_t newSize);;
		/// Call new operator and return the memory adress
		void* alloc(size_t size);
		/// Call delete operator
		void dealloc(void* ptr, size_t size);
		/// -Allocate new the new block of memory with the min size
		/// --Copy the old information to the new block
		/// -->Deallocate the old block
		void* realloc(void* ptr, size_t oldSize, size_t newSize);
	private:
		/// Singleton Constructor. Initializes counters
		MemoryMaster();
		~MemoryMaster();

		size_t getMinSize(size_t size);
	};
}

#endif