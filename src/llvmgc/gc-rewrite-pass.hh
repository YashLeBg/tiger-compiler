/**
 ** \file llvmgc/gc-rewrite-pass.hh
 ** \brief Declaration of llvm::GcRewritePass.
 */

#pragma once

#include <llvm/IR/PassManager.h>

namespace llvm
{
  /** \brief Runs the GcRewritePass over the llvm::Function F and applies various
   ** transformations to each function, including forcing usage of the frame pointer,
   ** and additional work over allocas.
   **/
  struct GcRewritePass : public PassInfoMixin<GcRewritePass>
  {
  public:
    PreservedAnalyses run(Function& F, FunctionAnalysisManager& AM);
  };

} // namespace llvm
