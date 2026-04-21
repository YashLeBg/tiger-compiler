/**
 ** \file llvmgc/libllvmgc.hh
 ** \brief Public llvmgc module interface declaration.
 */

#pragma once

#include <memory>
#include <utility>

#include <llvm/IR/LLVMContext.h>

#include <llvm/IR/Type.h>
#include <llvmtranslate/fwd.hh>

/// Translation from ast::Ast to llvm::Value.
namespace llvmgc
{

  /// Translate the file into a llvm::Module.
  std::pair<std::unique_ptr<llvm::LLVMContext>, std::unique_ptr<llvm::Module>>
  translate(const ast::Ast& the_program);

  /// Load the gc runtime as a llvm::Module.
  std::unique_ptr<llvm::Module> runtime_gc_get(llvm::LLVMContext& ctx,
                                               bool llvm_gc_debug);

  /// The LLVM gc runtime as a string, loaded from the generated file.
  /// This function is implemented in $(build_dir)/src/llvmgc/gc-runtime.cc
  /// For more information take a look at `local.am`. This works exactly as `runtime_string` in llvmtranslate.
  const char* runtime_gc_string();

  /// The gc runtime with debug directives enabled.
  const char* runtime_debug_gc_string();
} // namespace llvmgc
