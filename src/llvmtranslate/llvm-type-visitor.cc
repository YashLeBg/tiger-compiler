/**
 ** \file llvmtranslate/llvm-type-visitor.cc
 ** \brief Implementation of llvmtranslator::LLVMTypeVisitor.
 */

#include <llvm/IR/DerivedTypes.h>

#include <llvmtranslate/llvm-type-visitor.hh>
#include <type/types.hh>

namespace llvmtranslate
{
  LLVMTypeVisitor::LLVMTypeVisitor(llvm::LLVMContext& ctx)
    : ctx_{ctx}
  {}

  llvm::Type* LLVMTypeVisitor::llvm_type_get() { return type_; }

  llvm::Type* LLVMTypeVisitor::get_record_ltype(const type::Record* e)
  {
    return structs_[e];
  }

  llvm::Type* LLVMTypeVisitor::llvm_type(const type::Type& type)
  {
    operator()(type);
    return type_;
  }

  void LLVMTypeVisitor::operator()(const type::Nil&)
  {
    type_ = llvm::PointerType::get(ctx_, 1);
  }

  void LLVMTypeVisitor::operator()(const type::Void&)
  {
<<<<<<< HEAD
    // FIXED: Some code was deleted here (Void types can be interpreted as int or void type).
    type_ =llvm::Type::getVoidTy(ctx_);
=======
    // FIXME: Some code was deleted here (Void types can be interpreted as int or void type).
>>>>>>> 2028-tc-EXTS.0
  }

  void LLVMTypeVisitor::operator()(const type::Int&)
  {
    type_ = llvm::Type::getInt64Ty(ctx_);
  }

  void LLVMTypeVisitor::operator()(const type::String&)
  {
    // Strings are pointers to characters in LLVM.
<<<<<<< HEAD
    // FIXED: Some code was deleted here.
    type_ = llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(ctx_));
=======
    // FIXME: Some code was deleted here.
>>>>>>> 2028-tc-EXTS.0
  }

  void LLVMTypeVisitor::operator()(const type::Named& e)
  {
    // FIXME: Some code was deleted here.
<<<<<<< HEAD
    type_=llvm_type(*e.type_get());
=======
>>>>>>> 2028-tc-EXTS.0
  }

  void LLVMTypeVisitor::operator()(const type::Record& e)
  {
    // If the record was never translated, translate it
    if (!structs_[&e])
      {
        // We need to create the struct in two passes to support recursive
        // types, like 'type a = { next : a }
        // So, first we create an empty struct
        structs_[&e] = llvm::StructType::create(ctx_);
        // Then set the body of the structure
        std::vector<llvm::Type*> field_types;
        field_types.reserve(e.fields_get().size());
<<<<<<< HEAD
        // FIXed: Some code was deleted here.
        for (const auto& each:e.fields_get())
          {
            field_types.push_back(llvm_type(each.type_get()));
          }
=======
        // FIXME: Some code was deleted here.
>>>>>>> 2028-tc-EXTS.0
        structs_[&e]->setBody(std::move(field_types), false);
      }

    type_ = llvm::PointerType::get(structs_[&e], 1);
  }

  void LLVMTypeVisitor::operator()(const type::Array& e)
  {
    // Arrays are pointers to the array elements, like in C.
    // FIXME: Some code was deleted here.
<<<<<<< HEAD
    type_ = llvm::PointerType::getUnqual(llvm_type(e.type_get()));
=======
>>>>>>> 2028-tc-EXTS.0
  }

} // namespace llvmtranslate
