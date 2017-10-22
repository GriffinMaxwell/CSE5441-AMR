/***
 * File: Map_Double.c
 */

#include <stdlib.h>
#include "Map_Double.h"
#include "Macro.h"

static void AddEntry(void *context, int key, void *_value)
{
   REINTERPRET(instance, context, Map_Double_t *);
	REINTERPRET(value, _value, double *);
	
   instance->doubles[key] = *value;
}

/*
 * Assumes key maps to valid double. Only returns NULL if key is too big and would
 *  cause an array out-of-bounds
 */
static void * FindEntry(void *context, int key)
{
   REINTERPRET(instance, context, Map_Double_t *);

   return (key < instance->length) ? &instance->doubles[key] : NULL;
}

void Map_Double_Init(Map_Double_t *instance, uint32_t length)
{
   instance->interface.Add = AddEntry;
   instance->interface.Find = FindEntry;

   instance->doubles = calloc((size_t)length, sizeof(double));
   instance->length = length;
}

void Map_Double_Deinit(Map_Double_t *instance)
{
   free(instance->doubles);
}
