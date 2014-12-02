#ifndef __DATA_STRUCT_H__
#define __DATA_STRUCT_H__


#include <stdlib.h>
#include <string.h>

#define LIST_MAX 10


struct node
{
	void* data;
	int sort_index;
	struct node* previous_node;
	struct node* next_node;
};

struct down_sorted_list
{
	struct node* head;
	struct node* tail;
};

struct node* get_index_node(struct down_sorted_list* self, int index);
int get_length(struct down_sorted_list* self);
void add_node(struct down_sorted_list* self, struct node* input_node);
void add_all(struct down_sorted_list* self, struct down_sorted_list* src);
void remove_back_node(struct down_sorted_list* self);
void remove_back_nodes(struct down_sorted_list* self, int cnt);


#endif