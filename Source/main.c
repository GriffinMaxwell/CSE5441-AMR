/***
 * File: main.c
 */

#include <stdio.h>
#include "Map_Box.h"
#include "FormattedReader_Box.h"

static Map_Box_t mapIdToBox;
static FormattedReader_Box_t boxReader;

int main(int argc, char *argv[])
{
   // Read command line args
   double affectRate;
   double epsilon;
   sscanf(argv[1], "%lf", &affectRate);
   sscanf(argv[2], "%lf", &epsilon);

   // Read first line of stdin for # boxes and grid size
   int numBoxes;
   int numGridRows;
   int numGridCols;
   fscanf(stdin, "%d %d %d", &numBoxes, &numGridRows, &numGridCols);

   // Initialize objs.
   Map_Box_Init(&mapIdToBox, (uint32_t)numBoxes);

   FormattedReader_Box_Init(&boxReader, stdin);

   // Read in input grid
      // Read id
      // Read box
      // Store in map
   int index;
   for(index = 0; index < numBoxes; index++)
   {
      int id;
      fscanf(stdin, "%d", &id);

      Box_t box;
      FormattedReader_Read(&boxReader.interface, &box);

      Map_Add(&mapIdToBox.interface, id, &box);
   }

   // start timers
   // convergence loop:
   //    iterate over boxes
   //       calculate new DSV
   //       track min/max
   //    iterate over boxes
   //       commit new DSVs
   // stop timers

   // display stats
	printf("AFFECT_RATE: %lf\n", affectRate);
	printf("EPSILON: %lf\n", epsilon);
	printf("num boxes: %d\n", numBoxes);
	printf("num rows: %d\n", numGridRows);
	printf("num cols: %d\n", numGridCols);

   // Deinitialize objects
   Map_Box_Deinit(&mapIdToBox);

   return 0;
}
