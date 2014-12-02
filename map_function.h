#ifndef __MAP_FUNCTION_H__
#define __MAP_FUNCTION_H__

#include "types.h"

#define DISTANCE(x, y) ( (x) * (x) + (y) * (y) )

_bool get_near_blank(int* dest_x, int* dest_y, int src_x, int src_y,
			 int (*map)[MAP_HEIGHT]);
_bool get_near_predator(int* dest_x, int* dest_y, int src_x, int src_y,
			 int (*map)[MAP_HEIGHT]);

#endif