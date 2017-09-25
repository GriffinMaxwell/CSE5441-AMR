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

static void AddItem(void *context, void *item)
{
   REINTERPRET(instance, context, List_Fixed_t *);

   memcpy(CalculateItemAddress(instance, instance->currentLength), item, instance->size);
   instance->currentLength++;
}

static void * GetItem(void *context, uint32_t index)
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

uint32_t List_Fixed_CurrentLength(List_Fixed_t *instance)
{
   return instance->currentLength;
}

void List_Fixed_Reinit(List_Fixed_t *instance)
{
   instance->currentLength = 0;
}

void List_Fixed_Deinit(List_Fixed_t *instance)
{
   free(instance->data);
}
