#include "predator.h"
#include "types.h"
#include "data_struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern struct down_sorted_list gene_storage;
extern struct down_sorted_list predator_storage;

void init(int (*map)[MAP_HEIGHT]);
void print(int (*map)[MAP_HEIGHT]);
void sreenshot(int (*map)[MAP_HEIGHT]);
void process(int (*map)[MAP_HEIGHT]);

int main(int argc, char const *argv[])
{
	int map[MAP_WIDTH][MAP_HEIGHT];
	int command;
	int continue_cnt = 0;

	int gene_storage_size;
	int predator_storage_size;
	int i;

	init(map);

	while(1) {
		system("clear");
		system("clear");

		print(map);
		process(map);


		if(continue_cnt-- > 0) 
			continue;

		printf("(1 계속 2 종료 3 스크린샷)명령 : ");
		scanf("%d", &command);

		switch(command) {
		case 1:
			printf("횟수 : ");
			scanf("%d", &continue_cnt);
			break;
		case 2:
			return 0;
			break;
		case 3:
			sreenshot(map);
			break;
		}

		fflush(stdout);
		usleep(50);
	}

	gene_storage_size = get_length(&gene_storage);
	predator_storage_size = get_length(&predator_storage);

	for(i = 0; i < gene_storage_size; ++i) {
		free(get_index_node(&gene_storage, i)->data);
	}
	remove_back_nodes(&gene_storage, gene_storage_size);

	for(i = 0; i < predator_storage_size; ++i) {
		free(get_index_node(&predator_storage, i)->data);
	}
	remove_back_nodes(&predator_storage, predator_storage_size);

	return 0;
}

void init(int (*map)[MAP_HEIGHT]) {
	int i, j;

	srand(time(NULL));

	for(i = 0; i < MAP_WIDTH; ++i) {
		for(j = 0; j < MAP_HEIGHT; ++j) {
			map[i][j] = CODE_BLANK;
		}
	}

	for(i = 0; i < PREY_INIT_CNT; ++i) {
		int new_x = rand() % MAP_WIDTH;
		int new_y = rand() % MAP_HEIGHT;

		if(map[new_x][new_y] == CODE_BLANK)
			map[new_x][new_y] = CODE_PREY;
	}

	for(i = 0; i < PREDATOR_INIT_CNT; ++i) {

		int new_x = rand() % MAP_WIDTH;
		int new_y = rand() % MAP_HEIGHT;

		if(map[new_x][new_y] != CODE_BLANK)
			continue;

		struct predator* new_predator = malloc(sizeof(struct predator));
		struct node* new_predator_node = malloc(sizeof(struct node));
		struct predator_gene* new_gene = malloc(sizeof(struct predator_gene));
		struct node* new_gene_node = malloc(sizeof(struct node));
		struct down_sorted_list* new_parent_gene = malloc(sizeof(struct down_sorted_list));
		new_predator->direct = rand() % DIRECT_CNT;
		new_predator->generation = 0;
		new_predator->life_cnt = PREDATOR_LIFE_SPAN;
		new_predator->on_delay = 0;

		new_gene->float_pattern = rand();
		new_gene->hunt_pattern = rand();
		new_gene->achievement_cnt = 0;

		new_gene_node->data = new_gene;
		new_gene_node->sort_index = 0;
		add_node(&gene_storage, new_gene_node);

		new_predator->own_gene = new_gene;

		new_parent_gene->head = NULL;
		new_parent_gene->tail = NULL;

		new_predator->parent_genes = new_parent_gene;

		new_predator->sight = PREDATOR_SIGHT;
		new_predator->x = new_x;
		new_predator->y = new_y;

		map[new_x][new_y] = CODE_PREDAT0R;

		new_predator_node->data = new_predator;
		new_predator_node->sort_index = new_predator->own_gene->achievement_cnt;
		add_node(&predator_storage, new_predator_node);
	}
}

void print(int (*map)[MAP_HEIGHT]) {
	int i, j;

	for(i = 0; i < MAP_WIDTH; ++i) {
		for(j = 0; j < MAP_HEIGHT; ++j) {
			char ch;
			switch(map[i][j]) {
			case CODE_BLANK:
				ch = ' ';
				break;
			case CODE_PREY:
				ch = '*';
				break;
			case CODE_PREDAT0R:
				ch = '@';
				break;
			}

			printf("%2c", ch);
		}
		printf("\n");
	}
}

void sreenshot(int (*map)[MAP_HEIGHT]) {
	FILE* file;
	int i, j;

	srand(time(NULL));

	file = fopen("screenshot.txt", "w");

	for(i = 0; i < MAP_WIDTH; ++i) {
		for(j = 0; j < MAP_HEIGHT; ++j) {
			char ch;
			switch(map[i][j]) {
			case CODE_BLANK:
				ch = ' ';
				break;
			case CODE_PREY:
				ch = '*';
				break;
			case CODE_PREDAT0R:
				ch = '@';
				break;
			}

			fprintf(file, "%c", ch);
		}
		fprintf(file, "\n");
	}	
}

void process(int (*map)[MAP_HEIGHT]) {
	int size = get_length(&predator_storage);
	int i;

	for(i = 0; i < size; ++i) {
		struct predator* tmp;
		tmp = get_index_node(&predator_storage, i)->data;
		active_predator(tmp, map);
	}
}