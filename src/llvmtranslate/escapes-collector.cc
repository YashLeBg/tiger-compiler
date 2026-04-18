#include <ast/all.hh>
#include <ast/default-visitor.hh>
#include <ast/non-object-visitor.hh>
#include <llvmtranslate/escapes-collector.hh>
#include <type/function.hh>

namespace llvmtranslate
{
  class EscapesCollector
    : public ast::DefaultConstVisitor
    , public ast::NonObjectConstVisitor
  {
  public:
    using super_type = ast::DefaultConstVisitor;
    using super_type::operator();

    EscapesCollector()
      : did_modify_{false}
      , escaped_{}
    {}

    escaped_map_type& escaped_get() { return escaped_; }

    void operator()(const ast::FunctionChunk& e) override
    {
      bool saved_did_modify = did_modify_;

      did_modify_ = !e.empty();
      while (did_modify_)
        {
          did_modify_ = false;
          super_type::operator()(e);
        }

      did_modify_ = saved_did_modify;
    }

    void operator()(const ast::FunctionDec& e) override
    {
      auto* func = curr_function_;
      curr_function_ = &e;
      if (e.body_get())
        e.body_get()->accept(*this);
      curr_function_ = func;
    }

    void operator()(const ast::CallExp& e) override
    {
      super_type::operator()(e);

      const ast::FunctionDec* f = e.def_get();
      if (f && curr_function_)
        {
          auto callee_key = static_cast<const type::Function*>(f->type_get());
          auto caller_key =
            static_cast<const type::Function*>(curr_function_->type_get());
          for (const ast::VarDec* v : escaped_[callee_key])
            {
              if (v->def_get() != curr_function_)
                {
                  auto& vars = escaped_[caller_key];
                  if (vars.find(v) == vars.end())
                    {
                      vars.insert(v);
                      did_modify_ = true;
                    }
                }
            }
        }
    }

    void operator()(const ast::SimpleVar& e) override
    {
      const ast::VarDec* v = e.def_get();
      if (v && curr_function_ && v->def_get() != curr_function_)
        {
          auto key = static_cast<const type::Function*>(curr_function_->type_get());
          auto& vars = escaped_[key];
          if (vars.find(v) == vars.end())
            {
              vars.insert(v);
              did_modify_ = true;
            }
        }
    }

  private:
    bool did_modify_ = false;
    escaped_map_type escaped_;
    const ast::FunctionDec* curr_function_ = nullptr;
  };

  escaped_map_type collect_escapes(const ast::Ast& ast)
  {
    EscapesCollector collect;
    collect(ast);
    return std::move(collect.escaped_get());
  }

} // namespace llvmtranslate
