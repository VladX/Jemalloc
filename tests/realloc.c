#include "jemalloc.h"

const size_t min_size = 1024 * 1;
const size_t max_size = 1024 * 1024;
#define num 100

int main (void)
{
	size_t i;
	char * ptrs[num];
	MALLOC_INIT();
	for (i = 0; i < num; i++)
	{
		size_t size = (rand() % (max_size - min_size)) + min_size;
		ptrs[i] = je_realloc(NULL, 1);
		ptrs[i] = je_realloc(ptrs[i], size);
		memset(ptrs[i], 1, size);
	}
	for (i = 0; i < num; i++)
		je_free(ptrs[i]);
	return 0;
}
