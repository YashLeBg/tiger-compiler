/**
 ** \file bind/tasks.hh
 ** \brief Bind module related tasks.
 */

#pragma once

#include <task/libtask.hh>

namespace bind::tasks
{
  TASK_GROUP("3. Binding");

  /// Flag to display addresses or not
  BOOLEAN_TASK_DECLARE("B|bindings-display",
                       "display bindings",
                       bindings_display_p,
                       "");

  /// Run bindings
  TASK_DECLARE("b|bindings-compute",
               "bind the chunklist",
               bindings_compute,
               "parse");

  // Check that bindings have been computed
  TASK_DECLARE("bound", "check bindings", bound,
               "bindings-compute");
  //the one to rename identifiers  
  TASK_DECLARE("rename", "rename identifiers", rename_compute,
               "bindings-compute");

} // namespace bind::tasks
