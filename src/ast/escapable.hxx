/**
 ** \file ast/escapable.hxx
 ** \brief Inline methods of ast::Escapable.
 */

#pragma once

#include <ast/escapable.hh>

namespace ast
{
  // FIXED: Some code was deleted here.
    inline bool Escapable::escape_get() const {
      return escape_;
    }

    inline void Escapable::escape_set(bool val) {
      escape_ = val;
    }

    inline void Escapable::def_set(FunctionDec *val) {
      def_ = val;
    }

    inline const FunctionDec* Escapable::def_get() const {
      return def_;
    }

    inline FunctionDec* Escapable::def_get() {
      return def_;
    }

} // namespace ast
