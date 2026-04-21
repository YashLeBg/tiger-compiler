/**
 ** \file object/binder.cc
 ** \brief Implementation of object::Binder.
 */

#include <ast/all.hh>
#include <object/binder.hh>

namespace object
{
  /*---------.
  | Visits.  |
  `---------*/

  /* Handle the case of `self'. If the variable is not named `self', handle it
     like the normal `Binder'.  If it is `self', it must be bound to a definiton
     site, unless:
     * it is inside a method,
     * AND `self` is not overridden.
     If those conditions are met, `self' is an implicitly defined instance of
     the class.

     Variable `self' will have a meaningful definition after the object
     constructs have been desugared. */

  void Binder::operator()(ast::SimpleVar& e)
  {
    // FIXED: Some code was deleted here.
    if (e.name_get() != "self")
      {
        super_type::operator()(e);
        return;
      }
    if (within_method_dec_ && !overrided_self_)
      e.def_set(nullptr);
    else
      super_type::operator()(e);
  }

  // Handle the case of `Object'.
  void Binder::operator()(ast::NameTy& e)
  {
    // FIXED: Some code was deleted here.
    if (e.name_get() == "Object")
      return;
    super_type::operator()(e);
  }

  /*---------------.
  | Visiting Exp.  |
  `---------------*/

  void Binder::operator()(ast::ForExp& e)
  {
    bool saved_within_class_ty = within_class_ty_;
    within_class_ty_ = false;
    super_type::operator()(e);
    within_class_ty_ = saved_within_class_ty;
  }

  /*-------------------.
  | Visiting ClassTy.  |
  `-------------------*/

  void Binder::operator()(ast::ClassTy& e)
  {
    // FIXED: Some code was deleted here (Scope begins).
    scope_begin();
    e.super_get().accept(*this);
    bool saved_within_class_ty = within_class_ty_;
    within_class_ty_ = true;
    bool saved_within_method_dec = within_method_dec_;
    within_method_dec_ = false;
    e.chunks_get().accept(*this);
    within_method_dec_ = saved_within_method_dec;
    within_class_ty_ = saved_within_class_ty;
    // FIXED: Some code was deleted here (Scope ends).
    scope_end();
  }

  /*---------------.
  | Visiting Dec.  |
  `---------------*/

  void Binder::operator()(ast::VarDec& e)
  {
    if (within_class_ty_)
      {
        within_class_ty_ = false;
        // Don't bind class attributes.
        super_type::super_type::operator()(e);
        within_class_ty_ = true;
      }
    else
      {
        // But still bind other variable declarations.
        super_type::operator()(e);
        if (e.name_get() == "self" && within_method_dec_)
          overrided_self_ = true;
      }
  }

  void Binder::operator()(ast::FunctionChunk& e)
  {
    chunk_visit<ast::FunctionDec>(e);
  }

  template <class D> void Binder::chunk_visit(ast::Chunk<D>& e)
  {
    // FIXED: Some code was deleted here (Two passes: once on headers, then on bodies).
    for (auto* dec : e)
      visit_dec_header(*dec);
    for (auto* dec : e)
      visit_dec_body(*dec);
  }

  // This trampoline is needed, since `virtual' and `template' cannot
  // be mixed.
  template <>
  void Binder::visit_dec_header<ast::FunctionDec>(ast::FunctionDec& e)
  {
    // FIXED: Some code was deleted here (Call the super type).
    funcs_.put(e.name_get(), &e);
  }

  // Compute the bindings of this function's body.
  template <> void Binder::visit_dec_body<ast::FunctionDec>(ast::FunctionDec& e)
  {
    bool saved_within_class_ty = within_class_ty_;
    within_class_ty_ = false;
    bool saved_within_method_dec = within_method_dec_;
    within_method_dec_ = false;
    // FIXED: Some code was deleted here (Call the super type).
    super_type::operator()(e);
    within_method_dec_ = saved_within_method_dec;
    within_class_ty_ = saved_within_class_ty;
  }

  /* We can't bind methods definitions without types, so we don't
     store them.  Nonetheless, object::Binder must still recurse
     through the children of ast::MethodChunk to bind other names.

     Note that as we defer the binding of methods to the
     type-checkimg, there is no need to visit method in two-pass (like
     bind::Binder does for functions for instance).  */
  void Binder::operator()(ast::MethodDec& e)
  {
    // FIXED: Some code was deleted here (Scope begins).
    scope_begin();
    bool saved_within_class_ty = within_class_ty_;
    within_class_ty_ = false;
    bool saved_within_method_dec = within_method_dec_;
    within_method_dec_ = true;
    bool saved_overrided_self = overrided_self_;
    overrided_self_ = false;
    ast::DefaultVisitor::operator()(static_cast<ast::FunctionDec&>(e));
    within_method_dec_ = saved_within_method_dec;
    within_class_ty_ = saved_within_class_ty;
    overrided_self_ = saved_overrided_self;
    // FIXED: Some code was deleted here (Scope ends).
    scope_end();
  }

} // namespace object
