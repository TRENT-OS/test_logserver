#include "log_subject.h"
#include "log_observer_listT.h"
#include <string.h>
#include <stddef.h>



#define OBSERVER_TO_NODE                (Log_observer_t *)observer->data



#define ASSERT_SELF__(self)             \
    if(self == NULL)                    \
        nullptr = true;



#define ASSERT_VTABLE_PARENT__(self)    \
    if(self->vtable == NULL)            \
        nullptr = true;



#define ASSERT_VTABLE_CHILD__(self)     \
    if(self->parent.vtable == NULL)     \
        nullptr = true;



#define ASSERT_SELF_PARENT(self)        \
    ASSERT_SELF__(self)                 \
                                        \
    ASSERT_VTABLE_PARENT__(self)



#define ASSERT_SELF(self)               \
    ASSERT_SELF__(self)                 \
                                        \
    if(nullptr == false)                \
    {                                   \
        ASSERT_VTABLE_CHILD__(self)     \
    }



// foreward declaration
static bool _Log_subject_attach(Subject_t *self, Observer_t *observer);
static bool _Log_subject_detach(Subject_t *self, Observer_t *observer);
static void _Log_subject_notify(Subject_t *self);



static const Subject_Vtable Log_subject_vtable =
{
    .dtor   = Subject_dtor,
    .attach = _Log_subject_attach,
    .detach = _Log_subject_detach,
    .notify = _Log_subject_notify
};



static bool
_Log_subject_attach(Subject_t *self, Observer_t *observer)
{
    bool nullptr = false;
    bool isInsert = false;
    Log_observer_t *last = NULL;
    Subject_node_t *node = NULL;

    ASSERT_SELF_PARENT(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    if(observer == NULL){
        // Debug_printf
        return false;
    }

    node = (Subject_node_t *)(((Log_subject_t *)self->data)->data);

    last = Node_get_last((Log_observer_t *)node->first);

    // first entry
    if(last == NULL)
        last = (OBSERVER_TO_NODE);

    isInsert = Node_insert(last, OBSERVER_TO_NODE);

    node->first = (void *)Node_get_first(last);

    return isInsert;
}



static bool
_Log_subject_detach(Subject_t *self, Observer_t *observer)
{
    bool nullptr = false;
    bool isDelete = false;
    Subject_node_t *node = NULL;

    ASSERT_SELF_PARENT(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    if(observer == NULL){
        // Debug_printf
        return false;
    }

    node = (Subject_node_t *)(((Log_subject_t *)self->data)->data);

    if(!Node_has_prev(OBSERVER_TO_NODE)){
        node->first = (void *)Node_get_next(OBSERVER_TO_NODE);
    }

    isDelete = Node_delete(OBSERVER_TO_NODE);

    return isDelete;
}



static void
_Log_subject_notify(Subject_t *self)
{
    bool nullptr = false;
    Log_observer_t *first = NULL;
    Log_observer_t *next = NULL;
    Subject_node_t *node = NULL;

    ASSERT_SELF_PARENT(self);

    if(nullptr){
        // Debug_printf
        return;
    }

    // traverse list
    node = (Subject_node_t *)(((Log_subject_t *)self->data)->data);

    first = (Log_observer_t *)node->first;

    ASSERT_SELF(first);

    if(nullptr){
        // Debug_printf
        return;
    }

    if(first != NULL){
        first->parent.vtable->update(&first->parent);

        while (Node_has_next(first)) {
            next = Node_get_next(first);

            next->parent.vtable->update(&next->parent);

            first = next;
        }
    }
}



bool
Log_subject_ctor(Log_subject_t *self)
{
    bool nullptr = false;
    bool retval = false;
    Subject_node_t *node = NULL;

    ASSERT_SELF__(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    memset(self, 0, sizeof (Log_subject_t));

    retval = Subject_ctor(&self->parent, self);

    self->parent.vtable = &Log_subject_vtable;

    self->data = &self->subject_list;
    node = (Subject_node_t *)self->data;
    node->first = NULL;

    return retval;
}



void
Log_subject_dtor(Log_subject_t *self)
{
    memset(self, 0, sizeof (Subject_t));
}



bool
Log_subject_attach(Log_subject_t *self, Log_observer_t *observer)
{
    bool nullptr = false;

    ASSERT_SELF(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    if(observer == NULL){
        // Debug_printf
        return false;
    }

    return self->parent.vtable->attach(&self->parent, &observer->parent);
}



bool
Log_subject_detach(Log_subject_t *self, Log_observer_t *observer)
{
    bool nullptr = false;

    ASSERT_SELF(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    if(observer == NULL){
        // Debug_printf
        return false;
    }

    return self->parent.vtable->detach(&self->parent, &observer->parent);
}



bool
Log_subject_action(Log_subject_t *self)
{
    bool nullptr = false;

    ASSERT_SELF(self);

    if(nullptr){
        // Debug_printf
        return false;
    }

    self->parent.vtable->notify(&self->parent);

    return true;
}
