/**
 ** \file ast/object-visitor.hxx
 ** \brief Implementation for ast/object-visitor.hh.
 */

#pragma once

#include <ast/all.hh>
#include <ast/object-visitor.hh>
#include <misc/contract.hh>

namespace ast
{
  template <template <typename> class Const>
  GenObjectVisitor<Const>::GenObjectVisitor()
    : GenVisitor<Const>()
  {}

  template <template <typename> class Const>
  GenObjectVisitor<Const>::~GenObjectVisitor()
  {}

  /*-------------------------------.
  | Object-related visit methods.  |
  `-------------------------------*/
  //visite la classe parent ses champs et ses methodes
  template <template <typename> class Const>
  void GenObjectVisitor<Const>::operator()(const_t<ClassTy>& e)
  {
    this->accept(e.super_get());
    e.fields_get().accept(*this);
    e.methods_get().accept(*this);
  }
  //visite les declarations de methode dans le chunk
  template <template <typename> class Const>
  void GenObjectVisitor<Const>::operator()(const_t<MethodChunk>& e)
  {
    e.formals_get().accept(*this);
    this->accept(e.result_get());
    this->accept(e.body_get());
  }
  //visite les parametres le type de retour et le body de la methode
  template <template <typename> class Const>
  void GenObjectVisitor<Const>::operator()(const_t<MethodDec>& e)
  {
    e.object_get().accept(*this);
    for (auto* arg : e.args_get())
      arg->accept(*this);
  }
  //visite l objet et les arguments de l appel de methode
  template <template <typename> class Const>
  void GenObjectVisitor<Const>::operator()(const_t<MethodCallExp>& e)
  {
    e.class_name_get().accept(*this);
    for (auto* arg : e.args_get())
      arg->accept(*this);
  }
  //visite le nom de classe et les arguments du nouvel objet
  template <template <typename> class Const>
  void GenObjectVisitor<Const>::operator()(const_t<ObjectExp>& e)
  {
    e.object_get().accept(*this);
  }

} // namespace ast
