#ifndef DEBUGMALLOC_NOMACRO_H
#define DEBUGMALLOC_NOMACRO_H

#include <stdbool.h>
#include "debugmalloc-impl.h"

/* Use these macro-like functions to allocate and deallocate memory.
 * Semantics: the same as the standard functions. */
#define debugmalloc_malloc(X) debugmalloc_malloc_full((X), "malloc", #X, __FILE__, __LINE__, false)
#define debugmalloc_calloc(X,Y) debugmalloc_malloc_full((X)*(Y), "calloc", #X ", " #Y, __FILE__, __LINE__, true)
#define debugmalloc_realloc(P,X) debugmalloc_realloc_full((P), (X), "realloc", #X, __FILE__, __LINE__)
#define debugmalloc_free(P) debugmalloc_free_full((P), "free", __FILE__, __LINE__)

#endif  /* DEBUGMALLOC_NOMACRO_H */
