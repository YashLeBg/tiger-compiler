/**
 ** \file llvmgc/translator.hh
 ** \brief Declaration of llvmgc::GCTranslator
 */

#pragma once

#include <llvm/IR/Value.h>
#include <llvmtranslate/translator.hh>

namespace llvmgc
{

  /** \brief
   ** Translates various AST nodes in a different form to
   ** accomodate the garbage collection process.
   **/
  class GCTranslator : public llvmtranslate::Translator
  {
  public:
    /// Super class.
    using super_type = llvmtranslate::Translator;
    /// Import overloaded operator() methods.
    using super_type::operator();

    GCTranslator(llvm::Module& module,
                 llvmtranslate::escaped_map_type&& escaped);

    /// \name Expressions
    /// \{

    /** \brief
     ** Replaces the calls to tc_malloc done by llvmtranslate::Translator
     ** with calls to a specialized function `tc_init_record`, similar to `tc_init_array`.
     **/
    void operator()(const ast::RecordExp& e) override;

    /** \brief
    ** Translates the strings to a constant struct that hold: gc metadata,
    ** and the characters of the string including the null terminator.
    ** Therefore, the string "Tigrou" is translated to:
    ** { struct metadata, ['T', 'i', 'g', 'r', 'o', 'u', '\0'] }
    **/
    void operator()(const ast::StringExp& e) override;
    /// \}
  };

} // namespace llvmgc
