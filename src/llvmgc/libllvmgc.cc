/**
 ** \file llvmgc/libgc.hh
 ** \brief Public llvmgc module interface implementation.
 **/

#include <common.hh> // program_name
#include <llvmgc/gc-rewrite-pass.hh>
#include <llvmgc/libllvmgc.hh>
#include <llvmgc/tasks.hh>
#include <llvmgc/translator.hh>
#include <llvmtranslate/escapes-collector.hh>
#include <llvmtranslate/translator.hh>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

#include <llvm/AsmParser/Parser.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Passes/PassBuilder.h>

#pragma GCC diagnostic pop

namespace llvmgc
{

  std::pair<std::unique_ptr<llvm::LLVMContext>, std::unique_ptr<llvm::Module>>
  translate(const ast::Ast& the_program)
  {
    auto ctx = std::make_unique<llvm::LLVMContext>();
    auto module = std::make_unique<llvm::Module>(program_name, *ctx);

/// If defined, use the GCTranslator to generate LLVM-IR.
/// This alternative translation may be used to generate a more GC-friendly IR.
#ifdef TC_LLVMGC_TRANSLATION
    GCTranslator translate{*module,
                           llvmtranslate::collect_escapes(the_program)};
#else
    llvmtranslate::Translator translate{
      *module, llvmtranslate::collect_escapes(the_program)};
#endif

    translate(the_program);

#ifdef TC_LLVMGC_TRANSLATION
    // Verifying the module is needed if you choose to run an LLVM pass.
    // Some passes may crash or produce a SEGV when working on an ill-formatted module.
    if (llvm::verifyModule(*module, &llvm::errs()))
      {
        misc::error e;
        e.ice_here();
      }
#endif

    // https://llvm.org/docs/WritingAnLLVMNewPMPass.html
    // These must be declared in this order so that they are destroyed in the
    // correct order due to inter-analysis-manager references.
    llvm::LoopAnalysisManager LAM;
    llvm::FunctionAnalysisManager FAM;
    llvm::CGSCCAnalysisManager CGAM;
    llvm::ModuleAnalysisManager MAM;

    llvm::PassBuilder PB;

    PB.registerModuleAnalyses(MAM);
    PB.registerCGSCCAnalyses(CGAM);
    PB.registerFunctionAnalyses(FAM);
    PB.registerLoopAnalyses(LAM);
    PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);

    llvm::ModulePassManager MPM =
      PB.buildPerModuleDefaultPipeline(llvm::OptimizationLevel::O1);

/// If `TC_LLVMGC_TRANSLATION` defined, run our own LLVM pass.
#ifdef TC_LLVMGC_TRANSLATION
    // Our own pass, to setup the gc strategy and additional work on alloca
    MPM.addPass(llvm::createModuleToFunctionPassAdaptor(llvm::GcRewritePass{}));

    // FIXME: Some code was deleted here (You may choose to run various llvm passes).
#endif

    MPM.run(*module, MAM);

#ifdef TC_LLVMGC_TRANSLATION
    if (llvm::verifyModule(*module, &llvm::errs()))
      {
        misc::error e;
        e.ice_here();
      }
#endif

    return {std::move(ctx), std::move(module)};
  }

  std::unique_ptr<llvm::Module> runtime_gc_get(llvm::LLVMContext& ctx,
                                               bool llvm_gc_debug)
  {
    llvm::SMDiagnostic diag;
    const char* runtime_string = nullptr;

    if (llvm_gc_debug)
      {
        runtime_string = runtime_debug_gc_string();
      }
    else
      {
        runtime_string = runtime_gc_string();
      }

    return llvm::parseAssemblyString(runtime_string, diag, ctx);
  }
} // namespace llvmgc
