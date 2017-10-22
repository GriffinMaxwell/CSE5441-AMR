/***
 * File: Macro.h
 * Desc: Useful macros
 */

#ifndef MACRO_H
#define MACRO_H

#define IGNORE(arg) \
   (void)arg

#define MAX(a, b) \
   ((a) > (b)) ? (a) : (b)

#define HAS_CONVERGED(max, min, epsilon) \
      (((max) - (min)) <= ((epsilon) * (max)))

#define MIN(a, b) \
   ((a) > (b)) ? (b) : (a)

#define REINTERPRET(new_name, old_name, new_type) \
   new_type new_name = (new_type)old_name

#endif
