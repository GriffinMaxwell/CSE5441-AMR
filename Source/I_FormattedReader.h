/***
 * File: I_FormattedReader.h
 * Desc: Interface functions for using a format to parse input into some storage
 */

#ifndef I_FORMATTEDREADER_H
#define I_FORMATTEDREADER_H

typedef struct
{
   /*
    * Reads a chunk of formatted data from an input into some storage
    * @param storage: holds the formatted data
    */
   void (*Read)(void *context, void *storage);
} I_FormattedReader_t;

/*
 * Interface function call macros
 */
#define FormattedReader_Read(instance, storage) \
   (instance)->Read((instance), (storage))

#endif
