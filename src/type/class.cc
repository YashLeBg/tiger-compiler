/**
 ** \file type/class.cc
 ** \brief Implementation for type/class.hh.
 */

#include <ostream>

#include <misc/algorithm.hh>
#include <type/class.hh>
#include <type/visitor.hh>
#include <type/builtin-types.hh>

namespace type
{
  Class::Class(const Class* super)
    : Type()
    , id_(fresh_id())
    , super_(super)
    , subclasses_()
  {}

  void Class::accept(ConstVisitor& v) const { v(*this); }

  void Class::accept(Visitor& v) { v(*this); }

  const Type* Class::attr_type(misc::symbol key) const
  {
    const Attribute* attribute = attr_find(key);
    if (!attribute)
      return nullptr;
    return &attribute->type_get();
  }

  const Type* Class::meth_type(misc::symbol key) const
  {
    return meth_find(key);
  }

  // FIXED: Some code was deleted here (Find common super class).
  const Class* Class::common_root(const Class& other) const
  {
    std::set<const Class*> ancestors;
    for (const Class* c = this; c != nullptr; c = c->super_get())
      ancestors.insert(c);
    for (const Class* c = &other; c; c = c->super_get())
      if (ancestors.count(c))
        return c;
    return nullptr;
  }

  // FIXED: Some code was deleted here (Super class soundness test).
  bool Class::sound() const
  {
    std::set<const Class*> seen;
    for (const Class* c = this; c; c = c->super_get())
      if (!seen.insert(c).second)
        return false;
    return true;
  }

  // FIXED: Some code was deleted here (Special implementation of "compatible_with" for Class).
  bool Class::compatible_with(const Type& other) const
  {
    if (dynamic_cast<const Nil*>(&other.actual()))
      return true;
    const Class* c = dynamic_cast<const Class*>(&other.actual());
    if (!c)
      return false;
    for (const Class* current = this; current; current = current->super_get())
      if (current == c)
        return true;
    return false;
  }

  const Class& Class::object_instance()
  {
    static Class instance_;
    return instance_;
  }

  unsigned Class::fresh_id()
  {
    static unsigned counter_ = 0;
    return counter_++;
  }

} // namespace type
