/*
 * debug.h
 * Joel Wolfrath, 2013
 *
 * Standard debugging functions
 */

#ifndef DEBUG_H
#define DEBUG_H

#include "../core/core.h"

void _debug(char *,char *, uint32);
void _panic(char *, char*, uint32);
void _assert(int32, char*, uint32);
void trace_stack(uint32);

#define DEBUG(x) _debug(x, __FILE__, __LINE__);
#define PANIC(x) _panic(x, __FILE__, __LINE__);
#define ASSERT(x) _assert(x, __FILE__, __LINE__);

#endif
