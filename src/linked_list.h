#pragma once

#include "reminder.h"
#include <stdlib.h>

typedef struct node_s {
  reminder_t *reminder;
  struct node_s *next;
} node_t;

typedef struct list_s {
  size_t size;
  node_t *head;
} list_t;

void free_list(list_t *list);
list_t *init_list(void);
void remove_from_list(list_t *list);
int append_to_list(list_t *list, reminder_t *reminder);

