/***
 * File: I_DsvUpdater.h
 * Desc: Interface functions for calculating and committing updated DSVs
 */

#ifndef I_DSVUPDATER_H
#define I_DSVUPDATER_H

typedef struct
{
   /*
    * Calculates the updated DSV and stores in a separate argument.
    * @param current: contains the current DSV
    * @param updated: storage for the updated DSV
    */
    void (*Calculate)(void *context, void *current, void *updated);

    /*
    * Commits the updated DSV to the current DSV.
    * @param current: contains the current DSV
    * @param updated: updated DSV to commit to the current
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
