#pragma once


#include "log_observer.h"


bool
Node_has_prev(Log_observer_t *self);


bool
Node_has_next(Log_observer_t *self);


Log_observer_t *
Node_get_prev(Log_observer_t *self);


Log_observer_t *
Node_get_next(Log_observer_t *self);


bool
Node_insert(Log_observer_t *self, Log_observer_t *new_node);


bool
Node_delete(Log_observer_t *self);


Log_observer_t *
Node_get_first(Log_observer_t *self);


Log_observer_t *
Node_get_last(Log_observer_t *self);
