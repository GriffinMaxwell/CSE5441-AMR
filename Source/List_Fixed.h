/***
 * File: List_Fixed.h
 * Desc: I_List implementation where the total size is fixed at initialization
 */

#ifndef LIST_FIXED_H
#define LIST_FIXED_H

#include <stdint.h>
#include <stddef.h>
#include "I_List.h"

typedef struct
{
   I_List_t interface;

   // Private members
   uint32_t length;
   uint32_t currentLength;
   size_t size;
   void *data;
} List_Fixed_t;

/*
 * Initialize a List_Fixed
 * @param length: The max number of items that can fit in the list
 * @param size: The size of each element in the list
 */
void List_Fixed_Init(List_Fixed_t *instance, uint32_t length, size_t size);

/*
 * Get the current number of items in a List_Fixed
 * @return uint32_t: The current length
 */
uint32_t List_Fixed_CurrentLength(List_Fixed_t *instance);

/*
 * Reset a List_Fixed to its initialized state.
 * @note: does not reallocate memory
 */
void List_Fixed_Reset(List_Fixed_t *instance);

/*
 * Deinitialize a List_Fixed
 */
void List_Fixed_Deinit(List_Fixed_t *instance);

#endif
