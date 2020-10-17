#include <CheerNik.h>
#include <stdio.h>

namespace cn
{
	void log()
	{
		printf("CN::FILE_%s::LINE_%d:\n", __FILE__, __LINE__);
	}
}