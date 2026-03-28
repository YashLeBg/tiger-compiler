/**
 ** \file ast/type-constructor.hh
 ** \brief Declaration of ast::TypeConstructor.
 */

#pragma once

#include <ast/fwd.hh>
#include <type/fwd.hh>
#include <ast/typable.hh>
namespace ast
{
  /** \class ast::TypeConstructor
   ** \brief Create a new type.
   */

  class TypeConstructor:public Typable
  {
    // FIXME: Some code was deleted here.
    // basicly thats the same thing as typebale but there we juste stock the created type
      public:
          TypeConstructor();
    virtual ~TypeConstructor(){delete created_type_;}
    const type::Type* created_type_get() const;
    void created_type_set(const type::Type* t);
    protected:
      const type::Type* created_type_;

        

  };
} // namespace ast
#include <ast/type-constructor.hxx>
