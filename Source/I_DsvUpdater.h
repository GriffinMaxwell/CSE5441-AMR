/***
 * File: I_DsvUpdater.h
 * Desc: Interface functions for calculating and committing one or more DSVs
 */

#ifndef I_DSVUPDATER_H
#define I_DSVUPDATER_H

typedef struct
{
   /*
    * Calculates the updated DSV value as a separate piece of data.
    * @param current: object that is or contains the current DSV
    * @param updated: storage for the updated DSV(s)
    */
    void (*Calculate)(void *context, void *current, void *updated);

    /*
    * Calculates the updated DSV value as a separate piece of data.
    * @param current: object that is or contains the current DSV
    * @param updated: the updated DSV(s) to commit to the current
    */
    void (*Commit)(void *context, void *current, void *updated);
} I_DsvUpdater_t;

/*
 * Interface function call macros
 */
#define DsvUpdater_Calculate(instance, current, updated) \
   (instance)->Calculate((instance), (current), (updated))

#define DsvUpdater_Commit(instance, current, updated) \
   (instance)->Commit((instance), (current), (updated))

#endif
