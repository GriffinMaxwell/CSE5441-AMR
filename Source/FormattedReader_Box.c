/***
 * File: FromattedReader_Box.c
 */

#include <string.h>
#include "FormattedReader_Box.h"
#include "Box.h"
#include "Macro.h"

static void ReadPosition(FormattedReader_Box_t *instance, Box_t *box)
{
   fscanf(
      instance->input,
      "%d %d %d %d",
      &box->position.y,
      &box->position.x,
      &box->position.height,
      &box->position.width);
}

static void ReadNeighborIds(FormattedReader_Box_t *instance, List_Fixed_t *neighborIds)
{
   uint32_t numNeighbors;
   fscanf(instance->input, "%u", &numNeighbors);

   List_Fixed_Init(neighborIds, numNeighbors, sizeof(int));

   int i;
   for(i = 0; i < numNeighbors; i++)
   {
      int id;
      fscanf(instance->input, "%d", &id);
      List_Add(&neighborIds->interface, &id);
   }
}

static void ReadTemperature(FormattedReader_Box_t *instance, Box_t *box)
{
   fscanf(instance->input, "%lf", &box->temperature);
}

static void ReadBox(void *context, void *storage)
{
   REINTERPRET(instance, context, FormattedReader_Box_t *);
   REINTERPRET(box, storage, Box_t *);

   ReadPosition(instance, box);
   ReadNeighborIds(instance, &box->neighborIds.top);
   ReadNeighborIds(instance, &box->neighborIds.bottom);
   ReadNeighborIds(instance, &box->neighborIds.left);
   ReadNeighborIds(instance, &box->neighborIds.right);
   ReadTemperature(instance, box);
}

void FormattedReader_Box_Init(FormattedReader_Box_t *instance, FILE *input)
{
   instance->interface.Read = ReadBox;
   instance->input = input;
}
