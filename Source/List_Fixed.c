/***
 * File: List_Fixed.c
 */

#include <stdlib.h>
#include <string.h>
#include "List_Fixed.h"
#include "Macro.h"

static void * CalculateItemAddress(List_Fixed_t *instance, uint32_t index)
{
   REINTERPRET(bytes, instance->data, uint8_t *);

   return &bytes[instance->size * index];
}

static void AddItem(I_List_t *context, void *item)
{
   REINTERPRET(instance, context, List_Fixed_t *);

   memcpy(CalculateItemAddress(instance, instance->currentLength), item, instance->size);
   instance->currentLength++;
}

static void * GetItem(I_List_t *context, uint32_t index)
{
   REINTERPRET(instance, context, List_Fixed_t *);

   return CalculateItemAddress(instance, index);
}

void List_Fixed_Init(List_Fixed_t *instance, uint32_t length, size_t size)
{
   instance->interface.Add = AddItem;
   instance->interface.Get = GetItem;

   instance->length = length;
   instance->size = size;

   instance->data = calloc((size_t)length, size);
   instance->currentLength = 0;
}

size_t List_Fixed_Length(List_Fixed_t *instance)
{
   return instance->length;
}

void List_Fixed_Deinit(List_Fixed_t *instance)
{
   free(instance->data);
}
