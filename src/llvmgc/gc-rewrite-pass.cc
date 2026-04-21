/**
 ** \file llvmgc/gc-rewrite-pass.cc
 ** \brief Implementation of llvm::GcRewritePass
 ** An LLVM pass can be viewed as a visitor that runs over a given LLVM structure.
 ** In the case of `GcRewritePass`, the `run` member function is applied to every LLVM function
 ** present in the module given to the constructor.
 **
 ** NOTE:
 ** Like llvmgc::GCTranslator, the `GcRewritePass` is optional and a working GC can
 ** be implemented without using it.
 **/

#include <llvm/IR/Constants.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/Casting.h>
#include <llvmgc/gc-rewrite-pass.hh>
#include <llvmgc/libllvmgc.hh>

llvm::PreservedAnalyses
llvm::GcRewritePass::run(Function& F,
                         [[maybe_unused]] FunctionAnalysisManager& AM)
{
  auto& ctx_ = F.getContext();

  // For usage of the frame pointer
  llvm::AttrBuilder B(ctx_);
  B.addAttribute("frame-pointer", "all");

  llvm::AttributeList AL =
    llvm::AttributeList::get(ctx_, llvm::AttributeList::FunctionIndex, B);

  F.setAttributes(AL);

  // Create a builder at the entry block of the function
  llvm::IRBuilder<> tmp(&F.getEntryBlock(), F.getEntryBlock().begin());

  // Set the insert point right after allocas declarations to insert instructions at
  // the beginning of the function
  tmp.SetInsertPointPastAllocas(&F);

  for (llvm::inst_iterator begin = llvm::inst_begin(F), end = llvm::inst_end(F);
       begin != end; ++begin)
    {
      // FIXME: Some code was deleted here (Apply any transformation over the function's instructions).
    }

  return PreservedAnalyses::none();
}
