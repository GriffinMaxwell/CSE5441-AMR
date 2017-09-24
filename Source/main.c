/***
 * File: main.c
 */

#include <stdio.h>
#include <time.h>
#include "Map_Box.h"
#include "FormattedReader_Box.h"

static Map_Box_t mapIdToBox;
static FormattedReader_Box_t boxReader;

static double affectRate;
static double epsilon;

static int numBoxes;
static int numGridRows;
static int numGridCols;

static clock_t clockStart;
static clock_t clockEnd;
static time_t timeStart
static time_t timeEnd;
static struct timespec rtcStart;
static struct timespec rtcEnd;

static inline void StartTimers()
{
   clockStart = clock();
   timeStart = time(NULL);
   clock_gettime(CLOCK_REALTIME, &rtcStart);
}

static inline void StopTimers()
{
   clockEnd = clock();
   timeEnd = time(NULL);
   clock_gettime(CLOCK_REALTIME, &rtcEnd);
}

static void DisplayStats()
{
   printf("\n\n");
   printf("********************\n");
   printf("dissipation converged in %d iterations\n", -1);
   printf("    with max DSV = %lf and min DSV = %lf\n", -1.0, -1.0);
   printf("    AFFECT_RATE = %lf;\t\tEPSILON = %lf\n", affectRate, epsilon);
   printf("    Num boxes = %d;\t\t\tNum rows = %d;\t\tNum columns = %d\n", numBoxes, numGridRows, numGridCols);
   printf("\n");
   printf("elaspsed convergence loop time  (clock): %d", clockEnd - clockStart);
   printf("elaspsed convergence loop time   (time): %lf", difftime(timeEnd, timeStart));
   printf("elaspsed convergence loop time (chrono): %lf", (double)((rtcEnd.tv_sec - rtcStart.tv_sec) * CLOCKS_PER_SECOND));
   printf("********************\n");
}

int main(int argc, char *argv[])
{
   // Initialize objs.
   Map_Box_Init(&mapIdToBox, (uint32_t)numBoxes);
   FormattedReader_Box_Init(&boxReader, stdin);

   // Parse command line args
   sscanf(argv[1], "%lf", &affectRate);
   sscanf(argv[2], "%lf", &epsilon);

   // Read first line of stdin for number of boxes and grid dimensions
   fscanf(stdin, "%d %d %d", &numBoxes, &numGridRows, &numGridCols);

   // Read in input grid
   int index;
   for(index = 0; index < numBoxes; index++)
   {
      // Read id
      int id;
      fscanf(stdin, "%d", &id);

      // Read box
      Box_t box;
      FormattedReader_Read(&boxReader.interface, &box);

      // Store in map
      Map_Add(&mapIdToBox.interface, id, &box);
   }

   StartTimers();

   // convergence loop:
   //    iterate over boxes
   //       calculate new DSV
   //       track min/max
   //    iterate over boxes
   //       commit new DSVs

   StopTimers();

   DisplayStats();

   // Deinitialize objects
   Map_Box_Deinit(&mapIdToBox);

   return 0;
}
