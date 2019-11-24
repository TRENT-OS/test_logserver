#pragma once


#include <stdint.h>
#include <stdbool.h>


typedef struct ListT_t ListT_t;
typedef struct NodeT_t NodeT_t;


typedef void
(*ListT_dtorT)(ListT_t *self);


typedef bool
(*ListT_hasPrevT)(NodeT_t *current);


typedef bool
(*ListT_hasNextT)(NodeT_t *current);


typedef void *
(*ListT_getPrevT)(NodeT_t *current);


typedef void *
(*ListT_getNextT)(NodeT_t *current);


typedef bool
(*ListT_insertT)(NodeT_t *current, NodeT_t *new_Node);


typedef bool
(*ListT_deleteT)(NodeT_t *current);


typedef void *
(*ListT_getFirstT)(NodeT_t *current);


typedef void *
(*ListT_getLastT)(NodeT_t *current);


typedef bool
(*ListT_isInsideT)(NodeT_t *current);


typedef struct
{
    ListT_dtorT     dtor;
    ListT_hasPrevT  has_prev;
    ListT_hasNextT  has_next;
    ListT_getPrevT  get_prev;
    ListT_getNextT  get_next;
    ListT_insertT   insert;
    ListT_deleteT   delete;
    ListT_getFirstT get_first;
    ListT_getLastT  get_last;
    ListT_isInsideT isInside;
}
ListT_Vtable;


struct ListT_t
{
    const ListT_Vtable *vtable;
};


struct NodeT_t
{
    void *prev;
    void *next;
};


bool
ListT_ctor(ListT_t *self);


void
ListT_dtor(ListT_t *self);
