/***
 * File: List_Fixed.h
 * Desc: I_List implementation where the total size is fixed at init
 */

#ifndef LIST_FIXED_H
#define LIST_FIXED_H

#include <stddef.h>
#include "I_List.h"

typedef struct
{
   I_List_t interface;

   uint32_t length;
   size_t size;
   void *data;
   uint32_t currentLength;
} List_Fixed_t;

/*
 * Initialize a List_Fixed
 * @param length: The max number of items that can fit in the list
 * @param size: The size of each element in the list
 */
void List_Fixed_Init(List_Fixed_t *instance, uint32_t length, size_t size);

/*
 * Get the current number of items in a List_Fixed
 * @return size_t: The current length
 */
uint32_t List_Fixed_CurrentLength(List_Fixed_t *instance);

/*
 * Reset a List_Fixed to its initialized state.
 */
void List_Fixed_Reset(List_Fixed_t *instance);

/*
 * Deinitialize a List_Fixed
 */
void List_Fixed_Deinit(List_Fixed_t *instance);

#endif
