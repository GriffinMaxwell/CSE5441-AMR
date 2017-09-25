/***
 * File: DsvUpdater_BoxTemperature.c
 */

#include "DsvUpdater_BoxTemperature.h"
#include "Box.h"
#include "Macro.h"

static void CalculateWeightedNeighborAverageTemperature(void *context,void *current, void *updated)
{

}

static void CommitDsvTemperature(void *context, void *current, void *updated)
{
   IGNORE(context);
   REINTERPRET(box, current, Box_t *);
   REINTERPRET(temperature, updated, double *);

   box->temperature = *nextTemperature;
}

void DsvUpdater_BoxTemperature_Init(
   DsvUpdater_BoxTemperature_t *instance,
   double affectRate)
{
   instance->interface.Calculate = CalculateWeightedNeighborAverageTemperature;
   instance->interface.Commit = CommitDsvTemperature;

   instance->affectRate = affectRate;
}
