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
  // FIXME: Some code was deleted here (Any global context that should be available to the allocator and collector).

  /// The frame address of main: the top of the stack
  void* tos;
  /// Is the gc enabled ?
  bool gc_enabled;
};

struct gc_object
{
  struct gc_md
  {
    // FIXME: Some code was deleted here (Define the format of the metadata carried by heap allocated objects).
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
