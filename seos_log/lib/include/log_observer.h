#pragma once


#include "abstract_observer.h"
#include "listT.h"
#include <stdbool.h>


typedef struct
{
    NodeT_t    node;
    ListT_t    listT;
    Observer_t parent;
} Log_observer_t;


bool
Log_observer_ctor(Log_observer_t *self);


void
Log_observer_dtor(Log_observer_t *self);


bool
Log_observer_update(Log_observer_t *self);
