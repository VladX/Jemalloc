#include <stdlib.h>
#include <string.h>

void * je_malloc(size_t);
void * je_calloc(size_t, size_t);
void * je_realloc(void *, size_t);
void je_free(void *);
void malloc_init_hard(void);
#ifdef MOZ_MEMORY_WINDOWS
#define MALLOC_INIT() malloc_init_hard()
#else
#define MALLOC_INIT()
#endif
