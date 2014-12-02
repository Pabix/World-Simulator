#include "data_struct.h"
#include <stdio.h>

struct node* get_index_node(struct down_sorted_list* self, int index) {
	struct node* result = self->head;

	while(index-- && ( result->next_node != NULL) )
		result = result->next_node;

	return result;
}

int get_length(struct down_sorted_list* self) {
	int length = 0;

	struct node* tmp = self->head;

	while(tmp != NULL) {
		++length; 
		tmp = tmp->next_node;
	}


	return length;
}

void add_node(struct down_sorted_list* self, struct node* _input_node) {
	struct node* tmp;

	struct node* input_node = malloc(sizeof(struct node));
	input_node->data = _input_node->data;
	input_node->sort_index = _input_node->sort_index;
	input_node->next_node = NULL;
	input_node->previous_node = NULL;


	if(self->head == NULL) {
		self->head = input_node;
		self->tail = self->head;
		input_node->previous_node = NULL;
		input_node->next_node = NULL;
		goto end;
	}


	tmp = self->head;

	while( tmp != NULL) {
		if(tmp->sort_index <= input_node->sort_index) {
			if(tmp == self->head) {
				input_node->next_node = tmp;
				tmp->previous_node = input_node;

				input_node->previous_node = NULL;
				self->head = input_node;

				goto end;
			}

			tmp->previous_node->next_node = input_node;

			input_node->previous_node = tmp->previous_node;
			input_node->next_node = tmp;

			tmp->previous_node = input_node;
			goto end;
		}
		tmp = tmp->next_node;
	}

	self->tail->next_node = input_node;
	input_node->previous_node = self->tail;

	input_node->next_node = NULL;
	self->tail = input_node;
	goto end;

	end:
	return;
}

void add_all(struct down_sorted_list* self, struct down_sorted_list* src) {
	struct node* tmp = src->head;
	int size;


	if(tmp == NULL)
		return;

	while(tmp != NULL) {
		add_node(self, tmp);
		tmp = tmp->next_node;
	}


	size = get_length(self);
	if(size > LIST_MAX) {
		remove_back_nodes(self, size - LIST_MAX);
	}


}

void remove_back_node(struct down_sorted_list* self) {
	struct node* target_node = self->tail;

	if(target_node == NULL)
		return;

	if(target_node->previous_node == NULL) {
		free(target_node);
		self->head = NULL;
		self->tail = NULL;
	}

	target_node->previous_node->next_node = NULL;

	self->tail = target_node->previous_node;

	free(target_node);
}

void remove_back_nodes(struct down_sorted_list* self, int cnt) {
	struct node* tmp = self->tail;

	if(tmp == NULL) {
		return;
	}

	while(cnt-- && tmp != NULL) {
		remove_back_node(self);
		tmp = tmp->previous_node;
	}
};