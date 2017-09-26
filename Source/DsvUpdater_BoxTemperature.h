/***
 * File: DsvUpdater_BoxTemperature.h
 * Desc: I_DsvUpdater implementation where the DSV is the temperature field
 *       inside a Box_t. Uses a weighted average of the neighboring Box_t
 *       temperatures proportioned by the supplied affect rate to Calculate
 *       the updated DSVs.
 */

#ifndef DSVUPDATER_BOXTEMPERATURE_H
#define DSVUPDATER_BOXTEMPERATURE_H

#include "I_DsvUpdater.h"
#include "Map_Box_t"

typdef struct
{
   DsvUpdater_BoxTemperature_t interface;

   Map_Box_t *map;
   double affectRate;
} DsvUpdater_BoxTemperature_t;

/*
 * Initialize a DsvUpdater_BoxTemperature
 * @param map: map storing all of the boxes and IDs in the grid
 * @param affectRate: The proportion that the weighted neighbor average affects the current DSV
 */
void DsvUpdater_BoxTemperature_Init(
   DsvUpdater_BoxTemperature_t *instance,
   Map_Box_t *map,
   double affectRate);

#endif
