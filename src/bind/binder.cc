/**
 ** \file bind/binder.cc
 ** \brief Implementation for bind/binder.hh.
 */

#include <ast/all.hh>
#include <bind/binder.hh>

#include <misc/contract.hh>

namespace bind
{
  /*-----------------.
  | Error handling.  |
  `-----------------*/

  /// The error handler.
  const misc::error& Binder::error_get() const { return error_; }

  // FIXME: Some code was deleted here.
  void Binder::operator()(ast::SimpleVar& e)
  {
    auto* def = vars_.get(e.name_get());
    if (def == nullptr)
      error_ << misc::error::error_type::bind;
    else
      e.def_set(def);
  }

  void Binder::operator()(ast::LetExp& e)
  {
    scope_begin();
    e.chunks_get().accept(*this);
    e.body_get().accept(*this);
    scope_end();
  }

  void Binder::operator()(ast::CallExp& e)
  {
    auto* def = funcs_.get(e.name_get());
    if (def == nullptr)
      error_ << misc::error::error_type::bind;
    else
      e.def_set(def);
    for (auto* arg : e.args_get())
      arg->accept(*this);
  }

  void Binder::operator()(ast::WhileExp& e)
  {
    e.test_get().accept(*this);
    auto* old_loop = loop_;
    loop_ = &e;
    e.body_get().accept(*this);
    loop_ = old_loop;
  }

  void Binder::operator()(ast::ForExp& e)
  {
    scope_begin();
    e.vardec_get().accept(*this);
    e.hi_get().accept(*this);
    auto* old_loop = loop_;
    loop_ = &e;
    e.body_get().accept(*this);
    loop_ = old_loop;

    scope_end();
  }

  void Binder::operator()(ast::BreakExp& e)
  {
    if (loop_ == nullptr)
      error_ << misc::error::error_type::bind;
    else
      e.def_set(loop_);
  }

  void Binder::operator()(ast::NameTy& e)
  {
    auto* def = types_.get(e.name_get());
    if (def == nullptr)
      error_ << misc::error::error_type::bind;
    else
      e.def_set(def);
  }

  void Binder::operator()(ast::VarChunk& e)
  {
    for (auto* dec : e)
      operator()(*dec);
  }

  void Binder::operator()(ast::VarDec& e)
  {
    if (e.init_get())
      e.init_get()->accept(*this);
    if (e.type_name_get())
      e.type_name_get()->accept(*this);
    vars_.put(e.name_get(), &e);
  }

  void Binder::operator()(ast::FunctionChunk& e)
  {
    chunk_visit<ast::FunctionDec>(e, funcs_);
  }

  void Binder::operator()(ast::FunctionDec& e)
  {
    scope_begin();
    for (auto* formal : e.formals_get())
      operator()(*formal);
    if (e.result_get())
      e.result_get()->accept(*this);
    if (e.body_get())
      e.body_get()->accept(*this);
    scope_end();
  }

  void Binder::operator()(ast::TypeChunk& e)
  {
    chunk_visit<ast::TypeDec>(e, types_);
  }

  void Binder::operator()(ast::TypeDec& e) { e.ty_get().accept(*this); }

} // namespace bind
