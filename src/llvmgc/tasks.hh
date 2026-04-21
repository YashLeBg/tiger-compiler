#pragma once

#include <llvmtranslate/fwd.hh>
#include <task/libtask.hh>

namespace llvmgc::tasks
{
  TASK_GROUP("LLVM Garbage Collection");

  /// Translate the AST to LLVM IR in a gc complying way.
  TASK_DECLARE("llvm-gc-compute",
               "translate to garbage collected LLVM IR.",
               llvm_gc_compute,
               "typed desugar-for desugar-string-cmp desugar escapes-compute");

  /// Enables debugging of the gc. This flag will cause llvm_gc_runtime_link to link the debug runtime
  /// The debug runtime is compiled with the `GCDEBUG` preprocessor directive defined.
  BOOLEAN_TASK_DECLARE("llvm-gc-debug",
                       "links the gc runtime in debug mode",
                       llvm_gc_debug_p,
                       "");

  /// Link the gc runtime to the generated module.
  TASK_DECLARE("llvm-gc-runtime-link",
               "links the IR to the GC runtime.",
               llvm_gc_runtime_link,
               "llvm-gc-compute");
} // namespace llvmgc::tasks
