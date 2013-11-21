/*
 * debug.h
 * Wolfrath/Kriewall, 2013
 *
 * Standard debugging functions
 */

#ifndef DEBUG_H
#define DEBUG_H

#include "../core/core.h"

void _debug(char *,char *, uint);
void _panic(char *, char*, uint);
void _assert(uint, char*, uint);
void trace_stack(uint);

#define DEBUG(x) _debug(x, __FILE__, __LINE__);
#define PANIC(x) _panic(x, __FILE__, __LINE__);
#define ASSERT(x) _assert(x, __FILE__, __LINE__);

#endif
