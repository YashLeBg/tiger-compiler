/**
 ** \file llvmgc/runtime/gc-ctrl.h
 ** \brief Declaration of the TC gc control functions.
 ** The definition of the primitives used to interact with the garbage collector.
 */

#ifndef GC_CTRL_H
#define GC_CTRL_H

#include <stddef.h>

/// Enable or disable the garbage collector.
void tc_gc_enable(size_t enable);

/// Manually run the collector.
void tc_gc_run(void);

#endif /* ! GC_CTRL_H */
