/**
 ** \file llvmgc/tasks.cc
 ** \brief LLVM GC Translate tasks.
 */

#include <llvm/IR/Module.h>
#include <llvmtranslate/tasks.hh>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

#include <llvm/Config/llvm-config.h> // LLVM_VERSION_*
#include <llvm/Linker/Linker.h>

#pragma GCC diagnostic pop

#include <ast/tasks.hh>
#include <llvmgc/libllvmgc.hh>
#include <llvmtranslate/fwd.hh>
#define DEFINE_TASKS 1
#include <llvmgc/tasks.hh>
#undef DEFINE_TASKS

namespace llvmgc::tasks
{
  /// Translate the AST to LLVM IR in a GC friendly way.
  void llvm_gc_compute()
  {
    llvmtranslate::tasks::module = translate(*ast::tasks::the_program);
  }

  void llvm_gc_runtime_link()
  {
    auto gc_runtime =
      runtime_gc_get(*llvmtranslate::tasks::module.first, llvm_gc_debug_p);

#if LLVM_VERSION_MAJOR == 3 && LLVM_VERSION_MINOR <= 7
    auto gc_link =
      llvm::Linker::LinkModules(module.second.get(), runtime.get());
#else
    auto gc_link = llvm::Linker::linkModules(
      *llvmtranslate::tasks::module.second, std::move(gc_runtime));
#endif

    (void)gc_link;
    postcondition(!gc_link); // Returns true on error
  }
} // namespace llvmgc::tasks
