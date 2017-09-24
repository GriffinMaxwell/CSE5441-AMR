/***
 * File: Map_Box.c
 */

#include <stdlib.h>
#include "Map_Box.h"
#include "Macro.h"

static void AddEntry(void *context, int key, void *value)
{
   REINTERPRET(instance, context, Map_Box_t *);

   instance->ids[instance->currentLength] = key
   memcpy(instance->boxes[instance->currentLength], value, sizeof(Box_t));
   currentLength++;
}

/*
 * Linear search through the keys
 */
static void * FindEntry(void *context, int key)
{
   REINTERPRET(instance, context, Map_Box_t *);

   uint32_t index;
   for(index = 0; index < currentLength; index++)
   {
      if(key == instance->ids[index])
      {
         return boxes[index];
      }
   }
}

void Map_Box_Init(Map_Box_t *instance, uint32_t length)
{
   instance->interface.Add = AddEntry;
   instance->interface.Find = FindEntry;

   instance->ids = calloc((size_t)length, sizeof(int));
   instance->boxes = calloc((size_t)length, sizeof(Box_t));

   instance->currentLength = 0;
}

void Map_Box_Deinit(Map_Box_t *instance)
{
   free(instance->ids);
   free(instance->boxes);
}
