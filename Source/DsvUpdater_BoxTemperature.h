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

typdef struct
{
   DsvUpdater_BoxTemperature_t interface;

   double affectRate;
} DsvUpdater_BoxTemperature_t;

/*
 * Initialize a DsvUpdater_BoxTemperature
 * @param affectRate: The proportion that the weighted neighbor average affects the current DSV
 */
void DsvUpdater_BoxTemperature_Init(
   DsvUpdater_BoxTemperature_t *instance,
   double affectRate);

#endif
