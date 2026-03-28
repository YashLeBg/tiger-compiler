/**
 ** \file misc/singleton.hh
 ** \brief Generic singleton
 */

#pragma once

namespace misc
{
  template <typename T> class Singleton
  {
    // FIXME: Some code was deleted here.
      public:
          //same as the piscine exo.
          Singleton(const Singleton&)=delete;
          Singleton& operator=(const Singleton&)=delete;
      static const T& instance(){
      static T instance_;
      return instance_;
      }
      protected:
      Singleton()=default;
      ~Singleton()=default;
  };

} // namespace misc
