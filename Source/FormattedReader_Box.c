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
      &box->position.x,
      &box->position.y,
      &box->position.height,
      &box->position.width);
}

static void ReadBoxNeighborIds(FormattedReader_Box_t *instance, List_Fixed_t *neighborIds)
{
   uint32_t numNeighbors;
   fscanf("%u", &numNeighbors);

   List_Fixed_Init(neighborIds, numNeighbors, sizeof(int));

   int index;
   for(index = 0; index < numNeighbors; index++)
   {
      int id;
      fscanf(instance->input, "%d", &id)

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
   ReadNeighborIds(instance, &box->neighborIds.top);
   ReadNeighborIds(instance, &box->neighborIds.bottom);
   ReadNeighborIds(instance, &box->neighborIds.left);
   ReadNeighborIds(instance, &box->neighborIds.right);

)

}

void FormattedReader_Box_Init(FormattedReader_Box_t *instance, FILE *input)
{
   instance->interface.Read = ReadBox;
   instance->input = input;
}
