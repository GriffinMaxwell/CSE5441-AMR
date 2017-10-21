/***
 * File: Map_Double.h
 * Desc: I_Map implementation where a key is an ID that maps to a Double_t
   Note: the IDs must be 0 <= ID < length
 */

#ifndef MAP_DOUBLE_H
#define MAP_DOUBLE_H

#include <stdint.h>
#include "I_Map.h"

typedef struct
{
   I_Map_t interface;

   // Private members
   uint32_t length;
   double *doubles;
} Map_Double_t;

/*
 * Initialize a Map_Double
 * @param length: the maximum number of key-value pairs that fit in this map
 */
void Map_Double_Init(Map_Double_t *instance, uint32_t length);

/*
 * Deinitialize a Map_Double
 */
void Map_Double_Deinit(Map_Double_t *instance);

#endif
