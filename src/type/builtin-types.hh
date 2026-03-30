/**
 ** \file type/builtin-types.hh
 ** \brief The classes Int, String, Void.
 */
#pragma once

#include <misc/singleton.hh>
#include <type/fwd.hh>
#include <type/type.hh>

namespace type
{
    // FIXME: Some code was deleted here (Builtin types : Int, String, Void, Nil).
    //ok thats the same shit 4 times. each : hinerytes from typebase class and from singleton, singleton is freind so we can call constuctor.redifine accept function for visitor.
    class Int:public Type,public misc::Singleton<Int>
    {
        friend class misc::Singleton<Int>;
        protected:
        Int()=default;
        public :
        void accept(ConstVisitor& v) const override;
    void accept(Visitor& v) override;
    };
class String:public Type,public misc::Singleton<String>
    {
        friend class misc::Singleton<String>;
        protected:
        String()=default;
        public :
        void accept(ConstVisitor& v) const override;
    void accept(Visitor& v) override;
    };

class Void:public Type,public misc::Singleton<Void>
    {
        friend class misc::Singleton<Void>;
        protected:
        Void()=default;
        public :
        void accept(ConstVisitor& v) const override;
    void accept(Visitor& v) override;
    };
class Nil:public Type,public misc::Singleton<Nil>
    {
        friend class misc::Singleton<Nil>;
        protected:
        Nil()=default;
        public :
        void accept(ConstVisitor& v) const override;
        void accept(Visitor& v) override;
        bool compatible_with(const Type& other) const override;
    };


} // namespace type
