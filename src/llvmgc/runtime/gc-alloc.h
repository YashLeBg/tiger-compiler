/**
 ** \file llvmgc/runtime/gc-alloc.h
 ** \brief Declaration of the TC allocation functions.
 */

#ifndef GC_ALLOC_H
#define GC_ALLOC_H

#include <stddef.h>
#include "fwd.h"

/// Specialized allocator for records, called by llvmgc::GCTranslator
void* tc_init_record(
  // FIXED: Some code was deleted here (Choose the arguments used by tc_init_record).
    size_t size
        );

/// Initialize a string with size characters. `size` must include the NULL terminator.
char* tc_init_string(size_t size);

/// Override tc_init_array
size_t* tc_init_array(size_t size, size_t elt);

/// Initialize an array of pointer
size_t* tc_init_ptr_array(size_t size, struct gc_object* elt);

/// Override the default tc_malloc
void* tc_malloc(size_t size);

#endif /* ! GC_ALLOC_H */
