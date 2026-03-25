/**
 ** \file bind/renamer.cc
 ** \brief Implementation of bind::Renamer.
 */

#include <bind/renamer.hh>

namespace bind
{
  using namespace ast;

 //Default constructor
  Renamer::Renamer() : super_type()
  {}
  //Rename a variable and visite its expression
  //the parameter is a vardec node
  void Renamer::operator()(ast::VarDec& dec)
  {
    misc::symbol new_name = misc::symbol::fresh(dec.name_get());
    new_names_[&dec] = new_name;
    dec.name_set(new_name);
    super_type::operator()(dec);
  }
  //That function take a functiondec node as param and rename a funciont and visite body/param
  void Renamer::operator()(ast::FunctionDec& dec)
  {
    misc::symbol new_name = misc::symbol::fresh(dec.name_get());
    new_names_[&dec] = new_name;
    dec.name_set(new_name);
    super_type::operator()(dec);
  }
  //That function is the same as above but for typedec nodes
  void Renamer::operator()(ast::TypeDec& dec)
  {
    misc::symbol new_name = misc::symbol::fresh(dec.name_get());
    new_names_[&dec] = new_name;
    dec.name_set(new_name);
    super_type::operator()(dec);
  }
  //That function apply new name to variable
  void Renamer::operator()(ast::SimpleVar& e)
  {
    if(e.def_get()){
        visit(e,e.def_get());
    }
    super_type::operator()(e);
  }
  //That function apply a new name to function
  void Renamer::operator()(ast::CallExp& e)
  {
  if(e.def_get()){
        visit(e,e.def_get());
    }
    super_type::operator()(e);
  }

  //That function apply new name to type
  void Renamer::operator()(ast::NameTy& e)
  {
      if(e.def_get()){
        visit(e,e.def_get());
    }
    super_type::operator()(e);
  }

  // FIXME: Some code was deleted here.

 // namespace bind
