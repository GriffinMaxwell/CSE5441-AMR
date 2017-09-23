/***
 * File: I_List.h
 * Desc: Interface functions for a list data structure
 */

#ifndef I_LIST_H
#define I_LIST_H

typedef struct
{
   /*
    * Add an item to the back of the list
    * @param item: The item to add to the list
    */
   void *(Add)(void *item);

   /*
    * Gets the item in the list at the given index
    * @param index: The index of the item in the list
    */
    void *(Get)(int index);
} I_List_t;

#endif
