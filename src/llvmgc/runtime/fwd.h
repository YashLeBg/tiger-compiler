#ifndef GC_FWD_H
#define GC_FWD_H

#include <stdint.h>

#define EXIT_GC_FAILURE 121

/// The type of a word.
/// Everything is a word in Tiger, so a pointer to any Tiger object is a WORD*.
typedef uintptr_t tc_word_t;

/// How the GC seens an object.
struct gc_object;

#endif /* ! GC_FWD_H */
