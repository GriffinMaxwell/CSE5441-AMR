/***
 * File: I_FormattedReader.h
 * Desc: Interface functions for using a format to read into data storage from
 *       a stdio FILE structure
 */

#ifndef I_FORMATTEDREADER_H
#define I_FORMATTEDREADER_H

typedef struct
{
   /*
    * Reads data from a FILE into some storage based on a format.
    * @param storage: Storage to hold the formatted data
    */
   void (*Read)(void *context, void *storage);
} I_FormattedReader_t;

/*
 * Interface function call macros
 */
#define FormattedReader_Read(instance, storage) \
   (instance)->Read((instance), (storage))
   
#endif
