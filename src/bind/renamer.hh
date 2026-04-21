/**
 ** \file bind/renamer.hh
 ** \brief Implementation of bind::Renamer.
 */

#pragma once
#include <map>
#include <misc/symbol.hh>
#include <ast/default-visitor.hh>
#include <ast/non-object-visitor.hh>

namespace bind
{
  /// Perform identifier renaming within an AST (in place),
  /// without support for objects.
  class Renamer
    : public ast::DefaultVisitor
    , public ast::NonObjectVisitor
  {
  public:
    using super_type = ast::DefaultVisitor;

    // Import overloaded virtual functions.
    using super_type::operator();

    // FIXME: Some code was deleted here.
    Renamer();
    virtual ~Renamer() = default;
    // Visit methods.
    /// \brief Process a declaration body or a usage site.
    ///
    /// \a def is the definition site of \e (must be equal to
    /// \a e if it is a Dec node).
    template <class E, class Def> void visit(E& e, const Def* def);

    /// \name Visiting definition sites.
    /// \{
    // FIXME: Some code was deleted here.
    //overriding each operator() for each type
    void operator()(ast::VarDec& dec) override;
    void operator()(ast::FunctionDec& dec) override;
    void operator()(ast::TypeDec& dec) override;
    /// \}

    /// \name Visiting usage sites.
    /// \{
    // FIXME: Some code was deleted here.
    //same explication as above
    void operator()(ast::SimpleVar& dec) override;
    void operator()(ast::CallExp& dec) override;
    void operator()(ast::NameTy& dec) override;
    /// \}

    void operator()(ast::FunctionChunk& e) override;
    void operator()(ast::TypeChunk& e) override;

  private:
    // FIXME: Some code was deleted here.
    //associate a pointer to a new name unique
    std::map<const ast::Dec*, misc::symbol> new_names_;
  };

} // namespace bind

#include <bind/renamer.hxx>
