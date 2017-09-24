/***
 * File: Map_Box.h
 * Desc: I_Map implementation where a key is an ID that maps to a Box_t
 */

#ifndef MAP_BOX_H
#define MAP_BOX_H

#include <stdint.h>
#include "I_Map.h"
#include "Box.h"

typdef struct
{
   I_Map_t interface;

   int *ids;
   Box_t *boxes;
   uint32_t currentLength;
} Map_Box_t;

/*
 * Initialize a Map_Box
 * @param length: the maximum number of key-value pairs that fit in this map
 */
void Map_Box_Init(Map_Box_t *instance, uint32_t length);

/*
 * Deinitialize a Map_Box
 */
void Map_Box_Init(Map_Box_t *instance);

#endif
