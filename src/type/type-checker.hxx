/**
 ** \file type/type-checker.hxx
 ** \brief Inline methods of type::TypeChecker.
 */

#pragma once

#include <ast/all.hh>
#include <type/pretty-printer.hh>
#include <type/type-checker.hh>
#include <type/types.hh>

namespace type
{

  /*----------------.
  | Setting types.  |
  `----------------*/

  template <typename NodeType>
  void TypeChecker::type_default(NodeType& e, const type::Type* type)
  {
    // FIXED: Some code was deleted here.
    if (!e.type_get()) {
      type_set(e,type);
    }
  }

  template <typename NodeType>
  void TypeChecker::created_type_default(NodeType& e, const type::Type* type)
  {
    // FIXED: Some code was deleted here.
    if (!e.type_get()) {
      type_set(e, type);
    }
  }

  template <typename NodeType>
  void TypeChecker::type_set(NodeType& e, const type::Type* type)
  {
    // FIXED: Some code was deleted here (Basically e.type_set(type)).
    e.type_set(type);
  }

  /*-----------------.
  | Error handling.  |
  `-----------------*/

  template <typename T>
  void
  TypeChecker::error(const ast::Ast& ast, const std::string& msg, const T& exp)
  {
    error_ << misc::error::error_type::type << ast.location_get() << ": " << msg
           << ": " << exp << std::endl;
  }

  template <typename T, typename U>
  void
  TypeChecker::error_and_recover(T& loc, const std::string& msg, const U& exp)
  {
    error(loc, msg, exp);

    const type::Type* nil_error_ = nullptr;
    // FIXME: Some code was deleted here (Get the Nil type).
    nil_error_ = &Nil::instance();
    loc.type_set(nil_error_);
  }

  template <typename NodeType>
  void TypeChecker::check_type(NodeType& e,
                               const std::string& s,
                               const Type& expected)
  {
    // FIXED: Some code was deleted here.
    const Type* cur = type(e);
    if (!cur->compatible_with(expected)) {
      type_mismatch(e,s,*cur,"expected type",expected);
    }
  }

  /*------------.
  | Functions.  |
  `------------*/

  template <typename Routine_Type, typename Routine_Node>
  void TypeChecker::visit_routine_body(Routine_Node& e)
  {
    // FIXME: Some code was deleted here.
    venv_.scope_begin();
    for (const auto& f : e.formals_get())
      f->accept(*this);
    const Type* body = type(*e.body_get());
    const Type& curr_type = e.type_get()->actual();
    const Type& res = dynamic_cast<const Routine_Type&>(curr_type).result_get();
    check_types(*e.body_get(), "result", *body, "expected", res);
    venv_.scope_end();
  }

} // namespace type
