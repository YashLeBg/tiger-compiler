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
    // FIXME: Some code was deleted here.
  }

  template <typename NodeType>
  void TypeChecker::created_type_default(NodeType& e, const type::Type* type)
  {
    // FIXME: Some code was deleted here.
  }

  template <typename NodeType>
  void TypeChecker::type_set(NodeType& e, const type::Type* type)
  {
    // FIXME: Some code was deleted here (Basically e.type_set(type)).
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
    loc.type_set(nil_error_);
  }

  template <typename NodeType>
  void TypeChecker::check_type(NodeType& e,
                               const std::string& s,
                               const Type& expected)
  {
    // FIXME: Some code was deleted here.
  }

  /*------------.
  | Functions.  |
  `------------*/

  template <typename Routine_Type, typename Routine_Node>
  void TypeChecker::visit_routine_body(Routine_Node& e)
  {
    // FIXME: Some code was deleted here.
  }

} // namespace type
