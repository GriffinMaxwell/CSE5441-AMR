/***
 * File: Box.h
 * Desc: Data types for a box's sides, temperature, location, and neighbors
 */

#ifndef BOX_H
#define BOX_H

#include "List_Fixed.h"

typedef enum
{
   Box_Side_Top,
   Box_Side_Bottom,
   Box_Side_Left,
   Box_Side_Right
} Box_Side_t;

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
