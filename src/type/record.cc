/**
 ** \file type/record.cc
 ** \brief Implementation for type/record.hh.
 */

#include <ostream>

#include <type/builtin-types.hh>
#include <type/record.hh>
#include <type/visitor.hh>

namespace type
{
  void Record::accept(ConstVisitor& v) const { v(*this); }

  void Record::accept(Visitor& v) { v(*this); }

  const Type* Record::field_type(misc::symbol key) const
  {
    for (const Field& f : fields_)
      if (f.name_get() == key)
        return &f.type_get();
    return nullptr;
  }

  int Record::field_index(misc::symbol key) const
  {
    int i = 0;
    for (const Field& f : fields_)
      {
        if (f.name_get() == key)
          return i;
        i++;
      }
    return -1;
  }


  // FIXED: Some code was deleted here (Special implementation of "compatible_with" for Record).
  bool Record::compatible_with(const Type& other) const
  {
    return *this == other || dynamic_cast<const Nil*>(&other.actual());
  }

} // namespace type
