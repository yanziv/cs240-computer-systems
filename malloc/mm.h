#include <stdio.h>

extern int mm_init (void);
extern void *mm_malloc (size_t size);
extern void mm_free (void *ptr);

/* For Extra Fun -- not required. */
extern void* mm_realloc(void* payload, size_t size);
