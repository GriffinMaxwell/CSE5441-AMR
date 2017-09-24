/***
 * File: FormattedReader_Box.h
 * Desc: I_FormattedReader implementation that reads into a Box_t using the
 *       following format:
 *
 *       line 0:  <int: upper_left_y> <int: upper_left_x> <int: height> <int: width>
 *       line 1:  <int: num_top_neighbors> <int: top_neighbor_id> [...]
 *       line 2:  <int: num_bottom_neighbors> <int: bottom_neighbor_ids> [...]
 *       line 3:  <int: num_left_neighbors> <int: left_neighbor_ids> [...]
 *       line 4:  <int: num_right_neighbors> <int: right_neighbor_ids> [...]
 *       line 5:  <double: temperature>
 */

#ifndef FORMATTED_READER_H
#define FORMATTED_READER_H

#include <stdio.h>
#include "I_FormattedReader.h"

typedef struct
{
   I_FormattedReader_t interface;

   FILE *input;
} FormattedReader_Box_t;

/*
 * Initialize a FormattedReader_Box
 */
void FormattedReader_Box_Init(FormattedReader_Box_t *instance, FILE *input);

#endif
