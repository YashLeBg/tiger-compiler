/**
 ** \file llvmgc/runtime/tiger-gc.c
 ** \brief Implementation of the TC gc.
 ** It holds the main algorithm of the collector.
 ** This is a suggestion of skeleton for an implementation of a garbage collector.
 */

#include "tiger-gc.h"
#include <stdlib.h>
#include "fwd.h"

/// Defined by the compiler
extern void tc_main(int);
/// The function that defines the consts array
extern void init_consts(void);
/// This is called for initialization
struct gc_ctx gc_ctx_ = {
  // FIXME: Some code was deleted here.
    /// At the beging there is no object
    .head = NULL,
    /// At the beging we didnot allocated anything
    .allocated_bytes=0,
  .gc_enabled = true,
  .tos = NULL,
};

void gc_collect()
{
  if (!gc_ctx_.gc_enabled)
    return;

  // FIXME: Some code was deleted here (Run the collector).
}

void gc_enter_runtime()
{
  // FIXME: Some code was deleted here (Initialize any context required by the runtime).
}

void gc_exit_runtime()
{
  // FIXME: Some code was deleted here (Any logic required when exiting the runtime and going back to the tiger function).
}

int main(void)
{
  init_consts();

#ifdef GCDEBUG
  // This code will be included if the Tiger program was compiled with the task `--llvm-gc-debug`
#endif

  /// Initialize the top of the stack with main's frame address.
  gc_ctx_.tos = __builtin_frame_address(0);

  // FIXME: Some code was deleted here (Initialize any additional context required by the allocator and garbage collctor).
  tc_main(0);

  return 0;
}
