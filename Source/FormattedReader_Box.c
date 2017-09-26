/***
 * File: FromattedReader_Box.c
 */

#include <string.h>
#include "FormattedReader_Box.h"
#include "Box.h"
#include "Macro.h"

static void ReadBoxPositions(FormattedReader_Box_t *instance, Box_t *box)
{
   fscanf(
      instance->input,
      "%d %d %d %d",
      &box->position.y,
      &box->position.x,
      &box->position.height,
      &box->position.width);
}

static void ReadBoxNeighborIds(FormattedReader_Box_t *instance, List_Fixed_t *neighborIds)
{
   uint32_t numNeighbors;
   fscanf(instance->input, "%u", &numNeighbors);

   List_Fixed_Init(neighborIds, numNeighbors, sizeof(int));

   int index;
   for(index = 0; index < numNeighbors; index++)
   {
      int id;
      fscanf(instance->input, "%d", &id);

      List_Add(&neighborIds->interface, &id);
   }
}

static void ReadBoxTemperature(FormattedReader_Box_t *instance, Box_t *box)
{
   fscanf(instance->input, "%lf", &box->temperature);
}

static void ReadBox(void *context, void *storage)
{
   REINTERPRET(instance, context, FormattedReader_Box_t *);
   REINTERPRET(box, storage, Box_t *);

   ReadBoxPositions(instance, box);
   ReadBoxNeighborIds(instance, &box->neighborIds.top);
   ReadBoxNeighborIds(instance, &box->neighborIds.bottom);
   ReadBoxNeighborIds(instance, &box->neighborIds.left);
   ReadBoxNeighborIds(instance, &box->neighborIds.right);
   ReadBoxTemperature(instance, box);
}

void FormattedReader_Box_Init(FormattedReader_Box_t *instance, FILE *input)
{
   instance->interface.Read = ReadBox;
   instance->input = input;
}
