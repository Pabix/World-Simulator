#include "select_lib.h"


int arithmetic_roulette_select(unsigned int range) {
	srand(time(NULL));

	unsigned int seed = range * ( range + 1 ) / 2;

	int random_num = rand() % seed;


	while(random_num < 0)
		random_num -= range--;

	return range;
}