/*
   *  SEOS utility functions.
   *
   *  Copyright (C) 2019, Hensoldt Cyber GmbH
*/


/**
 * @defgroup    util Utility functions
 *
 * @brief       Utility functions for hex dump.
*/


#pragma once


#include <stdio.h>


/**
 * @details %printHexDump provides print
 *
 * @param   szDescriptor:   descriptor string or NULL
 * @param   blob:           data buffer
 * @param   blobSize:       size (in bytes)
 *
 * @return  count printed bytes
 *
 * @retval  > 0 if all right
 *          0   if fail to print
 *
 * @ingroup util
*/
size_t printHexDump(const char *szDescriptor, const volatile void *blob, size_t blobSize);
