/***
 * File: Macro.h
 * Desc: Macros needed throughout the project
 */

#ifndef MACRO_H
#define MACRO_H

#define IGNORE(arg) \
   (void)arg
   
#define REINTERPRET(new_name, old_name, new_type) \
   new_type new_name = (new_type)old_name

#endif
