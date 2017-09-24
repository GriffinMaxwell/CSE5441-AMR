/***
 * File: Map_Box.c
 */

#include <stdlib.h>
#include <string.h>
#include "Map_Box.h"
#include "Macro.h"

static void AddEntry(void *context, int key, void *value)
{
   REINTERPRET(instance, context, Map_Box_t *);

   // store keys to know which boxes are valid and can be freed on deinit
   instance->ids[currentLength] = key;
   instance->currentLength++;

   memcpy(&instance->boxes[key], value, sizeof(Box_t));
}

/*
 * Linear search through the keys
 */
static void * FindEntry(void *context, int key)
{
   REINTERPRET(instance, context, Map_Box_t *);

   return (key < instance->length) ? &instance->box[key] : NULL;
}

void Map_Box_Init(Map_Box_t *instance, uint32_t length)
{
   instance->interface.Add = AddEntry;
   instance->interface.Find = FindEntry;

   instance->ids = calloc((size_t)length, sizeof(int));
   instance->boxes = calloc((size_t)length, sizeof(Box_t));

   instance->length = length;
   instance->currentLength = 0;
}

void Map_Box_Deinit(Map_Box_t *instance)
{
   // First deinitialize the lists of neighbor IDs inside the boxes
   uint32_t i;
   for(i = 0; i < instance->currentLength; i++)
   {
      int id = ids[i];
   	List_Fixed_Deinit(&instance->boxes[id].neighborIds.top);
   	List_Fixed_Deinit(&instance->boxes[id].neighborIds.bottom);
   	List_Fixed_Deinit(&instance->boxes[id].neighborIds.left);
   	List_Fixed_Deinit(&instance->boxes[id].neighborIds.right);
   }

   free(instance->ids);
   free(instance->boxes);
}
