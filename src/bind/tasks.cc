/**
 ** \file bind/tasks.cc
 ** \brief Bind module tasks implementation.
 */

#include <ast/tasks.hh>
#include <bind/libbind.hh>
#include <common.hh>
#define DEFINE_TASKS 1
#include <bind/tasks.hh>
#undef DEFINE_TASKS

namespace bind::tasks
{
  void bindings_compute()
  {
    task_error() << bind::bind(*ast::tasks::the_program);
    task_error().exit_on_error();
  }

  void bound() {}

} // namespace bind::tasks
