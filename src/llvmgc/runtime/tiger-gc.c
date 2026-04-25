/**
 ** \file llvmgc/runtime/tiger-gc.c
 ** \brief Implementation of the TC gc.
 ** It holds the main algorithm of the collector.
 ** This is a suggestion of skeleton for an implementation of a garbage collector.
 */

#include "tiger-gc.h"
#include <stdlib.h>
#include "fwd.h"

/// Defined by the compiler
extern void tc_main(int);
/// The function that defines the consts array
extern void init_consts(void);
/// This is called for initialization
struct gc_ctx gc_ctx_ = {
  // FIXED: Some code was deleted here.
    /// At the beging there is no object
    .head = NULL,
    /// At the beging we didnot allocated anything
    .allocated_bytes=0,
    .threshold=1024*1024, ///We start using that limit but we will be able to change it
  .gc_enabled = true,
  .tos = NULL,
};

//check if obj is the adress of an object in the heap
// return a pointer of this object if it is and NULL if its not
static struct gc_object* in_heap(void* obj) {
  struct gc_object* gc = gc_ctx_.head;
  while (gc != NULL) {
    if (obj == (void*)&gc->f[0]) {
      return gc;
    }
    gc = gc->md.next;
  }
  return NULL;
}

static void mark_object(struct gc_object* obj) {
  if (obj == NULL || obj->md.marked)
    return;
  obj->md.marked = true;
  size_t num_words = obj->md.size / sizeof(tc_word_t);
  for (size_t i = 0; i < num_words; i++) {
    void* ptr = (void*)obj->f[i];
    if (ptr != NULL) {
      struct gc_object* child = in_heap(ptr);
      if (child != NULL)
        mark_object(child);
    }
  }
}

void stack_scan() {
  void** bottom = (void**)__builtin_frame_address(0);
  void** top = (void**)gc_ctx_.tos;
  while (bottom < top) {
    void* val = *bottom;
    if (val != NULL) {
      struct gc_object* obj = in_heap(val);
      if (obj != NULL)
        mark_object(obj);
    }
    bottom++;
  }
}

static void sweep() {
  struct gc_object* current = gc_ctx_.head;
  while (current != NULL) {
    struct gc_object* next_obj = current->md.next;
    if (!current->md.marked) {
      if (current->md.prev != NULL)
        current->md.prev->md.next = current->md.next;
      else
        gc_ctx_.head = current->md.next;
      if (current->md.next != NULL)
        current->md.next->md.prev = current->md.prev;
      gc_ctx_.allocated_bytes -= current->md.size;
      free(current);
    } else {
      current->md.marked = false;
    }
    current = next_obj;
  }
}

void gc_collect()
{
  if (!gc_ctx_.gc_enabled)
    return;
  stack_scan();
  sweep();
}

void gc_enter_runtime()
{
  gc_ctx_.gc_enabled = false;
}

void gc_exit_runtime()
{
  gc_ctx_.gc_enabled = true;
}

int main(void)
{
  init_consts();

#ifdef GCDEBUG
  // This code will be included if the Tiger program was compiled with the task `--llvm-gc-debug`
#endif

  /// Initialize the top of the stack with main's frame address.
  gc_ctx_.tos = __builtin_frame_address(0);

  tc_main(0);

  return 0;
}
