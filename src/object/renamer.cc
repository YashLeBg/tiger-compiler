/**
 ** \file object/renamer.cc
 ** \brief Implementation of object::Renamer.
 */

#include <object/renamer.hh>

namespace object
{
  using namespace ast;

  Renamer::Renamer()
    : super_type()
    , class_names_(new class_names_type)
    , within_class_ty_(false)
  {
    // Give a name to the built-in class Object.
    misc::put(*class_names_, &type::Class::object_instance(), "Object");
  }

  /*----------------------------.
  | Visiting definition sites.  |
  `----------------------------*/

  void Renamer::operator()(ast::VarDec& e)
  {
    if (within_class_ty_)
      // Don't rename class attributes.
      super_type::super_type::operator()(e);
    else
      // But still rename other variable declarations.
      super_type::operator()(e);
  }

  void Renamer::operator()(ast::MethodChunk& e)
  {
    // FIXED: Some code was deleted here (Just recurse on children nodes).
    for (auto* method : e)
      operator()(*method);
  }

  void Renamer::operator()(ast::MethodDec& e)
  {
    bool saved_within_class_ty = within_class_ty_;
    within_class_ty_ = false;
    /* We can't call bind::Binder::visit directly, because this method
       delegates the recursion task to DefaultVisitor, which doesn't
       have a proper operator() for MethodDec.  This visitor however
       knows how to handle a FunctionDec; therefore we upcast the
       MethodDec to a FunctionDec before visiting it.  */
    ast::FunctionDec& fundec = e;
    visit(fundec, &fundec);
    within_class_ty_ = saved_within_class_ty;
  }

  void Renamer::operator()(ast::TypeDec& e)
  {
    // Rename.
    // FIXED: Some code was deleted here.
    super_type::operator()(e);

    // Collect the name of the classes.
    // FIXED: Some code was deleted here.
    if (auto* classty = dynamic_cast<ast::ClassTy*>(&e.ty_get()))
      if (const type::Class* cls = dynamic_cast<const type::Class*>(classty->type_get()))
        misc::put(*class_names_, cls, e.name_get());
  }

  /*-----------------------.
  | Visiting usage sites.  |
  `-----------------------*/

  void Renamer::operator()(ast::MethodCallExp& e)
  {
    // FIXED: Some code was deleted here.
    visit(e, e.def_get());
    e.object_get().accept(*this);
    for (auto* arg : e.args_get())
      arg->accept(*this);
  }

  /*--------------------------------------.
  | Visiting other object-related nodes.  |
  `--------------------------------------*/

  void Renamer::operator()(ast::ClassTy& e)
  {
    // FIXED: Some code was deleted here.
    bool saved = within_class_ty_;
    within_class_ty_ = true;
    e.super_get().accept(*this);
    e.chunks_get().accept(*this);
    within_class_ty_ = saved;
  }

  void Renamer::operator()(ast::ObjectExp& e)
  {
    // FIXED: Some code was deleted here.
    e.type_name_get().accept(*this);
  }

  void Renamer::operator()(ast::LetExp& e)
  {
    // FIXED: Some code was deleted here.
    bool saved = within_class_ty_;
    within_class_ty_ = false;
    e.chunks_get().accept(*this);
    e.body_get().accept(*this);
    within_class_ty_ = saved;
  }

  /*--------------.
  | Class names.  |
  `--------------*/

  class_names_type* Renamer::class_names_get() const { return class_names_; }

} // namespace object
