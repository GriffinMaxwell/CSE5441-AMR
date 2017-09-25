/***
 * File: main.c
 */

#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include "Map_Box.h"
#include "FormattedReader_Box.h"

#define NS_PER_US (1000)

static Map_Box_t mapIdToBox;
static FormattedReader_Box_t boxReader;
static List_Fixed_t activeIds;
static List_Fixed_t nextTemperatures;

static double affectRate;
static double epsilon;

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
   printf("********************\n");
   printf("dissipation converged in %d iterations\n", -1);
   printf("    with max DSV = %lf and min DSV = %lf\n", -1.0, -1.0);
   printf("    AFFECT_RATE = %lf;\tEPSILON = %lf\n", affectRate, epsilon);
   printf("    Num boxes = %d;\tNum rows = %d;\tNum columns = %d\n", numBoxes, numGridRows, numGridCols);
   printf("\n");
   printf("elaspsed convergence loop time  (clock): %lf\n", clockEnd - clockStart);
   printf("elaspsed convergence loop time   (time): %lf\n", difftime(timeEnd, timeStart));
   printf("elaspsed convergence loop time (chrono): %lf\n", (double)(((rtcEnd.tv_sec - rtcStart.tv_sec) * CLOCKS_PER_SEC) + ((rtcEnd.tv_nsec - rtcStart.tv_nsec) / NS_PER_US)));
   printf("********************\n");
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

static void CalculateNewBoxTemperaturesAndCheckMinMax()
{
	bool firstBox = true;
	uint32_t i;
   for(i = 0; i < numBoxes; i++)
   {
      Box_t *box = Map_Find(&mapIdToBox.interface, i);
      if(NULL != box)
      {
         double nextTemperature;// = CaculateNewBoxTemperature();
         List_Add(&activeIds.interface, &i);
         List_Add(&nextTemperatures.interface, &nextTemperature);

         if(firstBox)
         {
         	firstBox = false;
         	minTemperature = nextTemperature;
         	maxTemperature = nextTemperature;
         }
         else if(nextTemperature < minTemperature)
         {
         	minTemperature = nextTemperature;
       	}
       	else if(nextTemperature > maxTemperature)
       	{
       		maxTemperature = nextTemperature;
       	}
      }
   }
}

static void CommitNewBoxTemperatures()
{
	uint32_t i;
   for(i = 0; i < List_Fixed_CurrentLength(&activeIds); i++)
   {
      int *id = List_Get(&activeIds.interface, i);
      double *nextTemperature = List_Get(&nextTemperatures.interface, i);

      Box_t *box = Map_Find(&mapIdToBox.interface, *id);
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
   List_Fixed_Init(&activeIds, numBoxes, sizeof(int));
   List_Fixed_Init(&nextTemperatures, numBoxes, sizeof(double));

	ReadInputGrid();

   StartTimers();

	numIterations = 0;
   do {
      List_Fixed_Reset(&activeIds);
      List_Fixed_Reset(&nextTemperatures);

      CalculateNewBoxTemperaturesAndCheckMinMax();
      CommitNewBoxTemperatures();
      
      numIterations++;
   } while(maxTemperature - minTemperature > epsilon);

   StopTimers();

   DisplayStats();

   // Deinitialize objects
   Map_Box_Deinit(&mapIdToBox);
   List_Fixed_Deinit(&activeIds);
   List_Fixed_Deinit(&nextTemperatures);

   return 0;
}
