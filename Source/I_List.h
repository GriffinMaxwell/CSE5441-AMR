/***
 * File: I_List.h
 * Desc: Interface functions for a list data structure
 */

#ifndef I_LIST_H
#define I_LIST_H

#include <stdint.h>

typedef struct
{
   /*
    * Add an item to the back of the list
    * @param item: The item to add to the list
    */
   void (*Add)(I_List_t *context, void *item);

   /*
    * Gets the item in the list at the given index
    * @param index: The index of the item in the list
    */
    void * (*Get)(I_List_t *context, uint32_t index);
} I_List_t;

/*
 * Interface function call macros
 */
#define List_Add(instance, item) \
   (instance)->Add((instance), (item))

#define List_Get(instance, index) \
   (instance)->Get((instance), (index))

#endif
