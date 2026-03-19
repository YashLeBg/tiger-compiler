/**
 ** \file bind/libbind.cc
 ** \brief Define exported bind functions.
 */

#include <bind/binder.hh>
#include <bind/libbind.hh>

namespace bind
{
  // function to
  misc::error bind(ast::ChunkList& e)
  {
    Binder b;
    b(e);
    return b.error_get();
  }

} // namespace bind
