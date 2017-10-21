/***
 * File: main.c
 */

#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include "Map_Box.h"
#include "FormattedReader_Box.h"
#include "DsvUpdater_BoxTemperature.h"
#include "Macro.h"

#define MS_PER_SEC (1000)
#define NS_PER_MS (1000000)

static Map_Box_t mapIdToBox;
static Map_Box_t mapIdToUpdatedTemperature;
static FormattedReader_Box_t boxReader;
static DsvUpdater_BoxTemperature_t dsvUpdater;

static double affectRate;
static double epsilon;
static int numThreads;

static uint32_t numBoxes;
static uint32_t numGridRows;
static uint32_t numGridCols;
static uint32_t numIterations;

static clock_t clockStart;
static clock_t clockEnd;
static time_t timeStart;
static time_t timeEnd;
static struct timespec rtcStart;
static struct timespec rtcEnd;

static double minTemperature;
static double maxTemperature;

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
   printf("\n");
   printf("********************************************************************************\n");
   printf("temperature dissipation converged in %d iterations\n", numIterations);
   printf("    with max DSV = %lf and min DSV = %lf\n", maxTemperature, minTemperature);
   printf("    AFFECT_RATE = %lf;\tEPSILON = %lf\n", affectRate, epsilon);
   printf("    Num boxes = %d;\tNum rows = %d;\tNum columns = %d\n", numBoxes, numGridRows, numGridCols);
   printf("\n");

   printf("elaspsed convergence loop time:\n");
   printf("    using clock():          %d clicks (%f s)\n",
      clockEnd - clockStart,
      ((float)(clockEnd - clockStart))/CLOCKS_PER_SEC);
   printf("    using time():           %.f s\n", difftime(timeEnd, timeStart));
   printf("    using clock_gettime():  %.3f ms\n",
      (double)(((rtcEnd.tv_sec - rtcStart.tv_sec) * MS_PER_SEC)
      + ((rtcEnd.tv_nsec - rtcStart.tv_nsec) / NS_PER_MS)));
   printf("********************************************************************************\n");
}

static void ReadInputGrid()
{
   uint32_t i;
   for(i = 0; i < numBoxes; i++)
   {
      int id;
      fscanf(stdin, "%d", &id);

      Box_t box;
      FormattedReader_Read(&boxReader.interface, &box);

      Map_Add(&mapIdToBox.interface, id, &box);
   }
}

static void * ThreadSafeCalculateUpdatedBoxTemperatures(void * args);
{
   REINTERPRET(threadId, args, int *);

	uint32_t i;
   for(i = *threadId; i < numBoxes; i += numThreads)
   {
      Box_t *box = Map_Find(&mapIdToBox.interface, i);
      if(NULL != box)
      {
         double updatedTemperature;
			DsvUpdater_Calculate(&dsvUpdater.interface, box, &updatedTemperature);
			Map_Add(&mapIdToUpdatedTemperature.interface, i, &updatedTemperature);
      }
   }

   free(threadId);
   pthread_exit(NULL);
}

static void CommitUpdatedBoxTemperaturesAndFindMinMax()
{
	uint32_t i;
   for(i = 0; i < numBoxes; i++)
   {
      Box_t *box = Map_Find(&mapIdToBox.interface, i);
      if(NULL != box)
      {
         double *updatedTemperature = Map_Find(&mapIdToUpdatedTemperature.interface, i);
         DsvUpdater_Commit(&dsvUpdater.interface, box, updatedTemperature);

         if(i == 0)
         {
            minTemperature = *updatedTemperature;
            maxTemperature = *updatedTemperature;
         }
         else
         {
            minTemperature = MIN(minTemperature, *updatedTemperature);
            maxTemperature = MAX(maxTemperature, *updatedTemperature);
         }
      }
   }
}

int main(int argc, char *argv[])
{
   if (argc < 4)
   {
      printf("Error: Not enough arguments.");
      printf("Should be: AFFECT_RATE EPSILON NUM_THREADS");
      return 0;
   }

   // Parse command line args
   sscanf(argv[1], "%lf", &affectRate);
   sscanf(argv[2], "%lf", &epsilon);
   sscanf(argv[3], "%d", &numThreads);

   // Read first line of stdin for number of boxes and grid dimensions
   fscanf(stdin, "%d %d %d", &numBoxes, &numGridRows, &numGridCols);

   // Initialize objects
   Map_Box_Init(&mapIdToBox, (uint32_t)numBoxes);
   Map_Box_Init(&mapIdToUpdatedTemperature, (uint32_t)numBoxes);
   FormattedReader_Box_Init(&boxReader, stdin);
   DsvUpdater_BoxTemperature_Init(&dsvUpdater, &mapIdToBox, affectRate);

	ReadInputGrid();

   StartTimers();

   // Convergence loop
	for(numIterations = 0; !MEETS_CONVERGENCE_CONDITION(maxTemperature, minTemperature, epsilon); numIterations++)
   {
      int *threadId;
      pthread_t threads[numThreads];

      int i;
      for(i = 0; i < numThreads; i++)
      {
         threadId = malloc(sizeof(int));
         pthread_create(&threads[i], NULL, ThreadSafeCalculateUpdatedBoxTemperatures, (void *)threadId);
      }
      for(i = 0; i < numThreads; i++)
      {
         void *threadStatus;
         pthread_join(threads[i], &threadStatus);
      }

      CommitUpdatedBoxTemperaturesAndFindMinMax();
   }

   StopTimers();

   DisplayStats();

   // Deinitialize objects
   Map_Box_Deinit(&mapIdToBox);
   List_Fixed_Deinit(&updatedTemperatureIds);
   List_Fixed_Deinit(&updatedTemperatures);

   return 0;
}
