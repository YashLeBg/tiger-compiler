/**
 ** \file llvmgc/runtime/gc-ctrl.c
 ** \brief Implementation of the TC gc control functions.
 */

#include "gc-ctrl.h"
#include "tiger-gc.h"

void tc_gc_enable(size_t enable) { gc_ctx_.gc_enabled = !!enable; }

void tc_gc_run(void)
{
  gc_enter_runtime();

  gc_collect();

  gc_exit_runtime();
}
