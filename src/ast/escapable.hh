/**
 ** \file ast/escapable.hh
 ** \brief Declaration of ast::Escapable.
 */

#pragma once

#include <ast/fwd.hh>

namespace ast
{
  /// Escapable.
  class Escapable
  {
    // FIXED: Some code was deleted here.
    public:
    Escapable();
    virtual ~Escapable() = default;
    bool escape_get() const;
    void escape_set(bool val);
    const FunctionDec* def_get() const;
    FunctionDec* def_get();
    void def_set(FunctionDec* val);
    protected:
    FunctionDec* def_;
    bool escape_;

  };
} // namespace ast
#include <ast/escapable.hxx>
