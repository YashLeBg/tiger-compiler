/** \file misc/scoped-map.hxx
 ** \brief Implementation of misc::scoped_map.
 */

#pragma once

#include <sstream>
#include <stdexcept>
#include <type_traits>

#include <ranges>
#include <misc/algorithm.hh>
#include <misc/contract.hh>
#include <misc/indent.hh>

#include "scoped-map.hh"

namespace misc
{
  // FIXED: Some code was deleted here.
  template <typename Key, typename Data>
  scoped_map<Key, Data>::scoped_map()
  {
      //create the first scope
      scope_.push_back(std::map<Key, Data>());
  }

  template<typename Key, typename Data>
  void scoped_map<Key, Data>::scope_begin() {
      // put a new map for a new scope at the top of stack
      scope_.push_back(std::map<Key, Data>());
  }

  template<typename Key, typename Data>
  void scoped_map<Key, Data>::scope_end() {
      if (!scope_.empty()) {
          // delete the most recent map and forget all vars in it
          scope_.pop_back();
      }
  }

  template<typename Key, typename Data>
  void scoped_map<Key, Data>::put(const Key &key, const Data &value) {
      // add key and value to the current scope
      scope_.back()[key] = value;
  }

  template<typename Key, typename Data>
  Data scoped_map<Key, Data>::get(const Key &key) const {
      auto e = scope_.end();
      // go throught all scope starting by the most recent scope
      while (e != scope_.begin()) {
          e--;
          // looking for the key in this scope
          auto find = e->find(key);
          // if we found it return his data
          if (find != e->end()) {
              return find->second;
          }
      }
      // we didn't find the key
      // check if Data is a pointer
      if constexpr (CheckPointer<Data>)
      {
          return nullptr;
      }
      else {
          throw std::range_error("not found");
      }

  }

  template<typename Key, typename Data>
  std::ostream &scoped_map<Key, Data>::dump(std::ostream &ostr) const {
      // counter of scope
      int scope_cpt = 0;
      // go throught all scope beginning by the global one to the current one
      for (auto i = scope_.begin(); i != scope_.end();i++) {
          ostr << "------- Scope " << scope_cpt << " -------\n";
          // foreach scope print pair key-value
          for (auto j = i->begin(); j != i->end();j++) {
              ostr << j->first << ": " << j->second << "\n";
          }
          scope_cpt++;
      }
      return ostr;
  }

  template <typename Key, typename Data>
  inline std::ostream& operator<<(std::ostream& ostr,
                                  const scoped_map<Key, Data>& tbl)
  {
    return tbl.dump(ostr);
  }

} // namespace misc
