#pragma once


#include "log_observer.h"
#include "abstract_subject.h"
#include <stdbool.h>


typedef struct
{
    void *first;
} Subject_node_t;


typedef struct
{
    void           *data;
    Subject_node_t subject_list;
    Subject_t      parent;
} Log_subject_t;


bool
Log_subject_ctor(Log_subject_t *self);


void
Log_subject_dtor(Log_subject_t *self);


bool
Log_subject_attach(Log_subject_t *self, Log_observer_t *observer);


bool
Log_subject_detach(Log_subject_t *self, Log_observer_t *observer);


bool
Log_subject_action(Log_subject_t *self);
