/**
 ** \file llvmtranslate/translator.cc
 ** \brief Implementation of llvmtranslate::Translator
 */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

#include <llvm/Config/llvm-config.h> // LLVM_VERSION_*
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Verifier.h> // llvm::verifyFunction
#include <llvm/Support/Casting.h>
#include <llvm/TargetParser/Host.h> // llvm::sys
#include <llvm/TargetParser/Triple.h>

#pragma GCC diagnostic pop

#include <ast/all.hh>
#include <llvmtranslate/translator.hh>
<<<<<<< HEAD
#include <type/builtin-types.hh>
#include <type/function.hh>
#include <type/record.hh>
=======
>>>>>>> 2028-tc-EXTS.0

namespace llvmtranslate
{
  using namespace std::string_literals;

  namespace
  {
<<<<<<< HEAD
=======
    // Shorthands for integer type and pointer to integer type.
>>>>>>> 2028-tc-EXTS.0
    inline llvm::IntegerType* i64_t(llvm::LLVMContext& ctx)
    {
      return llvm::Type::getInt64Ty(ctx);
    }

    inline llvm::PointerType* i64p_t(llvm::LLVMContext& ctx)
    {
      return llvm::PointerType::get(llvm::Type::getInt64Ty(ctx), 1);
    }

    llvm::AllocaInst* create_alloca(llvm::Function* ll_function,
                                    llvm::Type* ll_type,
                                    const std::string& name)
    {
<<<<<<< HEAD
=======
      // Create an IRBuilder starting at the beginning of the current entry
      // block. LLVM treats allocas as local variables only if they occur at the
      // beginning of a function.
>>>>>>> 2028-tc-EXTS.0
      llvm::IRBuilder<> tmp(&ll_function->getEntryBlock(),
                            ll_function->getEntryBlock().begin());
      return tmp.CreateAlloca(ll_type, nullptr, name);
    }

<<<<<<< HEAD
    void set_default_attributes(llvm::Function& the_function,
                                const ast::FunctionDec& e)
    {
      the_function.addFnAttr(llvm::Attribute::NoUnwind);
      if (!e.body_get())
=======
    // Set default attributes to the functions
    void set_default_attributes(llvm::Function& the_function,
                                const ast::FunctionDec& e)
    {
      the_function.addFnAttr(llvm::Attribute::NoUnwind); // No exceptions in TC
      if (!e.body_get())                                 // Inline primitives
>>>>>>> 2028-tc-EXTS.0
        the_function.addFnAttr(llvm::Attribute::InlineHint);
    }

    std::string function_dec_name(const ast::FunctionDec& e)
    {
<<<<<<< HEAD
      if (e.name_get() == "_main")
        return "tc_main";
=======
      // Rename "_main" to "tc_main"
      if (e.name_get() == "_main")
        return "tc_main";
      // Prefix all the primitives with "tc_"
>>>>>>> 2028-tc-EXTS.0
      if (!e.body_get())
        return "tc_" + e.name_get().get();
      return e.name_get().get();
    }
  } // namespace

  Translator::Translator(llvm::Module& module, escaped_map_type&& escaped)
    : module_{module}
    , ctx_{module_.getContext()}
    , builder_{ctx_}
    , escaped_{std::move(escaped)}
    , type_visitor_{ctx_}
  {
<<<<<<< HEAD
=======
    // Initialize the allocator
>>>>>>> 2028-tc-EXTS.0
    auto function_ltype = llvm::FunctionType::get(
      llvm::PointerType::get(ctx_, 1), {i64_t(ctx_)}, false);

    auto name = "tc_malloc";
    auto linkage = llvm::Function::ExternalLinkage;
    malloc_ = llvm::Function::Create(function_ltype, linkage, name, &module_);

<<<<<<< HEAD
    auto process_triple = llvm::Triple(llvm::sys::getProcessTriple());
=======
    // The current process triple.
    auto process_triple = llvm::Triple(llvm::sys::getProcessTriple());
    // Set the 64-bit version of the triple.
>>>>>>> 2028-tc-EXTS.0
    module_.setTargetTriple(process_triple.get64BitArchVariant().str());
  }

  void Translator::operator()(const ast::Ast& e)
  {
    translate(e);
    value_ = nullptr;
  }

  llvm::Value* Translator::translate(const ast::Ast& node)
  {
    node.accept(*this);
    return value_;
  }

  llvm::Value* Translator::access_var(const ast::Var& e)
  {
    if (auto var_ast = dynamic_cast<const ast::SimpleVar*>(&e))
      {
<<<<<<< HEAD
        return locals_[current_function_][var_ast->def_get()];
      }
    else if (auto arr_ast = dynamic_cast<const ast::SubscriptVar*>(&e))
      {
        auto base_val = translate(arr_ast->var_get());
        auto index_val = translate(arr_ast->index_get());
        auto elem_ltype = llvm_type(*arr_ast->type_get());
        return builder_.CreateGEP(elem_ltype, base_val, index_val, "arrptr");
      }
    else if (auto field_ast = dynamic_cast<const ast::FieldVar*>(&e))
      {
        const ast::Var* var = &field_ast->var_get();
        auto var_val = translate(*var);

        const type::Record* record_type =
          static_cast<const type::Record*>(&var->type_get()->actual());
        misc::symbol field_name = field_ast->name_get();
        int index = field_ast->index_get();

        llvm::Type* record_ltype = type_visitor_.get_record_ltype(record_type);
=======
        // FIXME: Some code was deleted here.
      }
    else if (auto arr_ast = dynamic_cast<const ast::SubscriptVar*>(&e))
      {
        // FIXME: Some code was deleted here.
      }
    else if (auto field_ast = dynamic_cast<const ast::FieldVar*>(&e))
      {
        const ast::Var* var = nullptr;
        // FIXME: Some code was deleted here.
        auto var_val = translate(*var);

        const type::Record* record_type = nullptr;
        // FIXME: Some code was deleted here.
        misc::symbol field_name;
        // FIXME: Some code was deleted here.
        int index = -1;
        // FIXME: Some code was deleted here (Get the index of the field).

        // The GEP instruction provides us with safe pointer arithmetics,
        // usually used with records or arrays.
        llvm::Type* record_ltype = nullptr;
        // FIXME: Some code was deleted here (Get record's corresponding LLVM type).
>>>>>>> 2028-tc-EXTS.0
        return builder_.CreateStructGEP(record_ltype, var_val, index,
                                        "fieldptr_"s + field_name.get());
      }
    else
      unreachable();
  }

  llvm::Value* Translator::init_array(llvm::Value* count_val,
                                      llvm::Value* init_val)
  {
<<<<<<< HEAD
=======
    // Cast everything so that it is conform to the signature of init_array
    // int *init_array(int, int)

    // We need to separate the pointers and the ints.
    // LLVM requires a ptrtoint instruction for pointers
    // and a bitcast for others.
>>>>>>> 2028-tc-EXTS.0
    auto init_val_cast = init_val->getType()->isPointerTy()
      ? builder_.CreatePtrToInt(init_val, i64_t(ctx_), "init_array_ptrtoint")
      : builder_.CreateBitCast(init_val, i64_t(ctx_), "init_array_bitcast");

<<<<<<< HEAD
    std::vector<llvm::Type*> arg_type{i64_t(ctx_), init_val_cast->getType()};

    auto init_array_ltype =
      llvm::FunctionType::get(i64p_t(ctx_), arg_type, false);

=======
    // Create the init_array function:
    // First, the arguments (int*, int, int)
    std::vector<llvm::Type*> arg_type{i64_t(ctx_), init_val_cast->getType()};

    // Then, create the FunctionType.
    auto init_array_ltype =
      llvm::FunctionType::get(i64p_t(ctx_), arg_type, false);

    // Get the llvm::Function from the module related to the name and type
>>>>>>> 2028-tc-EXTS.0
    auto init_array_function = module_.getOrInsertFunction(
      init_val->getType()->isPointerTy() ? "tc_init_ptr_array"
                                         : "tc_init_array",
      init_array_ltype);

<<<<<<< HEAD
    std::vector<llvm::Value*> arg_vals{count_val, init_val_cast};

    auto init_array_call =
      builder_.CreateCall(init_array_function, arg_vals, "init_array_call");

=======
    // Prepare the arguments.
    std::vector<llvm::Value*> arg_vals{count_val, init_val_cast};

    // Create the call.
    auto init_array_call =
      builder_.CreateCall(init_array_function, arg_vals, "init_array_call");

    // Cast the result of the call in the desired type.
>>>>>>> 2028-tc-EXTS.0
    return builder_.CreateBitCast(init_array_call,
                                  init_val->getType()->getPointerTo(1),
                                  "init_array_call_cast");
  }

  llvm::Type* Translator::llvm_type(const type::Type& type)
  {
    type_visitor_(type);
    return type_visitor_.llvm_type_get();
  }

  llvm::FunctionType*
  Translator::llvm_function_type(const type::Function& function_type)
  {
<<<<<<< HEAD
    std::vector<llvm::Type*> args_types;
=======
    // Prepare the arguments
    std::vector<llvm::Type*> args_types;
    // First, if there are any escaped vars, create ptr arguments for it
    // (Lambda lifting)
>>>>>>> 2028-tc-EXTS.0

    if (auto escapes_it = escaped_.find(&function_type);
        escapes_it != std::end(escaped_))
      {
        auto& escapes = escapes_it->second;
        args_types.reserve(escapes.size()
                           + function_type.formals_get().fields_get().size());
        for (const auto dec : escapes)
          {
<<<<<<< HEAD
            llvm::Type* var_ltype = llvm_type(*dec->type_get());
=======
            llvm::Type* var_ltype = nullptr;
            // FIXME: Some code was deleted here (Get the llvm type of the VarDec).
>>>>>>> 2028-tc-EXTS.0
            args_types.emplace_back(llvm::PointerType::getUnqual(var_ltype));
          }
      }
    else
      args_types.reserve(function_type.formals_get().fields_get().size());

<<<<<<< HEAD
=======
    // Then, the actual arguments
>>>>>>> 2028-tc-EXTS.0
    for (const auto& field : function_type.formals_get())
      args_types.emplace_back(llvm_type(field.type_get()));

    llvm::Type* result_ltype = nullptr;
<<<<<<< HEAD
    if (dynamic_cast<const type::Void*>(&function_type.result_get().actual()))
      result_ltype = llvm::Type::getVoidTy(ctx_);
    else
      result_ltype = llvm_type(function_type.result_get());
=======
    // FIXME: Some code was deleted here (If the result is void typed, we assign llvm void type to result_ltype).
    result_ltype = llvm_type(function_type.result_get());
>>>>>>> 2028-tc-EXTS.0

    return llvm::FunctionType::get(result_ltype, args_types, false);
  }

  void Translator::operator()(const ast::SimpleVar& e)
  {
<<<<<<< HEAD
    if (dynamic_cast<const type::Void*>(&e.type_get()->actual()))
      {
        value_ = llvm::ConstantInt::getSigned(i64_t(ctx_), 0);
        return;
      }
    auto ptr = access_var(e);
    value_ = builder_.CreateLoad(llvm_type(*e.type_get()), ptr, e.name_get().get());
=======
    // Void var types are actually Ints represented by a 0
    // FIXME: Some code was deleted here.
>>>>>>> 2028-tc-EXTS.0
  }

  void Translator::operator()(const ast::FieldVar& e)
  {
<<<<<<< HEAD
    auto ptr = access_var(e);
    value_ = builder_.CreateLoad(llvm_type(*e.type_get()), ptr, "fieldload");
=======
    // FIXME: Some code was deleted here.
>>>>>>> 2028-tc-EXTS.0
  }

  void Translator::operator()(const ast::SubscriptVar& e)
  {
<<<<<<< HEAD
    auto ptr = access_var(e);
    value_ = builder_.CreateLoad(llvm_type(*e.type_get()), ptr, "arrload");
  }

  void Translator::operator()(const ast::NilExp&)
  {
    value_ = llvm::ConstantPointerNull::get(llvm::PointerType::get(ctx_, 1));
=======
    // FIXME: Some code was deleted here.
  }

  void Translator::operator()(const ast::NilExp& e)
  {
    // FIXME: Some code was deleted here (Create a null pointer).
>>>>>>> 2028-tc-EXTS.0
  }

  void Translator::operator()(const ast::IntExp& e)
  {
<<<<<<< HEAD
    value_ = llvm::ConstantInt::getSigned(i64_t(ctx_), e.value_get());
=======
    // FIXME: Some code was deleted here (Integers in Tiger are all 64bit signed).
>>>>>>> 2028-tc-EXTS.0
  }

  void Translator::operator()(const ast::StringExp& e)
  {
<<<<<<< HEAD
    value_ = builder_.CreateGlobalStringPtr(e.value_get(), "str");
=======
    // FIXME: Some code was deleted here (Strings are translated as `i8*` values, like C's `char*`).
>>>>>>> 2028-tc-EXTS.0
  }

  void Translator::operator()(const ast::RecordExp& e)
  {
<<<<<<< HEAD
    const type::Record* record_type =
      static_cast<const type::Record*>(&e.type_get()->actual());

    llvm_type(*record_type);
    auto struct_ltype = type_visitor_.get_record_ltype(record_type);

    auto sizeof_val = llvm::ConstantExpr::getSizeOf(struct_ltype);
    sizeof_val = llvm::ConstantExpr::getTruncOrBitCast(sizeof_val, i64_t(ctx_));

    auto malloc_val = builder_.CreateMalloc(
      i64_t(ctx_), struct_ltype, sizeof_val, nullptr, malloc_, "malloccall");

    for (size_t i = 0; i < e.fields_get().size(); ++i)
      {
        auto field_val = translate(e.fields_get()[i]->init_get());
        auto field_ptr = builder_.CreateStructGEP(struct_ltype, malloc_val, i);
        builder_.CreateStore(field_val, field_ptr);
      }
=======
    // Get the record type
    const type::Record* record_type = nullptr;
    // FIXME: Some code was deleted here.

    // Type the record and use get_record_ltype() to get its LLVM type
    llvm_type(*record_type);
    auto struct_ltype = type_visitor_.get_record_ltype(record_type);

    // The size of the structure and cast it to int
    auto sizeof_val = llvm::ConstantExpr::getSizeOf(struct_ltype);
    sizeof_val = llvm::ConstantExpr::getTruncOrBitCast(sizeof_val, i64_t(ctx_));

    // Generate the instruction calling Malloc
    auto malloc_val = builder_.CreateMalloc(
      i64_t(ctx_), struct_ltype, sizeof_val, nullptr, malloc_, "malloccall");

    // Init the fields
    // FIXME: Some code was deleted here.
>>>>>>> 2028-tc-EXTS.0

    value_ = malloc_val;
  }

  void Translator::operator()(const ast::OpExp& e)
  {
<<<<<<< HEAD
    auto left_val = translate(e.left_get());
    auto right_val = translate(e.right_get());

    switch (e.oper_get())
      {
      case ast::OpExp::Oper::add:
        value_ = builder_.CreateAdd(left_val, right_val, "addtmp");
        break;
      case ast::OpExp::Oper::sub:
        value_ = builder_.CreateSub(left_val, right_val, "subtmp");
        break;
      case ast::OpExp::Oper::mul:
        value_ = builder_.CreateMul(left_val, right_val, "multmp");
        break;
      case ast::OpExp::Oper::div:
        value_ = builder_.CreateSDiv(left_val, right_val, "divtmp");
        break;
      case ast::OpExp::Oper::eq:
        value_ = builder_.CreateICmpEQ(left_val, right_val, "eqtmp");
        break;
      case ast::OpExp::Oper::ne:
        value_ = builder_.CreateICmpNE(left_val, right_val, "netmp");
        break;
      case ast::OpExp::Oper::lt:
        value_ = builder_.CreateICmpSLT(left_val, right_val, "lttmp");
        break;
      case ast::OpExp::Oper::le:
        value_ = builder_.CreateICmpSLE(left_val, right_val, "letmp");
        break;
      case ast::OpExp::Oper::gt:
        value_ = builder_.CreateICmpSGT(left_val, right_val, "gttmp");
        break;
      case ast::OpExp::Oper::ge:
        value_ = builder_.CreateICmpSGE(left_val, right_val, "getmp");
        break;
      }
=======
    // FIXME: Some code was deleted here.
    // The comparison instructions returns an i1, and we need an i64, since everything
    // is an i64 in Tiger. Use a zero-extension to avoid this.
>>>>>>> 2028-tc-EXTS.0
    value_ = builder_.CreateZExtOrTrunc(value_, i64_t(ctx_), "op_zext");
  }

  void Translator::operator()(const ast::SeqExp& e)
  {
<<<<<<< HEAD
    if (e.exps_get().empty())
      {
        value_ = llvm::ConstantInt::getSigned(i64_t(ctx_), 0);
        return;
      }
    for (const auto& exp : e.exps_get())
      value_ = translate(*exp);
=======
    // An empty SeqExp is an empty expression, so we should return an int
    // containing 0, since its type is void.
    // FIXME: Some code was deleted here.
>>>>>>> 2028-tc-EXTS.0
  }

  void Translator::operator()(const ast::AssignExp& e)
  {
<<<<<<< HEAD
    auto val = translate(e.exp_get());
    auto ptr = access_var(e.var_get());
    builder_.CreateStore(val, ptr);
    value_ = llvm::ConstantInt::getSigned(i64_t(ctx_), 0);
=======
    // FIXME: Some code was deleted here.
>>>>>>> 2028-tc-EXTS.0
  }

  void Translator::operator()(const ast::IfExp& e)
  {
<<<<<<< HEAD
    auto cond_val = translate(e.test_get());
    auto zero = llvm::ConstantInt::getSigned(cond_val->getType(), 0);
    auto cmp = builder_.CreateICmpNE(cond_val, zero, "ifcond");

    auto then_bb = llvm::BasicBlock::Create(ctx_, "then", current_function_);
    auto else_bb = llvm::BasicBlock::Create(ctx_, "else", current_function_);
    auto merge_bb = llvm::BasicBlock::Create(ctx_, "ifcont", current_function_);

    builder_.CreateCondBr(cmp, then_bb, else_bb);

    builder_.SetInsertPoint(then_bb);
    auto then_val = translate(e.thenclause_get());
    auto then_end = builder_.GetInsertBlock();
    builder_.CreateBr(merge_bb);

    builder_.SetInsertPoint(else_bb);
    auto else_val = translate(e.elseclause_get());
    auto else_end = builder_.GetInsertBlock();
    builder_.CreateBr(merge_bb);

    builder_.SetInsertPoint(merge_bb);

    if (dynamic_cast<const type::Void*>(&e.type_get()->actual()))
      {
        value_ = llvm::ConstantInt::getSigned(i64_t(ctx_), 0);
        return;
      }

    auto phi = builder_.CreatePHI(then_val->getType(), 2, "iftmp");
    phi->addIncoming(then_val, then_end);
    phi->addIncoming(else_val, else_end);
    value_ = phi;
=======
    // FIXME: Some code was deleted here (IfExps are handled in a similar way to Kaleidoscope (see LangImpl5.html)).
>>>>>>> 2028-tc-EXTS.0
  }

  void Translator::operator()(const ast::WhileExp& e)
  {
<<<<<<< HEAD
=======
    // Bb containing the test and the branching
>>>>>>> 2028-tc-EXTS.0
    auto test_bb = llvm::BasicBlock::Create(ctx_, "test", current_function_);
    auto body_bb = llvm::BasicBlock::Create(ctx_, "body", current_function_);
    auto after_bb =
      llvm::BasicBlock::Create(ctx_, "afterloop", current_function_);

<<<<<<< HEAD
    loop_end_[&e] = after_bb;

    builder_.CreateBr(test_bb);

=======
    // Save the after block for breaks
    loop_end_[&e] = after_bb;

    // Explicitly fall through from the current block
    builder_.CreateBr(test_bb);

    // Start inside the test BasicBlock
>>>>>>> 2028-tc-EXTS.0
    builder_.SetInsertPoint(test_bb);

    auto cond_val = translate(e.test_get());
    auto zero_val = llvm::ConstantInt::getSigned(cond_val->getType(), 0);
    auto cmp_val = builder_.CreateICmpNE(cond_val, zero_val, "loopcond");

<<<<<<< HEAD
    builder_.CreateCondBr(cmp_val, body_bb, after_bb);

    builder_.SetInsertPoint(body_bb);
    translate(e.body_get());

    builder_.CreateBr(test_bb);

=======
    // Create the branching
    builder_.CreateCondBr(cmp_val, body_bb, after_bb);

    // Translate the body inside the body BasicBlock
    builder_.SetInsertPoint(body_bb);
    // Don't store the return value, is should be void.
    translate(e.body_get());

    // Go back to the Test BasicBlock
    builder_.CreateBr(test_bb);

    // Continue after the loop BasicBlock
>>>>>>> 2028-tc-EXTS.0
    builder_.SetInsertPoint(after_bb);
  }

  void Translator::operator()(const ast::BreakExp& e)
  {
<<<<<<< HEAD
    auto loop = static_cast<const ast::WhileExp*>(e.def_get());
    builder_.CreateBr(loop_end_[loop]);
    auto unreachable_bb =
      llvm::BasicBlock::Create(ctx_, "unreachable", current_function_);
    builder_.SetInsertPoint(unreachable_bb);
    value_ = llvm::ConstantInt::getSigned(i64_t(ctx_), 0);
=======
    // FIXME: Some code was deleted here.
>>>>>>> 2028-tc-EXTS.0
  }

  void Translator::operator()(const ast::ArrayExp& e)
  {
<<<<<<< HEAD
    auto count_val = translate(e.size_get());
    auto init_val = translate(e.init_get());
    value_ = init_array(count_val, init_val);
=======
    // Translate the number of elements,
    // fill the array with the default value, then
    // return the pointer to the allocated zone.
    // FIXME: Some code was deleted here (Use `init_array`).
>>>>>>> 2028-tc-EXTS.0
  }

  void Translator::operator()(const ast::CastExp& e)
  {
    auto exp_val = translate(e.exp_get());
<<<<<<< HEAD
    llvm::Type* ltype = llvm_type(*e.ty_get().type_get());
=======
    llvm::Type* ltype = nullptr;
    // FIXME: Some code was deleted here (Destination llvm type).
>>>>>>> 2028-tc-EXTS.0
    value_ = builder_.CreateBitCast(exp_val, ltype, "cast_exp");
  }

  void Translator::operator()(const ast::FunctionChunk& e)
  {
    for (const auto& fdec : e)
      visit_function_dec_header(*fdec);

    for (const auto& fdec : e)
<<<<<<< HEAD
=======
      // There is nothing to translate for primitives.
>>>>>>> 2028-tc-EXTS.0
      if (fdec->body_get())
        visit_function_dec_body(*fdec);
  }

  void Translator::visit_function_dec_header(const ast::FunctionDec& e)
  {
    bool is_main = e.name_get() == "_main";
    bool is_primitive = e.body_get() == nullptr;
    auto name = function_dec_name(e);

<<<<<<< HEAD
    const type::Type* node_type = e.type_get();
    auto& function_type = static_cast<const type::Function&>(*node_type);
    auto function_ltype = llvm_function_type(function_type);

=======
    const type::Type* node_type = nullptr;
    // FIXME: Some code was deleted here.
    auto& function_type = static_cast<const type::Function&>(*node_type);
    auto function_ltype = llvm_function_type(function_type);

    // Main and primitives have External linkage.
    // Other Tiger functions are treated as "static" functions in C.
>>>>>>> 2028-tc-EXTS.0
    auto linkage = is_main || is_primitive ? llvm::Function::ExternalLinkage
                                           : llvm::Function::InternalLinkage;

    auto the_function =
      llvm::Function::Create(function_ltype, linkage, name, &module_);
    set_default_attributes(*the_function, e);

    auto& escaped = escaped_[&function_type];

<<<<<<< HEAD
=======
    // Name each argument of the function
>>>>>>> 2028-tc-EXTS.0
    for (auto arg_it = the_function->arg_begin();
         arg_it != the_function->arg_end(); ++arg_it)
      {
        auto i = std::distance(the_function->arg_begin(), arg_it);
        auto var = escaped.size() && static_cast<size_t>(i) < escaped.size()
          ? *std::next(escaped_[&function_type].begin(), i)
          : e.formals_get()[i - escaped.size()];

        arg_it->setName(var->name_get().get());
      }
  }

  void Translator::visit_function_dec_body(const ast::FunctionDec& e)
  {
    auto the_function = module_.getFunction(function_dec_name(e));

<<<<<<< HEAD
=======
    // Save the old function in case a nested function occurs.
>>>>>>> 2028-tc-EXTS.0
    auto old_insert_point = builder_.saveIP();
    auto old_function = current_function_;
    current_function_ = the_function;

<<<<<<< HEAD
=======
    // Create a new basic block to start the function.
>>>>>>> 2028-tc-EXTS.0
    auto bb = llvm::BasicBlock::Create(ctx_, "entry_"s + e.name_get().get(),
                                       the_function);
    builder_.SetInsertPoint(bb);

<<<<<<< HEAD
    const type::Type* node_type = e.type_get();
=======
    const type::Type* node_type = nullptr;
    // FIXME: Some code was deleted here.
>>>>>>> 2028-tc-EXTS.0
    auto& function_type = static_cast<const type::Function&>(*node_type);
    auto& escaped = escaped_[&function_type];
    auto& formals = e.formals_get();

    auto arg_it = the_function->arg_begin();

    for (const auto var : escaped)
      {
        locals_[current_function_][var] = &*arg_it;
        ++arg_it;
      }

<<<<<<< HEAD
    for (const auto* formal : formals)
      {
        auto var_ltype = llvm_type(*formal->type_get());
        auto alloca =
          create_alloca(the_function, var_ltype, formal->name_get().get());
        builder_.CreateStore(&*arg_it, alloca);
        locals_[current_function_][formal] = alloca;
        ++arg_it;
      }

    auto body_val = translate(*e.body_get());

    if (dynamic_cast<const type::Void*>(&function_type.result_get().actual()))
      builder_.CreateRetVoid();
    else
      builder_.CreateRet(body_val);

    llvm::verifyFunction(*the_function);

=======
    // FIXME: Some code was deleted here (Create alloca instructions for each variable).

    // FIXME: Some code was deleted here (Create a return instruction).

    // Validate the generated code, checking for consistency.
    llvm::verifyFunction(*the_function);

    // Restore the context of the old function.
>>>>>>> 2028-tc-EXTS.0
    current_function_ = old_function;
    builder_.restoreIP(old_insert_point);
  }

  void Translator::operator()(const ast::CallExp& e)
  {
<<<<<<< HEAD
    const auto* fdec = e.def_get();
    bool is_primitive = fdec->body_get() == nullptr;
    auto name = is_primitive ? "tc_" + e.name_get().get() : e.name_get().get();
    if (e.name_get() == "_main")
      name = "tc_main";

    auto the_function = module_.getFunction(name);

    auto& func_type = static_cast<const type::Function&>(*fdec->type_get());
    auto& escaped = escaped_[&func_type];

    std::vector<llvm::Value*> args;

    for (const auto var : escaped)
      args.emplace_back(locals_[current_function_][var]);

    for (const auto& arg : e.args_get())
      args.emplace_back(translate(*arg));

    bool is_void =
      dynamic_cast<const type::Void*>(&func_type.result_get().actual()) !=
      nullptr;

    auto call = builder_.CreateCall(the_function, args, is_void ? "" : "calltmp");
    if (is_void)
      value_ = llvm::ConstantInt::getSigned(i64_t(ctx_), 0);
    else
      value_ = call;
=======
    // Look up the name in the global module table.
    // If it's a primitive, rename the call to tc_name.
    //
    // Then, add the escaped variables and the rest of the arguments to the
    // list of arguments, and return the correct value.
    // FIXME: Some code was deleted here.
>>>>>>> 2028-tc-EXTS.0
  }

  void Translator::operator()(const ast::VarDec& e)
  {
<<<<<<< HEAD
    if (dynamic_cast<const type::Void*>(&e.type_get()->actual()))
      {
        locals_[current_function_][&e] = nullptr;
        if (e.init_get())
          translate(*e.init_get());
        value_ = llvm::ConstantInt::getSigned(i64_t(ctx_), 0);
        return;
      }
    auto var_ltype = llvm_type(*e.type_get());
    auto alloca =
      create_alloca(current_function_, var_ltype, e.name_get().get());
    locals_[current_function_][&e] = alloca;
    if (e.init_get())
      {
        auto init_val = translate(*e.init_get());
        builder_.CreateStore(init_val, alloca);
      }
    value_ = llvm::ConstantInt::getSigned(i64_t(ctx_), 0);
=======
    // Void var types are actually Ints represented by a 0
    // FIXME: Some code was deleted here.
>>>>>>> 2028-tc-EXTS.0
  }

} // namespace llvmtranslate
