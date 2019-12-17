#include "listT.h"
#include "log_symbol.h"
#include <string.h>
#include <stddef.h>



static bool  _ListT_has_prev(NodeT_t *current);
static bool  _ListT_has_next(NodeT_t *current);
static void *_ListT_get_prev(NodeT_t *current);
static void *_ListT_get_next(NodeT_t *current);
static bool  _ListT_insert(NodeT_t *current, NodeT_t *new_node);
static bool  _ListT_delete(NodeT_t *current);
static void *_ListT_get_first(NodeT_t *current);
static void *_ListT_get_last(NodeT_t *current);
static bool  _ListT_isInside(NodeT_t *current);



static const ListT_Vtable ListT_vtable =
{
    .dtor      = ListT_dtor,
    .has_prev  = _ListT_has_prev,
    .has_next  = _ListT_has_next,
    .get_prev  = _ListT_get_prev,
    .get_next  = _ListT_get_next,
    .insert    = _ListT_insert,
    .delete    = _ListT_delete,
    .get_first = _ListT_get_first,
    .get_last  = _ListT_get_last,
    .isInside  = _ListT_isInside
};



bool
ListT_ctor(ListT_t *self)
{
    bool nullptr = false;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    self->vtable = &ListT_vtable;

    return true;
}



void
ListT_dtor(ListT_t *self)
{
    memset(self, 0, sizeof (ListT_t));
}



static bool
_ListT_has_prev(NodeT_t *current)
{
    bool retval = false;

    if(current == NULL){
        // Debug_printf
        return retval;
    }

    if(current->prev != NULL)
        retval = true;

    return retval;
}



static bool
_ListT_has_next(NodeT_t *current)
{
    bool retval = false;

    if(current == NULL){
        // Debug_printf
        return retval;
    }

    if(current->next != NULL)
        retval = true;

    return retval;
}




static void *
_ListT_get_prev(NodeT_t *current)
{
    NodeT_t *prev = NULL;

    if(current == NULL){
        // Debug_printf
        return prev;
    }

    if(_ListT_has_prev(current)){
        prev = current->prev;
        return prev;
    }

    return prev;
}




static void *
_ListT_get_next(NodeT_t *current)
{
    NodeT_t *next = NULL;

    if(current == NULL){
        // Debug_printf
        return next;
    }

    if(_ListT_has_next(current)){
        next = current->next;
        return next;
    }

    return next;
}




static bool
_ListT_insert(NodeT_t *current, NodeT_t *new_node)
{
    bool retval = false;

    NodeT_t *next = NULL;

    if(current == NULL){
        // Debug_printf
        return retval;
    }

    if(new_node == NULL){
        // Debug_printf
        return retval;
    }

    if(current == new_node){
        return true;
    }

    if(_ListT_has_next(current)){
        next = _ListT_get_next(current);
        next->prev = new_node;
    }

    current->next = new_node;

    new_node->prev = current;
    new_node->next = next;

    retval = true;

    return retval;
}




static bool
_ListT_delete(NodeT_t *current)
{
    NodeT_t *prev = NULL;
    NodeT_t *next = NULL;

    if(current == NULL){
        // Debug_printf
        return false;
    }

    if(_ListT_has_prev(current)){
        prev = _ListT_get_prev(current);
    }

    if(_ListT_has_next(current)){
        next = _ListT_get_next(current);
    }

    if (next == NULL && prev == NULL) {
        return true;
    }
    else if(next == NULL){
        prev->next = next;
    }
    else if (prev == NULL) {
        next->prev = prev;
    }
    else {
        prev->next = next;
        next->prev = prev;
    }

    current->prev = NULL;
    current->next = NULL;

    return true;
}



static void *
_ListT_get_first(NodeT_t *current)
{
    NodeT_t *first = NULL;

    if(current == NULL){
        // Debug_printf
        return first;
    }

    while (_ListT_has_prev(current)) {
        first = _ListT_get_prev(current);
        current = first;
    }

    if(first == NULL)
        first = current;

    return first;
}



static void *
_ListT_get_last(NodeT_t *current)
{
    NodeT_t *last = NULL;

    if(current == NULL){
        // Debug_printf
        return last;
    }

    while (_ListT_has_next(current)) {
        last = _ListT_get_next(current);
        current = last;
    }

    if(last == NULL)
        last = current;

    return last;
}



static bool
_ListT_isInside(NodeT_t *current)
{
    NodeT_t *first = NULL;
    bool retval = false;

    if(current == NULL){
        // Debug_printf
        return retval;
    }

    while (_ListT_has_next(first)) {
        if(first == current){
            retval = true;
            break;
        }

        first = _ListT_get_next(first);
    }

    if(first == current)
        retval = true;

    return retval;
}
