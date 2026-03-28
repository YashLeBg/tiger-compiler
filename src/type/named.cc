/**
 ** \file type/named.cc
 ** \brief Implementation for type/named.hh.
 */

#include <set>

#include <type/named.hh>
#include <type/visitor.hh>

namespace type
{
  Named::Named(misc::symbol name)
    : name_(name)
    , type_(nullptr)
  {}

  Named::Named(misc::symbol name, const Type* type)
    : name_(name)
    , type_(type)
  {}

  // Inherited functions
  void Named::accept(ConstVisitor& v) const
  {
    v(*this);
  }

  void Named::accept(Visitor& v)
  {
    v(*this);
  }

  bool Named::sound() const
  {
    std::set<const Named*> seen;
    const Named* current = this;
    while (current)
      {
        if (!seen.insert(current).second)
          return false;
        if (!current->type_)
          return true;
        current = dynamic_cast<const Named*>(current->type_);
      }
    return true;
  }

  bool Named::compatible_with(const Type& other) const
  {
    // FIXED: Some code was deleted here (Special implementation of "compatible_with" for Named).
    return actual().compatible_with(other);
  }

} // namespace type
