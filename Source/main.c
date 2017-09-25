/***
 * File: main.c
 */

#include <stdio.h>
#include <time.h>
#include "Map_Box.h"
#include "FormattedReader_Box.h"

static Map_Box_t mapIdToBox;
static FormattedReader_Box_t boxReader;
static List_Fixed_t activeIds;
static List_Fixed_t nextTemperatures;

static double affectRate;
static double epsilon;

static uint32_t numBoxes;
static uint32_t numGridRows;
static uint32_t numGridCols;

static clock_t clockStart;
static clock_t clockEnd;
static time_t timeStart
static time_t timeEnd;
static struct timespec rtcStart;
static struct timespec rtcEnd;

static double minTemperature;
static double maxTemperature;
static uint32_t i;

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

static void CalculateNewBoxTemperatures()
{
   for(i = 0; i < numBoxes; i++)
   {
      Box_t *box = Map_Find(&mapIdToBox, i)
      if(NULL != box)
      {
         double nextTemperature;// = CaculateNewBoxTemperature();
         List_Add(&activeIds.interface, &i);
         List_Add(&nextTemperatures.interface, &nextTemperature);

         // min max
      }
   }
}

static void CommitNewBoxTemperatures()
{
   for(i = 0; i < List_Fixed_CurrentLength(instance->); i++)
   {
      int *id = List_Get(&activeIds.interface, i);
      double *nextTemperature = List_Get(&nextTemperatures, i);

      Box_t *box = Map_Find(&mapIdToBox, *id);
      box->temperature = *nextTemperature;
   }
}

int main(int argc, char *argv[])
{
   // Parse command line args
   sscanf(argv[1], "%lf", &affectRate);
   sscanf(argv[2], "%lf", &epsilon);

   // Read first line of stdin for number of boxes and grid dimensions
   fscanf(stdin, "%d %d %d", &numBoxes, &numGridRows, &numGridCols);

   // Initialize objects
   Map_Box_Init(&mapIdToBox, (uint32_t)numBoxes);
   FormattedReader_Box_Init(&boxReader, stdin);
   List_Fixed_Init(activeIds, numBoxes, sizeof(int));
   List_Fixed_Init(nextTemperatures, numBoxes, sizeof(double));

   // Read in input grid
   for(i = 0; i < numBoxes; i++)
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
   do {
      List_Fixed_Reset(&activeIds);
      List_Fixed_Reset(&nextTemperature);

      CalculateNewBoxTemperatures();
      CommitNewBoxTemperatures();
   } while(maxTemperature - minTemperature > epsilon);

   StopTimers();

   DisplayStats();

   // Deinitialize objects
   Map_Box_Deinit(&mapIdToBox);
   List_Fixed_Deinit(&activeIds);
   List_Fixed_Deinit(&nextTemperatures);

   return 0;
}
