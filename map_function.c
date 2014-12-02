#include "map_function.h"

_bool get_near_blank(int* dest_x, int* dest_y, int src_x, int src_y,
			 int (*map)[MAP_HEIGHT]) {
	int i, j;

	for(i=-1; i<=1; ++i) {
		for(j=-1; j<=1; ++j) {
			if(src_x + i < 0)
				break;
			if(src_x + i >= MAP_WIDTH)
				break;
			if(src_y + j < 0)
				continue;
			if(src_y + j >= MAP_HEIGHT)
				continue;

			if(map[src_x + i][src_y + j] == CODE_BLANK) {
				goto GET_NEAR_BLANK_END;
			}
		}
	}

	return 0;

	GET_NEAR_BLANK_END:

	*dest_x = src_x + i;
	*dest_y = src_y + j;

	return 1;
}

_bool get_near_predator(int* dest_x, int* dest_y, int src_x, int src_y,
			 int (*map)[MAP_HEIGHT]) {
	int i, j;

	for(i=-1; i<=1; ++i) {
		for(j=-1; j<=1; ++j) {
			if(src_x + i < 0)
				break;
			if(src_x + i >= MAP_WIDTH)
				break;
			if(src_y + j < 0)
				continue;
			if(src_y + j >= MAP_HEIGHT)
				continue;

			if(map[src_x + i][src_y + j] == CODE_PREDAT0R) {
				goto GET_NEAR_PREDATOR_END;
			}
		}
	}

	return 0;

	GET_NEAR_PREDATOR_END:

	if(i == 0 && j == 0)
		return 0;

	*dest_x = src_x + i;
	*dest_y = src_y + j;

	return 1;
}