#pragma once


#include "abstract_subject.h"
//#include "log_databuffer.h"
#include <stdbool.h>


typedef struct{
    void *first;
} Subject_node_t;


typedef struct
{
    Subject_node_t       node;
    const Subject_Vtable *vtable;
} Log_subject_t;


bool
Log_subject_ctor(Log_subject_t *self);


void
Log_subject_dtor(Subject_t *self);


bool
Log_subject_attach(Subject_t *self, Observer_t *observer);


bool
Log_subject_detach(Subject_t *self, Observer_t *observer);


void
Log_subject_notify(Subject_t *self, void *data);
