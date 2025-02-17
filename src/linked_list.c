#include "linked_list.h"
#include "log.h"

list_t *init_list(void) {
  list_t *list = malloc(sizeof(list_t));
  if (!list) {
    fprintf(stderr, "malloc error\n");
    return NULL;
  }
  list->size = 0;
  list->head = NULL;
  return list;
}

void free_list(list_t *list) {
  if (!list) {
    return;
  }

  if (!list->head) {
    node_t *curr_node = list->head;
    while (curr_node) {
      node_t *next_node = curr_node->next;
      free(curr_node);
      curr_node = next_node;
    }
  }
  free(list);
}

int append_to_list(list_t *list, reminder_t *reminder) {
  if (!list) {
    fprintf(stderr, "list not initialized\n");
    return 1;
  }

  node_t *new_node = malloc(sizeof(node_t));
  if (!new_node) {
    fprintf(stderr, "malloc error\n");
    return 1;
  }
  new_node->reminder = reminder;
  new_node->next = NULL;
  list->size++;

  if (list->head == NULL) {
    list->head = new_node;
    return 0;
  }

  node_t *curr_node = list->head;
  while (curr_node->next) {
    curr_node = curr_node->next;
  }
  curr_node->next = new_node;

  return 0;
}

void remove_from_list(list_t *list) {
  if (!list) {
    fprintf(stderr, "list not initialized\n");
    return;
  }

  node_t *prev_node = list->head;
  node_t *curr_node = list->head;

  while (curr_node) {
    if (curr_node->next == NULL) {
      prev_node->next = NULL;

      curr_node = curr_node->next;
      list->size--;
    } else {
      prev_node = curr_node;
      curr_node = curr_node->next;
    }
  }
  free(curr_node);
}
