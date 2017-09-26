/***
 * File: DsvUpdater_BoxTemperature.c
 */

#include "DsvUpdater_BoxTemperature.h"
#include "Macro.h"

typedef enum
{
   BoxSide_Top,
   BoxSide_Bottom,
   BoxSide_Left,
   BoxSide_Right
} BoxSide_t;

static int BoxPerimeter(Box_t *box)
{
   return (2 * box->position.height) + (2 * box->position.width);
}

static int ContactDistance(Box_t *box, Box_t *neighbor, BoxSide_t side)
{
   int near;
   int far;

   switch(side)
   {
      case BoxSide_Top:
      case BoxSide_Bottom:
         near = MAX(box->position.x, neighbor->position.x);
         far = MIN((box->position.x + box->position.width), (neighbor->position.x + neighbor->position.width));
         break;

      case BoxSide_Left:
      case BoxSide_Right:
         near = MAX(box->position.y, neighbor->position.y);
         far = MIN((box->position.y + box->position.height), (neighbor->position.y + neighbor->position.height));
         break;
   }
   
   return far - near;
}

static double WeightedAdjacentTemperature(Box_t *box, Map_Box_t *map, BoxSide_t side)
{
   List_Fixed_t *neighbors;
   switch (side)
   {
      case BoxSide_Top:
         neighbors = &box->neighborIds.top;
         break;
      case BoxSide_Bottom:
         neighbors = &box->neighborIds.bottom;
         break;
      case BoxSide_Left:
         neighbors = &box->neighborIds.left;
         break;
      case BoxSide_Right:
         neighbors = &box->neighborIds.right;
         break;
   }

   double sum = 0;

   uint32_t neighborsLength = List_Fixed_CurrentLength(neighbors);
   if(neighborsLength == 0)
   {
      // Use current box temperature if no neighbors on this side
      switch(side)
      {
         case BoxSide_Top:
         case BoxSide_Bottom:
            sum = box->temperature * box->position.width;
            break;

         case BoxSide_Left:
         case BoxSide_Right:
            sum = box->temperature * box->position.height;
            break;
      }
   }
   else
   {
      int i;
      for(i = 0; i < neighborsLength; i++)
      {
      	int *id = List_Get(&neighbors->interface, i);
         Box_t *neighbor = Map_Find(&map->interface, *id);
         sum += neighbor->temperature * ContactDistance(box, neighbor, side);
      }
   }

   return sum;
}

static void CalculateUpdatedTemperature(void *context, void *current, void *_updated)
{
   REINTERPRET(instance, context, DsvUpdater_BoxTemperature_t *);
   REINTERPRET(box, current, Box_t *);
   REINTERPRET(updated, _updated, double *);

   double sum = WeightedAdjacentTemperature(box, instance->map, BoxSide_Top)
      + WeightedAdjacentTemperature(box, instance->map, BoxSide_Bottom)
      + WeightedAdjacentTemperature(box, instance->map, BoxSide_Left)
      + WeightedAdjacentTemperature(box, instance->map, BoxSide_Right);

   double waat =  sum / BoxPerimeter(box);

   *updated = box->temperature + ((waat - box->temperature) * instance->affectRate);
}

static void CommitTemperature(void *context, void *current, void *updated)
{
   IGNORE(context);
   REINTERPRET(box, current, Box_t *);
   REINTERPRET(temperature, updated, double *);

   box->temperature = *temperature;
}

void DsvUpdater_BoxTemperature_Init(
   DsvUpdater_BoxTemperature_t *instance,
   Map_Box_t *map,
   double affectRate)
{
   instance->interface.Calculate = CalculateUpdatedTemperature;
   instance->interface.Commit = CommitTemperature;

   instance->map = map;
   instance->affectRate = affectRate;
}
