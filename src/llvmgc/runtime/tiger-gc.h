/**
 ** \file llvmgc/runtime/tiger-gc.h
 ** \brief Declaration of the TC gc
 */

#ifndef TIGER_GC_H
#define TIGER_GC_H

#include <stdbool.h>
#include "fwd.h"

/// The global state of the garbage collector
struct gc_ctx
{
  // FIXED: Some code was deleted here (Any global context that should be available to the allocator and collector).
  /// The head of the double linked list that track all objects that have been allocated
    struct gc_object* head;
    /// The current amount of bytes allocated in the heap
    size_t allocated_bytes;
    ///The maximum amount of bytes allowed before arrasing all
    size_t threshlod
  /// The frame address of main: the top of the stack
  void* tos;
  /// Is the gc enabled ?
  bool gc_enabled;
};

struct gc_object
{
  struct gc_md
  {
    // FIXED: Some code was deleted here (Define the format of the metadata carried by heap allocated objects).
    /// The size of the user data in bytes requested during allocation
    size_t size;
    /// True means the object is alive, otherwise its dead
    bool marked;
    /// The pointer to the next allocated object in our list
    struct gc_object* next;
    /// The pointer to the previous allocated object 
    struct gc_object* prev;
  } md;
  // The fields of the object
  tc_word_t f[0]; // The user pointer points here
};

/// The global state of the garbage collector.
extern struct gc_ctx gc_ctx_;

void gc_collect();

void gc_enter_runtime();

void gc_exit_runtime();

#endif /* ! TIGER_GC_H */
