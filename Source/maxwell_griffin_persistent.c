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

static Map_Box_t mapIdToBox;
static Map_Double_t mapIdToUpdatedTemperature;
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

static bool hasConverged = false;
static pthread_barrier_t barrierCalculate, barrierCommit, barrierConverged;

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
   printf("    with number of (disposable) pthreads = %d\n", numThreads);
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

static void * CalculateUpdatedBoxTemperatures(int start, int end)
{
   uint32_t i;
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

static void CommitUpdatedBoxTemperatures(int start, int end)
{
   uint32_t i;
   for(i = start; i < end; i++)
   {
      DsvUpdater_Commit(
         &dsvUpdater.interface,
         Map_Find(&mapIdToBox.interface, i),
         Map_Find(&mapIdToUpdatedTemperature.interface, i));
   }
}

static void FindMinMax()
{
   // Start with first temperature
   double updatedTemperature = *(double *)Map_Find(&mapIdToUpdatedTemperature.interface, 0);
   minTemperature = updatedTemperature;
   maxTemperature = updatedTemperature;

   uint32_t i;
   for(i = 1; i < numBoxes; i++)
   {
         updatedTemperature = *(double *)Map_Find(&mapIdToUpdatedTemperature.interface, i);

         minTemperature = MIN(minTemperature, updatedTemperature);
         maxTemperature = MAX(maxTemperature, updatedTemperature);
   }
}

static void * ThreadSafeConvergenceLoop(void *args)
{
   REINTERPRET(threadId, args, int *);

   // Convergence loop
   while(!hasConverged)
   {
      // block distribution
      int boxesPerThread = numBoxes / numThreads;
      int start = (*threadId) * boxesPerThread;
      int end = (*threadId == numThreads - 1) ? numBoxes : start + boxesPerThread;	// gives leftover boxes to the last thread

      CalculateUpdatedBoxTemperatures(start, end);
      pthread_barrier_wait(&barrierCalculate);

      CommitUpdatedBoxTemperatures(start, end);
      pthread_barrier_wait(&barrierCommit);

      // Wait for main thread to update convergence condition
      pthread_barrier_wait(&barrierConverged);
   }

   free(threadId);
   pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
   if (argc < 4)
   {
      printf("Error: Not enough arguments.\n");
      printf("Should be: AFFECT_RATE EPSILON NUM_THREADS\n");
      return 0;
   }

   // Parse command line args
   sscanf(argv[1], "%lf", &affectRate);
   sscanf(argv[2], "%lf", &epsilon);
   sscanf(argv[3], "%d", &numThreads);

   // Set number of threads (or just have clause in parallel region?)

   // Read first line of stdin for number of boxes and grid dimensions
   fscanf(stdin, "%d %d %d", &numBoxes, &numGridRows, &numGridCols);

   // Initialize objects
   Map_Box_Init(&mapIdToBox, (uint32_t)numBoxes);
   Map_Double_Init(&mapIdToUpdatedTemperature, (uint32_t)numBoxes);
   FormattedReader_Box_Init(&boxReader, stdin);
   DsvUpdater_BoxTemperature_Init(&dsvUpdater, &mapIdToBox, affectRate);

   ReadInputGrid();

   StartTimers();

   // Parallelize this whole convergence loop (request # threads? other clauses?)
   // {
   // Fix this for loop so that hasConverged is atomic?
   for(numIterations = 0; !hasConverged; numIterations++)
   {
      // Store actual number of threads
      // CalculateUpdatedBoxTemperatures

      // Barrier
      // CommitUpdatedBoxTemperatures
      // Barrier

      // omp only one thread should run this
      // {
         FindMinMax();
         hasConverged = HAS_CONVERGED(maxTemperature, minTemperature, epsilon);
      // }
   }
   // }

   StopTimers();
   DisplayStats();

   // Destroy barrier and persistent threads
   for(i = 0; i < numThreads; i++)
   {
      void *threadStatus;
      pthread_join(threads[i], &threadStatus);
   }

   // Deinitialize objects
   Map_Box_Deinit(&mapIdToBox);
   Map_Double_Deinit(&mapIdToUpdatedTemperature);

   return 0;
}
