/**
 ** \file llvmgc/runtime/gc-alloc.c
 ** \brief Implementation of the TC allocation functions.
 **
 ** This file holds the functions used directly and indirectly by the Tiger program and the runtime
 ** to allocate arrays, records and strings.
 ** These functions should attempt to allocate memory according to their specifications and
 ** call the garbage collector when necessary.
 */

#include "gc-alloc.h"
#include "fwd.h"
#include "tiger-gc.h"
#include <stdlib.h>


/// For all those shorts function, they have the same logic : using tc_malloc to allocat memory and initialize it
char* tc_init_string(size_t size)
{
    // FIXED: Some code was deleted here.
    return (char*)tc_malloc(size);
}

size_t* tc_init_array(size_t size, size_t elt)
{
    // FIWED: Some code was deleted here.
    size_t* res=(size_t*)tc_malloc(size*sizeof(size_t));
    for(size_t i=0;i<size;i++)
    {
        res[i]=elt;
    }
    return res;
}

size_t* tc_init_ptr_array(size_t size, struct gc_object* elt)
{
    // FIXED: Some code was deleted here.
    size_t* res=(size_t*)tc_malloc(size*sizeof(size_t));
    for(size_t i=0;i<size;i++)
    {
        res[i]=(size_t)elt; /// let's cast it to size_t
    }
    return res;
}

void* tc_init_record(
        // FIXED: Some code was deleted here.
        size_t size
        )
{
/// We just aske the exact size to be allocated
return tc_malloc(size);   
}

void* tc_malloc(size_t size)
{
    // FIXED: Some code was deleted here.
    if(gc_ctx_.gc_enabled){
        /// If the allocation needed is greater than th threshold we put, then increasing it by 2
        if(size > gc_ctx_.threshold){
            gc_ctx_.threshold=size*2; 
        }
        /// If the next ellocation makes us
        if(gc_ctx_.allocated_bytes+size>=gc_ctx_.threshold)
        {
            gc_collect();
        }
    }
    /// The real allocation with malloc
    size_t total_size=sizeof(struct gc_object)+size;/// allocation size asked+ metadatas
    struct gc_object* new_obj = malloc(total_size);
    /// We initialize metadata
    new_obj->md.size=size;
    new_obj->md.marked=false;
    /// We start the double linked list
    new_obj->md.prev=NULL;
    new_obj->md.next=gc_ctx_.head;
    if (gc_ctx_.head!=NULL){
        gc_ctx_.head->md.prev=new_obj;}
    gc_ctx_.head=new_obj;
    ///update of the total allocated bytes
    gc_ctx_.allocated_bytes+=size;
    return new_obj->f;
}
