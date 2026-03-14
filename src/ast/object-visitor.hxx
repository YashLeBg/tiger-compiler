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
    this->accept(&e.super_get());
    this->accept(&e.chunks_get());
  }
  //visite les declarations de methode dans le chunk
  template <template <typename> class Const>
  void GenObjectVisitor<Const>::operator()(const_t<MethodChunk>& e)
  {
    for (auto meth : e)
      this->accept(meth);
  }
  //visite les parametres le type de retour et le body de la methode
  template <template <typename> class Const>
  void GenObjectVisitor<Const>::operator()(const_t<MethodDec>& e)
  {
    this->accept(&e.formals_get());
    if (e.result_get())
      this->accept(e.result_get());
    this->accept(e.body_get());
  }
  //visite l objet et les arguments de l appel de methode
  template <template <typename> class Const>
  void GenObjectVisitor<Const>::operator()(const_t<MethodCallExp>& e)
  {
    this->accept(&e.object_get());
    for (auto arg : e.args_get())
      this->accept(arg);
  }
  //visite le nom de classe et les arguments du nouvel objet
  template <template <typename> class Const>
  void GenObjectVisitor<Const>::operator()(const_t<ObjectExp>& e)
  {
    this->accept(&e.type_name_get());
  }

} // namespace ast
