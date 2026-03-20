/**
 ** \file bind/libbind.hh
 ** \brief Interface of the bind module.
 */

#pragma once

#include <ast/fwd.hh>
#include <misc/error.hh>

namespace bind
{
  // Bind the chunk.
  misc::error bind(ast::ChunkList& e);

} // namespace bind
