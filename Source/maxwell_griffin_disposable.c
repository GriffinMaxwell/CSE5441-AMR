/***
 * File: main.c
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "Map_Box.h"
#include "Map_Double.h"
#include "FormattedReader_Box.h"
#include "DsvUpdater_BoxTemperature.h"
#include "Macro.h"

#define MS_PER_SEC (1000)
#define NS_PER_MS (1000000)

// Objects
static Map_Box_t mapIdToBox;
static Map_Double_t mapIdToUpdatedTemperature;
static FormattedReader_Box_t boxReader;
static DsvUpdater_BoxTemperature_t dsvUpdater;

// User args and OpenMP actual threads
static double affectRate;
static double epsilon;
static int numRequestedThreads;
static int numActualThreads;

// Box dimensions
static uint32_t numBoxes;
static uint32_t numGridRows;
static uint32_t numGridCols;
static uint32_t numIterations;

// Timing variables
static clock_t clockStart;
static clock_t clockEnd;
static time_t timeStart;
static time_t timeEnd;
static struct timespec rtcStart;
static struct timespec rtcEnd;

// Min Max Temperatures
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
   printf("Using \"disposable\" threading with OpenMP:\n");
   printf("temperature dissipation converged in %d iterations\n", numIterations);
   printf(" -  with requested number of threads = %d\n", numRequestedThreads);
   printf(" -  with actual number of threads = %d\n", numActualThreads);
   printf(" -  with max DSV = %lf and min DSV = %lf\n", maxTemperature, minTemperature);
   printf(" -  AFFECT_RATE = %lf;\tEPSILON = %lf\n", affectRate, epsilon);
   printf(" -  Num boxes = %d;\tNum rows = %d;\tNum columns = %d\n", numBoxes, numGridRows, numGridCols);
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
   int i;
   for(i = 0; i < numBoxes; i++)
   {
      int id;
      fscanf(stdin, "%d", &id);

      Box_t box;
      FormattedReader_Read(&boxReader.interface, &box);

      Map_Add(&mapIdToBox.interface, id, &box);
   }
}

static void CalculateBlockOfBoxTemperatures(int threadId, int numThreads)
{
	// block distribution parameters
	int boxesPerThread = numBoxes / numThreads;
	int start = threadId * boxesPerThread;
	int end =  // gives leftover boxes to the last thread
      (threadId == numActualThreads - 1)
      ? numBoxes
      : start + boxesPerThread;
   
   int i;
   for(i = start; i < end; i++)
   {
      Box_t *box = Map_Find(&mapIdToBox.interface, i);
      if(NULL != box)
      {
         double updatedTemperature;
			DsvUpdater_Calculate(&dsvUpdater.interface, box, &updatedTemperature);
			Map_Add(&mapIdToUpdatedTemperature.interface, i, &updatedTemperature);
      }
   }
}

static void CommitBoxTemperaturesAndFindMinMax()
{
   int i;
   for(i = 0; i < numBoxes; i++)
   {
      Box_t *box = Map_Find(&mapIdToBox.interface, i);
      if(NULL != box)
      {
         double *updatedTemperature = Map_Find(&mapIdToUpdatedTemperature.interface, i);
         DsvUpdater_Commit(&dsvUpdater.interface, box, updatedTemperature);

         minTemperature = (i == 0) ? *updatedTemperature : MIN(minTemperature, *updatedTemperature);
         maxTemperature = (i == 0) ? *updatedTemperature : MAX(maxTemperature, *updatedTemperature);
      }
   }
}

int main(int argc, char *argv[])
{
   if (argc < 4)
   {
      printf("Error: Not enough arguments.\n");
      printf("Should be: AFFECT_RATE EPSILON NUM_REQUESTED_THREADS\n");
      return 0;
   }

   // Parse command line args
   sscanf(argv[1], "%lf", &affectRate);
   sscanf(argv[2], "%lf", &epsilon);
   sscanf(argv[3], "%d", &numRequestedThreads);

   // Read first line of stdin for number of boxes and grid dimensions
   fscanf(stdin, "%d %d %d", &numBoxes, &numGridRows, &numGridCols);

   // Initialize objects
   Map_Box_Init(&mapIdToBox, numBoxes);
   Map_Double_Init(&mapIdToUpdatedTemperature, numBoxes);
   FormattedReader_Box_Init(&boxReader, stdin);
   DsvUpdater_BoxTemperature_Init(&dsvUpdater, &mapIdToBox, affectRate);

	ReadInputGrid();

   StartTimers();

   // Convergence loop
   bool hasConverged = false;
	for(numIterations = 0; !hasConverged; numIterations++)
   {         
      #pragma omp parallel num_threads(numRequestedThreads)
      {
         numActualThreads = omp_get_num_threads();
         CalculateBlockOfBoxTemperatures(omp_get_thread_num(), numActualThreads);
      }

      CommitBoxTemperaturesAndFindMinMax();
      hasConverged = HAS_CONVERGED(maxTemperature, minTemperature, epsilon);
   }

   StopTimers();
   DisplayStats();

   // Deinitialize objects
   Map_Box_Deinit(&mapIdToBox);
   Map_Double_Deinit(&mapIdToUpdatedTemperature);

   return 0;
}
