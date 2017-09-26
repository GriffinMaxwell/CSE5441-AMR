/***
 * File: DsvUpdater_BoxTemperature.c
 */

#include "DsvUpdater_BoxTemperature.h"
#include "Macro.h"

static int BoxPerimeter(Box_t *box)
{
   return (2 * box->position.height) + (2 * box->position.width);
}

static int ContactDistance(Box_t *box, Box_t *neighbor, Box_Side_t side)
{
   int near;
   int far;

   switch(side)
   {
      // Horizontal sides use x and width
      case Box_Side_Top:
      case Box_Side_Bottom:
         near = MAX(box->position.x, neighbor->position.x);
         far = MIN((box->position.x + box->position.width), (neighbor->position.x + neighbor->position.width));
         break;

      // Vertical sides use y and height
      case Box_Side_Left:
      case Box_Side_Right:
         near = MAX(box->position.y, neighbor->position.y);
         far = MIN((box->position.y + box->position.height), (neighbor->position.y + neighbor->position.height));
         break;
   }

   return far - near;
}

static double WeightedAdjacentTemperature(
   Box_t *box,
   List_Fixed_t *neighbors,
   Map_Box_t *map,
   Box_Side_t side)
{
   double wat = 0;

   if(0 == List_Fixed_CurrentLength(neighbors))
   {
      // Use current box temperature if no neighbors on this side
      switch(side)
      {
         case Box_Side_Top:
         case Box_Side_Bottom:
            wat = box->temperature * box->position.width;
            break;

         case Box_Side_Left:
         case Box_Side_Right:
            wat = box->temperature * box->position.height;
            break;
      }
   }
   else
   {
      int i;
      for(i = 0; i < List_Fixed_CurrentLength(neighbors); i++)
      {
      	int *id = List_Get(&neighbors->interface, i);
         Box_t *neighbor = Map_Find(&map->interface, *id);

         wat += neighbor->temperature * ContactDistance(box, neighbor, side);
      }
   }

   return wat;
}

static void CalculateUpdatedTemperature(void *context, void *current, void *_updated)
{
   REINTERPRET(instance, context, DsvUpdater_BoxTemperature_t *);
   REINTERPRET(box, current, Box_t *);
   REINTERPRET(updated, _updated, double *);

   double waat =
      (WeightedAdjacentTemperature(box, &box->neighborIds.top, instance->map, Box_Side_Top)
      + WeightedAdjacentTemperature(box, &box->neighborIds.bottom, instance->map, Box_Side_Bottom)
      + WeightedAdjacentTemperature(box, &box->neighborIds.left, instance->map, Box_Side_Left)
      + WeightedAdjacentTemperature(box, &box->neighborIds.right, instance->map, Box_Side_Right))
      / BoxPerimeter(box);


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
