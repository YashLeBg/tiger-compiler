/**
 ** \file type/type-checker.cc
 ** \brief Implementation for type/type-checker.hh.
 */

#include "type-checker.hh"

#include <memory>
#include <ranges>

#include <ast/all.hh>
#include <type/type-checker.hh>
#include <type/types.hh>

namespace type
{
  TypeChecker::TypeChecker()
    : super_type()
    , error_()
  {}

  const Type* TypeChecker::type(ast::Typable& e)
  {
    // FIXED: Some code was deleted here.
    e.accept(*this);
    return e.type_get();
  }

  const Record* TypeChecker::type(const ast::fields_type& e)
  {
    auto res = new Record;
    // FIXED: Some code was deleted here.
    for (const auto& f:e) {
      f->type_name_get().accept(*this);
      res->field_add(f->name_get(),*f->type_name_get().type_get());
    }
    return res;
  }

  const Record* TypeChecker::type(const ast::VarChunk& e)
  {
    auto res = new Record;
    for (const auto& var : e)
      res->field_add(var->name_get(), *type(*var));

    return res;
  }

  const misc::error& TypeChecker::error_get() const { return error_; }

  /*-----------------.
  | Error handling.  |
  `-----------------*/

  void TypeChecker::error(const ast::Ast& ast, const std::string& msg)
  {
    error_ << misc::error::error_type::type << ast.location_get() << ": " << msg
           << std::endl;
  }

  void TypeChecker::type_mismatch(const ast::Ast& ast,
                                  const std::string& exp1,
                                  const Type& type1,
                                  const std::string& exp2,
                                  const Type& type2)
  {
    error_ << misc::error::error_type::type << ast.location_get()
           << ": type mismatch" << misc::incendl << exp1 << " type: " << type1
           << misc::iendl << exp2 << " type: " << type2 << misc::decendl;
  }

  void TypeChecker::check_types(const ast::Ast& ast,
                                const std::string& exp1,
                                const Type& type1,
                                const std::string& exp2,
                                const Type& type2)
  {
    // FIXME: Some code was deleted here (Check for type mismatch).
    if (!type1.compatible_with(type2)) {
      type_mismatch(ast, exp1, type1, exp2, type2);
    }
  }

  void TypeChecker::check_types(const ast::Ast& ast,
                                const std::string& exp1,
                                ast::Typable& type1,
                                const std::string& exp2,
                                ast::Typable& type2)
  {
    // Ensure evaluation order.
    type(type1);
    type(type2);
    // FIXME: Some code was deleted here (Check types).
    check_types(ast,exp1,*type(type1),exp2, *type(type2));
  }

  /*--------------------------.
  | The core of the visitor.  |
  `--------------------------*/

  /*-----------------.
  | Visiting /Var/.  |
  `-----------------*/

  void TypeChecker::operator()(ast::SimpleVar& e)
  {
    // FIXME: Some code was deleted here.
  }

  // FIXME: Some code was deleted here.

  /*-----------------.
  | Visiting /Exp/.  |
  `-----------------*/

  // Literals.
  void TypeChecker::operator()(ast::NilExp& e)
  {
    // FIXME: Some code was deleted here.
  }

  void TypeChecker::operator()(ast::IntExp& e)
  {
    // FIXME: Some code was deleted here.
  }

  void TypeChecker::operator()(ast::StringExp& e)
  {
    // FIXME: Some code was deleted here.
  }

  // Complex values.

  void TypeChecker::operator()(ast::RecordExp& e)
  {
    // FIXED: Some code was deleted here.
    e.type_name_get().accept(*this);
    const auto* t = &e.type_name_get().type_get()->actual();
    const auto* record = dynamic_cast<const Record*>(t);
    if (!record)
    {
      error(e, "not a record type", *e.type_name_get().name_get());
      type_set(e, &Void::instance());
      return;
    }
    for (const auto& field : e.fields_get())
    {
      const Type* exp = record->field_type(field->name_get());
      if (!exp)
        error(*field, "unexpected field", field->name_get());
      else
        check_types(field->init_get(), "field", *this->type(field->init_get()), "expected", *exp);
    }
    type_set(e, record);
  }

  void TypeChecker::operator()(ast::OpExp& e)
  {
    // FIXME: Some code was deleted here.
  }

  // FIXED: Some code was deleted here.
  void TypeChecker::operator()(ast::LetExp& e) {
    venv_.scope_begin();
    tenv_.scope_begin();
    e.chunks_get().accept(*this);
    type_set(e, type(e.body_get()));
    tenv_.scope_end();

    venv_.scope_end();
  }

  void TypeChecker::operator()(ast::ArrayExp& e)
  {
    e.type_name_get().accept(*this);
    const Type* res = &e.type_name_get().type_get()->actual();
    const Array* arr = dynamic_cast<const Array*>(res);
    if (!arr)
    {
      error(e, "not an array type", *e.type_name_get().name_get());
      type_set(e, &Void::instance());
      return;
    }
    check_type(e.size_get(), "size", Int::instance());
    check_types(e.init_get(), "init", *type(e.init_get()),"type", arr->type_get());
    type_set(e, arr);
  }

  void TypeChecker::operator()(ast::CallExp& e)
  {
    const auto* f = dynamic_cast<const Function*>(venv_.look(e.name_get()));
    if (!f)
    {
      error(e, "undeclared function", e.name_get());
      type_set(e, &Void::instance());
      return;
    }
    const auto& form = f->formals_get();
    const auto& argv = e.args_get();
    auto it = form.begin();
    auto arg_it = argv.begin();
    for (int i = 0; it != form.end() && arg_it != argv.end(); ++it, ++arg_it) {
      check_types(**arg_it, "arg", *type(**arg_it), "formal", it->type_get());
      i++;
    }
    if (it != form.end() || arg_it != argv.end()) {
      error(e, "argument count mismatch");
    }
    type_set(e, &f->result_get());
  }

  void TypeChecker::operator()(ast::ForExp& e)
  {
    check_type(e.vardec_get().init_get(), "lower bound", Int::instance());

    check_type(e.hi_get(), "upper bound", Int::instance());
    venv_.scope_begin();

    e.vardec_get().accept(*this);
    var_read_only_.insert(&e.vardec_get());
    type(e.body_get());
    type_set(e, &Void::instance());
    venv_.scope_end();
  }

  /*-----------------.
  | Visiting /Dec/.  |
  `-----------------*/

  /*------------------------.
  | Visiting FunctionChunk. |
  `------------------------*/

  void TypeChecker::operator()(ast::FunctionChunk& e)
  {
    chunk_visit<ast::FunctionDec>(e);
  }

  void TypeChecker::operator()(ast::FunctionDec&)
  {
    // We must not be here.
    unreachable();
  }

  // Store the type of this function.
  template <>
  void TypeChecker::visit_dec_header<ast::FunctionDec>(ast::FunctionDec& e)
  {
    // FIXED: Some code was deleted here.
    const Type* res = &Void::instance();
    if (e.result_get())
    {
      e.result_get()->accept(*this);
      res = e.result_get()->type_get();
    }
    const Record* f = type(e.formals_get());
    auto func = new Function(f,*res);

    type_set(e,func);
    venv_.push(e.name_get(),func);
  }

  // Type check this function's body.
  template <>
  void TypeChecker::visit_dec_body<ast::FunctionDec>(ast::FunctionDec& e)
  {
    if (e.body_get())
      visit_routine_body<Function>(e);
  }

  /*---------------.
  | Visit VarDec.  |
  `---------------*/

  void TypeChecker::operator()(ast::VarDec& e)
  {
    // FIXED: Some code was deleted here.
    const Type* init = nullptr;
    if (e.init_get()) {
      init = type(*e.init_get());
    }
    const Type* t = nullptr;
    if (e.type_name_get()) {
      e.type_name_get()->accept(*this);
      t = e.type_name_get()->type_get();
    }
    if (t && init) {
      check_types(e, "initialization", *init, "declared type", *t);
    }
    if (!t && init && init->actual() == Nil::instance()) {
      error(e,"nil can't be used");
    }
    const Type* type = init;
    if (t) {
      type = t;
    }
    type_set(e,type);
    venv_.push(e.name_get(),type);
  }

  /*--------------------.
  | Visiting TypeChunk. |
  `--------------------*/

  void TypeChecker::operator()(ast::TypeChunk& e)
  {
    chunk_visit<ast::TypeDec>(e);
  }

  void TypeChecker::operator()(ast::TypeDec&)
  {
    // We must not be here.
    unreachable();
  }

  // Store this type.
  template <> void TypeChecker::visit_dec_header<ast::TypeDec>(ast::TypeDec& e)
  {
    // We only process the head of the type declaration, to set its
    // name in E.  A declaration has no type in itself; here we store
    // the type declared by E.
    // FIXED: Some code was deleted here.
    auto name = new Named(e.name_get());
    tenv_.push(e.name_get(),name);
    type_set(e,name);
  }

  // Bind the type body to its name.
  template <> void TypeChecker::visit_dec_body<ast::TypeDec>(ast::TypeDec& e)
  {
    // FIXED: Some code was deleted here.
    const Type* t = tenv_.look(e.name_get());
    const Named* name = dynamic_cast<const Named*>(t);
    e.ty_get().accept(*this);
    name->type_set(e.ty_get().type_get());
    if (!name->sound()) {
      error(e, "rec def", e.name_get());
    }
  }

  /*------------------.
  | Visiting /Chunk/. |
  `------------------*/

  template <class D> void TypeChecker::chunk_visit(ast::Chunk<D>& e)
  {
    // FIXED: Some code was deleted here.
    for (auto& a : e) {
      visit_dec_header(*a);
    }
    for (auto& a : e) {
      visit_dec_body(*a);
    }
  }

  /*-------------.
  | Visit /Ty/.  |
  `-------------*/

  void TypeChecker::operator()(ast::NameTy& e)
  {
    // FIXED: Some code was deleted here (Recognize user defined types, and built-in types).
    const Type* t =tenv_.look(*e.name_get());
    if (!t) {
      error(e,"undefined type",*e.name_get());
    }
    type_set(e, t);
  }

  void TypeChecker::operator()(ast::RecordTy& e)
  {
    // FIXED: Some code was deleted here.
    type_set(e,type(e.fields_get()));
  }

  void TypeChecker::operator()(ast::ArrayTy& e)
  {
    // FIXED: Some code was deleted here.
    e.base_type_get().accept(*this);
    type_set(e,new Array(*e.base_type_get().type_get()));
  }

} // namespace type
