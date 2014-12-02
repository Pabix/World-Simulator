#include "predator.h"

struct down_sorted_list gene_storage;
struct down_sorted_list predator_storage;

struct predator* get_point_predator(int x, int y) {
	struct node* tmp;
	int size = get_length(&predator_storage);
	int i;

	for(i = 0; i < size; ++i) {
		tmp = get_index_node(&predator_storage, i);
		if(x == ( (struct predator*)tmp->data )->x && 
			y == ( (struct predator*)tmp->data )->y) {
			return tmp->data;
		}
	}

	return NULL;
}

void decide_direct(struct predator* self, int (*map)[MAP_HEIGHT]) {
	int i, j;
	int target_x = 0x7FFFFFFF, target_y = 0x7FFFFFFF; /* max value of integer */
	unsigned int result_direct;

	for(i=-(self->sight); i<(self->sight); ++i) {
		for(j=-(self->sight); j<(self->sight); ++j) {
			if(self->x + i < 0)
				break;
			if(self->x + i >= MAP_WIDTH)
				break;
			if(self->x + j < 0)
				continue;
			if(self->y + j >= MAP_HEIGHT)
				continue;

			if(map[self->x + i][self->y + j] == CODE_PREY) {
				if(DISTANCE(target_x, target_y) > 
					DISTANCE(self->x + i, self->y + j)) {
					target_x = self->x + i;
					target_y = self->y + j;
				}
			}
		}
	}

	result_direct = self->own_gene->float_pattern & 0x3;	/* get direct */
		self->own_gene->float_pattern >>= 2;
		self->own_gene->float_pattern &= 0x3FFFFFFF;
		self->own_gene->float_pattern |= ( result_direct << 30 );

	if(target_x == 0x7FFFFFFF && target_y == 0x7FFFFFFF) {
		goto DECIDE_DIRECT_END;
	}

	result_direct = self->own_gene->hunt_pattern & 0x3;	/* get direct */
		self->own_gene->hunt_pattern >>= 2;
		self->own_gene->hunt_pattern |= ( result_direct << 30 );

	if(target_x > self->x) {
		if(result_direct == DIRECT_LEFT)
			result_direct = DIRECT_RIGHT;
	} else {
		if(result_direct == DIRECT_RIGHT)
			result_direct = DIRECT_LEFT;
	}

	if(target_y > self->y) {
		if(result_direct == DIRECT_DOWN)
			result_direct = DIRECT_UP;
	} else {
		if(result_direct == DIRECT_UP)
			result_direct = DIRECT_DOWN;
	}

	DECIDE_DIRECT_END:

	self->direct = result_direct;

	return;
}

void decide_gene(struct predator* self) {
	int heredity_seed;
	int gene_seed;
	int parent_gene_length;


	srand(time(NULL));


	parent_gene_length = get_length(self->parent_genes);
	if(parent_gene_length < PREDATOR_HEREDITY_SEED) {
		gene_seed = arithmetic_roulette_select(parent_gene_length);
		goto DECIDE_GENE_END;
	}

	heredity_seed = rand() % PREDATOR_PROBABILITY_SEED; /* 1000 */

	if(heredity_seed < PREDATOR_MUTATION_PROBABILITY) { /* 10 */
		gene_seed = arithmetic_roulette_select(PREDATOR_MUTATION_SEED);
		goto DECIDE_GENE_END;
	}

	if(heredity_seed < PREDATOR_ODD_HEREDITY_PROBABILITY) { /* 1 */
		gene_seed = arithmetic_roulette_select(PREDATOR_ODD_HEREDITY_SEED);
		goto DECIDE_GENE_END;
	}

	gene_seed = arithmetic_roulette_select(PREDATOR_HEREDITY_SEED); /* 6 */
	goto DECIDE_GENE_END;


	DECIDE_GENE_END:
	self->own_gene = malloc(sizeof(struct predator_gene));
	memcpy(self->own_gene, get_index_node(self->parent_genes, gene_seed)->data, sizeof(struct predator_gene));
	self->own_gene->achievement_cnt = 0;
}


struct predator* get_baby(struct predator* husband, struct predator* wife,
				int (*map)[MAP_HEIGHT]) {
	struct predator* baby;
	struct node* husband_gene_node;
	struct node* wife_gene_node;

	struct node* new_gene_node;
	struct node* new_predator_node;

	if(wife->on_delay != 0)
		return NULL;
	if(husband->on_delay != 0)
		return NULL;


	baby = malloc(sizeof(struct predator));

	if(get_near_blank(&baby->x, &baby->y, wife->x, wife->y, map) == 1) {
		map[baby->x][baby->y] = CODE_PREDAT0R;
	} else {
		free(baby);
		return NULL;
	}

	baby->direct = rand() % DIRECT_CNT;
	baby->generation = husband->generation > wife->generation ? 
				husband->generation + 1 : wife->generation + 1;
	baby->life_cnt = PREDATOR_LIFE_SPAN;
	baby->on_delay = 1;
	baby->sight = PREDATOR_SIGHT;
	baby->parent_genes = malloc(sizeof(struct down_sorted_list));
	baby->parent_genes->head = NULL;
	baby->parent_genes->tail = NULL;
	add_all(baby->parent_genes, husband->parent_genes);
	add_all(baby->parent_genes, wife->parent_genes);

	husband_gene_node = malloc(sizeof(struct node));
	husband_gene_node->data = husband->own_gene;
	husband_gene_node->sort_index = husband->own_gene->achievement_cnt;
	wife_gene_node = malloc(sizeof(struct node));
	wife_gene_node->data = wife->own_gene;
	wife_gene_node->sort_index = wife->own_gene->achievement_cnt;

	add_node(baby->parent_genes, husband_gene_node);
	add_node(baby->parent_genes, wife_gene_node);

	decide_gene(baby);

	new_gene_node = malloc(sizeof(struct node));
	new_gene_node->data = baby->own_gene;
	new_gene_node->sort_index = 0;
	add_node(&gene_storage, new_gene_node);

	new_predator_node = malloc(sizeof(struct predator));
	new_predator_node->data = baby;
	new_predator_node->sort_index = baby->own_gene->achievement_cnt;
	add_node(&predator_storage, new_predator_node);

	wife->on_delay = PREDATOR_MARRY_DELAY;
	wife->life_cnt -= PREDATOR_MARRY_PUNISHMENT;
	husband->on_delay = PREDATOR_MARRY_DELAY;
	husband->life_cnt -= PREDATOR_MARRY_PUNISHMENT;
	baby->on_delay = PREDATOR_BABY_DELAY;

	return baby;
}

void active_predator(struct predator* self, int (*map)[MAP_HEIGHT]) {
	int original_x = self->x;
	int original_y = self->y;
	int wife_x;
	int wife_y;

	if(self->on_delay == 30000)
		return;

	if(self->on_delay > 0) {
		self->on_delay--;
		return;
	}

	if(self->on_delay < 0) {
		self->on_delay++;
	}

	map[self->x][self->y] = CODE_BLANK;

	if(self->direct == DIRECT_UP)
		++self->y;
	else if(self->direct == DIRECT_DOWN)
		--self->y;
	else if(self->direct == DIRECT_LEFT)
		--self->x;
	else if(self->direct == DIRECT_RIGHT)
		++self->x;

	if(self->x < 0)
		self->x = 0;
	if(self->x >= MAP_WIDTH)
		self->x = MAP_WIDTH - 1;
	if(self->y < 0)
		self->y = 0;
	if(self->y >= MAP_HEIGHT)
		self->y = MAP_HEIGHT - 1;

	if(map[self->x][self->y] == CODE_PREY) {
		self->life_cnt = PREDATOR_LIFE_SPAN;
		self->on_delay = 1;
	}

	if(map[self->x][self->y] == CODE_PREDAT0R) {
		self->x = original_x;
		self->y = original_y;
	}

	map[self->x][self->y] = CODE_PREDAT0R;
	map[original_x][original_y] = CODE_BLANK;


	if(get_near_predator(&wife_x, &wife_y, self->x, self->y, map) == 1) {
		struct predator* wife;
		wife = get_point_predator(wife_x, wife_y);
		if(wife != NULL)
			get_baby(self, wife, map);
	}

	decide_direct(self, map);

	self->life_cnt--;
	self->own_gene->achievement_cnt++;

	if(self->life_cnt <= 0){
		map[self->x][self->y] = CODE_BLANK;
		self->x = -1;
		self->y = -1;
		self->on_delay = 30000;
	}
}





















