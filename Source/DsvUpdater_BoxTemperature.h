/***
 * File: DsvUpdater_BoxTemperature.h
 * Desc: I_DsvUpdater implementation where the DSV is the temperature of a
 *       Box_t. Uses a weighted adjacent average of the neighbor temperatures
 *       proportioned by the supplied affect rate to calculate the updated DSVs.
 */

#ifndef DSVUPDATER_BOXTEMPERATURE_H
#define DSVUPDATER_BOXTEMPERATURE_H

#include "I_DsvUpdater.h"
#include "Map_Box.h"

typedef struct
{
   I_DsvUpdater_t interface;

   // Private members
   Map_Box_t *map;
   double affectRate;
} DsvUpdater_BoxTemperature_t;

/*
 * Initialize a DsvUpdater_BoxTemperature
 * @param map: all of the boxes in the grid and their IDs
 * @param affectRate: The proportion that the WAAT affects a box's temperature
 */
void DsvUpdater_BoxTemperature_Init(
   DsvUpdater_BoxTemperature_t *instance,
   Map_Box_t *map,
   double affectRate);

#endif
