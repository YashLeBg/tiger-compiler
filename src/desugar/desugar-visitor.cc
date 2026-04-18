/**
 ** \file desugar/desugar-visitor.cc
 ** \brief Implementation of desugar::DesugarVisitor.
 */

#include <ast/all.hh>
#include <ast/libast.hh>
#include <desugar/desugar-visitor.hh>
#include <misc/algorithm.hh>
#include <misc/symbol.hh>
#include <parse/libparse.hh>
#include <parse/tweast.hh>
#include <type/builtin-types.hh>

namespace desugar
{
  DesugarVisitor::DesugarVisitor(bool desugar_for_p, bool desugar_string_cmp_p)
    : super_type()
    , desugar_for_p_(desugar_for_p)
    , desugar_string_cmp_p_(desugar_string_cmp_p)
  {}

  /*-----------------------------.
  | Desugar string comparisons.  |
  `-----------------------------*/
  void DesugarVisitor::operator()(const ast::OpExp& e)
  {
    if (desugar_string_cmp_p_ && e.left_get().type_get()
        && dynamic_cast<const type::String*>(
             &e.left_get().type_get()->actual()))
      {
        ast::Exp* lhs = recurse(e.left_get());
        ast::Exp* rhs = recurse(e.right_get());
        parse::Tweast in;
        switch (e.oper_get())
          {
          case ast::OpExp::Oper::eq:
            in << "streq(" << lhs << ", " << rhs << ")";
            break;
          case ast::OpExp::Oper::ne:
            in << "streq(" << lhs << ", " << rhs << ") = 0";
            break;
          default:
            in << "strcmp(" << lhs << ", " << rhs << ") "
               << str(e.oper_get()) << " 0";
            break;
          }
        result_ = std::get<ast::Exp*>(parse::parse(in));
        return;
      }
    super_type::operator()(e);
  }

  /*----------------------.
  | Desugar `for' loops.  |
  `----------------------*/

  /*<<-
    Desugar `for' loops as `while' loops:

       for i := lo to hi do
         body

     is transformed as:

       let
         var _lo := lo
         var _hi := hi
         var i := _lo
       in
         if i <= _hi then
           while 1 do
             (
               body;
               if i = _hi then
                 break;
               i := i + 1
             )
       end

     Notice that:

     - a `_hi' variable is introduced so that `hi' is evaluated only
       once;

     - a `_lo' variable is introduced to prevent `i' from being in the
       scope of `_hi';

     - a first test is performed before entering the loop, so that the
       loop condition becomes `i < _hi' (instead of `i <= _hi'); this
       is done to prevent overflows on INT_MAX.
       ->>*/

  void DesugarVisitor::operator()(const ast::ForExp& e)
  {
    if (!desugar_for_p_)
      {
        super_type::operator()(e);
        return;
      }
    ast::Exp* lo = recurse(*e.vardec_get().init_get());
    ast::Exp* hi = recurse(e.hi_get());
    ast::Exp* body = recurse(e.body_get());
    misc::symbol iname = e.vardec_get().name_get();
    parse::Tweast in;
    in << "let var _lo := " << lo
       << " var _hi := " << hi
       << " var " << iname << " := _lo"
       << " in if " << iname << " <= _hi then"
       << " while 1 do (" << body << "; if " << iname << " = _hi then break;"
       << " " << iname << " := " << iname << " + 1) end";
    result_ = std::get<ast::Exp*>(parse::parse(in));
  }

} // namespace desugar
