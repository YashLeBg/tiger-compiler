/**
 ** \file escapes/escapes-visitor.cc
 ** \brief Implementation for escapes/escapes-visitor.hh.
 */
#include <ast/all.hh>



#include <escapes/escapes-visitor.hh>
#include <misc/contract.hh>



namespace escapes
{
  // FIXED: Some code was deleted here.
  void EscapesVisitor::operator()(ast::FunctionDec &e) {
      func_history_.push_back(&e);
      e.formals_get().accept(*this);
      if (e.body_get()) {
          e.body_get()->accept(*this);
      }
      func_history_.pop_back();
  }

  void EscapesVisitor::operator()(ast::VarDec& e) {
      e.escape_set(false);
      if (!func_history_.empty()) {
          e.def_set(func_history_.back());
      }
      deep_var_[&e] = func_history_.size();
      if (e.init_get()) {
          e.init_get()->accept(*this);
      }
  }

  void EscapesVisitor::operator()(ast::SimpleVar& e) {
      ast::VarDec* v = e.def_get();
      if (v) {
          if (func_history_.size() > deep_var_[v]) {
              v->escape_set(true);
          }
      }
  }
} // namespace escapes
