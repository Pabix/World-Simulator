#ifndef __PREDATOR_H_
#define __PREDATOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "data_struct.h"
#include "select_lib.h"
#include "types.h"
#include "map_function.h"

struct predator_gene
{
	int float_pattern;
	int hunt_pattern;
	int achievement_cnt;
};

struct predator
{
	int x, y;
	unsigned int direct;
	int generation;
	int sight;
	int life_cnt;
	int on_delay;
	struct predator_gene* own_gene;
	struct down_sorted_list* parent_genes;
};

struct predator* get_baby(struct predator* husband, struct predator* wife,
				 int (*map)[MAP_HEIGHT]);
void active_predator(struct predator* self, int (*map)[MAP_HEIGHT]);


#endif