/***
 * File: PthreadArgs.h
 * Desc: Container type for arguments to a thread safe function that calculates updated Box_t DSV's
 */

#ifndef PTHREADARGS_H
#define PTHREADARGS_H

typedef struct
{
   pthread_t threadId;
} PthreadArgs_t;

#endif
