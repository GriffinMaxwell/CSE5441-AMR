/***
 * File: I_Map.h
 * Desc: Interface functions for a key-value map data structure, where the
 *       key is an integer
 */

#ifndef I_MAP_H
#define I_MAP_H

typedef struct
{
   /*
    * Add a key-value pair to the map
    * @pre: the key doesn't already exist in the map
    * @param key: an integer key
    * @param value: the value associated with the key
    */
   void (*Add)(void *context, int key, void *value);

   /*
    * Finds the value associated with the given key
    * @param key: the key
    * @return void *: pointer to the value, or NULL if the key is not found
    */
    void * (*Find)(void *context, int key);
} I_Map_t;

/*
 * Interface function call macros
 */
#define Map_Add(instance, key, value) \
   (instance)->Add((instance), (key), (value))

#define Map_Find(instance, key) \
   (instance)->Find((instance), (key))

#endif
