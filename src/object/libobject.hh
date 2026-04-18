/**
 ** \file object/libobject.hh
 ** \brief Declare functions and variables exported by object module.
 */

#pragma once

#include <ast/fwd.hh>
#include <misc/error.hh>
#include <object/fwd.hh>

namespace object
{
  misc::error bind(ast::ChunkList& e);
  class_names_type* rename(ast::Ast& e);
} // namespace object

#include <object/libobject.hxx>
