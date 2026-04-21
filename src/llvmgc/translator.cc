/**
 ** \file llvmgc/translator.cc
 ** \brief Implementation of llvmgc::GCTranslator.
 **
 ** This override of llvmtranslate::Translator may be used to generate more gc-friendly LLVM-IR.
 ** For example, the provided overrides of the translation of ast::RecordExp and ast::StringExp
 ** can be used to insert runtime metadata in the objects.
 **
 ** NOTE:
 ** This Translator is optional, and a GC can be implemented without using it.
 **/

#include <llvm/IR/Attributes.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvmgc/libllvmgc.hh>
#include <llvmgc/translator.hh>
#include <misc/symbol.hh>

namespace llvmgc
{
  using namespace std::string_literals;

  namespace
  {

    // Shorthands for types.
    inline llvm::IntegerType* i64_t(llvm::LLVMContext& ctx)
    {
      return llvm::Type::getInt64Ty(ctx);
    }

    inline llvm::IntegerType* i8_t(llvm::LLVMContext& ctx)
    {
      return llvm::Type::getInt8Ty(ctx);
    }

    inline llvm::PointerType* string_t(llvm::LLVMContext& ctx,
                                       unsigned addr_space)
    {
      return llvm::PointerType::get(i8_t(ctx), addr_space);
    }

  } // namespace

  GCTranslator::GCTranslator(llvm::Module& module,
                             llvmtranslate::escaped_map_type&& escaped)
    : GCTranslator::super_type(module, std::move(escaped))
  {}

  void GCTranslator::operator()(const ast::StringExp& e)
  {
    // Strings are translated as a struct holding `struct metadata, [i8 * N]` values. The [i8 * N] array is returned as an i8 like C's `char*`.
    auto nb_chars = e.value_get().size() + 1;

    // Create the constant char array type for this string
    auto char_array_t = llvm::ArrayType::get(i8_t(ctx_), nb_chars);

    // Create the actual string
    std::vector<llvm::Constant*> init_string;

    init_string.reserve(nb_chars);

    for (const char c : e.value_get() + '\0')
      init_string.push_back(llvm::ConstantInt::get(i8_t(ctx_), c));

    auto inner_array = llvm::ConstantArray::get(char_array_t, init_string);

    std::vector<llvm::Constant*> struct_fields;
    std::vector<llvm::Type*> struct_types;

    // FIXME: Some code was deleted here (Create the fields of the struct and their types).

    // Create the fat pointer struct { struct metadata, char array } following the gc's expected metadata format
    llvm::StructType* str_ty_(llvm::StructType::create(ctx_));

    str_ty_->setBody(struct_types, false);

    // Initialize the struct with its fields
    auto* init = llvm::ConstantStruct::get(str_ty_, struct_fields);

    auto struct_ptr = new llvm::GlobalVariable(
      module_, str_ty_, true, llvm::Function::InternalLinkage, init,
      "the_global_" + misc::symbol::fresh().get());

    int idx = -1;

    // FIXME: Some code was deleted here (Set idx to the index of the char array in the struct).

    /// Get a pointer to the field of the char array
    value_ = builder_.CreateStructGEP(str_ty_, struct_ptr, idx, "the_string");

    /// Convert the [i8 * N]* to i8*
    value_ = builder_.CreatePointerCast(value_, string_t(ctx_, 1), "ptr_cast");
  }

  void GCTranslator::operator()(const ast::RecordExp& e)
  {
    // Get the record type
    const type::Record* record_type = nullptr;
    // FIXME: Some code was deleted here.

    // Type the record and use get_record_ltype() to get its LLVM type
    llvm_type(*record_type);
    auto struct_ltype = type_visitor_.get_record_ltype(record_type);

    std::vector<llvm::Type*> arg_type;
    std::vector<llvm::Value*> arg_vals;

    // FIXME: Some code was deleted here (Build the proper arguments for the call to tc_init_record function (see llvmgc/runtime/gc-alloc.h)).

    // Then, create the FunctionType.
    auto init_record_ltype =
      llvm::FunctionType::get(llvm::PointerType::get(ctx_, 1), arg_type, false);

    // Get the llvm::Function from the module related to the name and type
    auto init_record_function =
      module_.getOrInsertFunction("tc_init_record", init_record_ltype);

    // Create the call.
    auto init_record_call =
      builder_.CreateCall(init_record_function, arg_vals, "init_record_call");

    // Init the fields
    // FIXME: Some code was deleted here.

    value_ = init_record_call;
  }

} // namespace llvmgc
