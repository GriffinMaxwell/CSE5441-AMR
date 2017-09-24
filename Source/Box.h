/***
 * File: Box.h
 * Desc: Data type to store information about a box's location and its neighbors
 */

#ifndef BOX_H
#define BOX_H

#include "List_Fixed.h"

typedef struct
{
   double temperature;

   struct
   {
      int x;
      int y;
      int height;
      int width;
   } position;

   struct
   {
      List_Fixed_t top;
      List_Fixed_t bottom;
      List_Fixed_t left;
      List_Fixed_t right;
   } neighborIds;
} Box_t;

#endif
