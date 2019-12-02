#include "listT.h"
#include "log_symbol.h"
#include <string.h>
#include <stddef.h>



static bool  ListT_has_prev(NodeT_t *current);
static bool  ListT_has_next(NodeT_t *current);
static void *ListT_get_prev(NodeT_t *current);
static void *ListT_get_next(NodeT_t *current);
static bool  ListT_insert(NodeT_t *current, NodeT_t *new_node);
static bool  ListT_delete(NodeT_t *current);
static void *ListT_get_first(NodeT_t *current);
static void *ListT_get_last(NodeT_t *current);
static bool  ListT_isInside(NodeT_t *current);



static const ListT_Vtable ListT_vtable =
{
    .dtor      = ListT_dtor,
    .has_prev  = ListT_has_prev,
    .has_next  = ListT_has_next,
    .get_prev  = ListT_get_prev,
    .get_next  = ListT_get_next,
    .insert    = ListT_insert,
    .delete    = ListT_delete,
    .get_first = ListT_get_first,
    .get_last  = ListT_get_last,
    .isInside  = ListT_isInside
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
ListT_has_prev(NodeT_t *current)
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
ListT_has_next(NodeT_t *current)
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
ListT_get_prev(NodeT_t *current)
{
    NodeT_t *prev = NULL;

    if(current == NULL){
        // Debug_printf
        return prev;
    }

    if(ListT_has_prev(current)){
        prev = current->prev;
        return prev;
    }

    return prev;
}




static void *
ListT_get_next(NodeT_t *current)
{
    NodeT_t *next = NULL;

    if(current == NULL){
        // Debug_printf
        return next;
    }

    if(ListT_has_next(current)){
        next = current->next;
        return next;
    }

    return next;
}




static bool
ListT_insert(NodeT_t *current, NodeT_t *new_node)
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

    if(ListT_has_next(current)){
        next = ListT_get_next(current);
        next->prev = new_node;
    }

    current->next = new_node;

    new_node->prev = current;
    new_node->next = next;

    retval = true;

    return retval;
}




static bool
ListT_delete(NodeT_t *current)
{
    bool retval = false;

    NodeT_t *prev = NULL;
    NodeT_t *next = NULL;

    if(current == NULL){
        // Debug_printf
        return retval;
    }

    if(ListT_has_prev(current)){
        prev = ListT_get_prev(current);
    }

    if(ListT_has_next(current)){
        next = ListT_get_next(current);
    }

    if (next == NULL && prev == NULL) {
        retval = true;
        return retval;
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

    retval = true;

    return retval;
}



static void *
ListT_get_first(NodeT_t *current)
{
    NodeT_t *first = NULL;

    if(current == NULL){
        // Debug_printf
        return first;
    }

    while (ListT_has_prev(current)) {
        first = ListT_get_prev(current);
        current = first;
    }

    if(first == NULL)
        first = current;

    return first;
}



static void *
ListT_get_last(NodeT_t *current)
{
    NodeT_t *last = NULL;

    if(current == NULL){
        // Debug_printf
        return last;
    }

    while (ListT_has_next(current)) {
        last = ListT_get_next(current);
        current = last;
    }

    if(last == NULL)
        last = current;

    return last;
}



static bool
ListT_isInside(NodeT_t *current)
{
    NodeT_t *first = NULL;
    bool retval = false;

    if(current == NULL){
        // Debug_printf
        return retval;
    }

    while (ListT_has_next(first)) {
        if(first == current){
            retval = true;
            break;
        }

        first = ListT_get_next(first);
    }

    if(first == current)
        retval = true;

    return retval;
}
