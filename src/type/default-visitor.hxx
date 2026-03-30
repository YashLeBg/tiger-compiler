/**
 ** \file type/default-visitor.hxx
 ** \brief Implementation for type/default-visitor.hh.
 */

#pragma once

#include <misc/algorithm.hh>
#include <type/class.hh>
#include <type/default-visitor.hh>
#include <type/types.hh>

namespace type
{
  template <template <typename> class Const>
  GenDefaultVisitor<Const>::GenDefaultVisitor()
    : GenVisitor<Const>()
  {}

  template <template <typename> class Const>
  GenDefaultVisitor<Const>::~GenDefaultVisitor() = default;

  template <template <typename> class Const>
  void GenDefaultVisitor<Const>::operator()(const_t<Nil>&)
  {}

  template <template <typename> class Const>
  void GenDefaultVisitor<Const>::operator()(const_t<Void>&)
  {}

  template <template <typename> class Const>
  void GenDefaultVisitor<Const>::operator()(const_t<Int>&)
  {}

  template <template <typename> class Const>
  void GenDefaultVisitor<Const>::operator()(const_t<String>&)
  {}

  template <template <typename> class Const>
  void GenDefaultVisitor<Const>::operator()(const_t<Named>& e)
  {
    if (e.type_get())
      e.type_get()->accept(*this);
  }

  template <template <typename> class Const>
  void GenDefaultVisitor<Const>::operator()(const_t<Array>& e)
  {
    e.type_get().accept(*this);
  }

  template <template <typename> class Const>
  void GenDefaultVisitor<Const>::operator()(const_t<Record>& e)
  {
    for (const auto& field : e)
      field.type_get().accept(*this);
  }

  template <template <typename> class Const>
  void GenDefaultVisitor<Const>::operator()(const_t<Class>& e)
  {
    if (e.super_get())
      e.super_get()->accept(*this);
    for (const auto& attr : e.attrs_get())
      attr.type_get().accept(*this);
    for (const auto& meth : e.meths_get())
      meth->accept(*this);
  }

  template <template <typename> class Const>
  void GenDefaultVisitor<Const>::operator()(const_t<Function>& e)
  {
    e.formals_get().accept(*this);
    e.result_get().accept(*this);
  }

  template <template <typename> class Const>
  void GenDefaultVisitor<Const>::operator()(const_t<Method>& e)
  {
    e.Function::accept(*this);
  }

} // namespace type
