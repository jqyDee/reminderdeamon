#include <stdio.h>

#include "linked_list.h"

int init_list_tests(void) {
  int return_value = 0;
  printf(" -- list init tests\n");

  list_t *list = init_list();

  // check if list was malloced
  if (list == NULL) {
    fprintf(stderr, "FAILED => list was not initialized\n");
    return_value++;
  } else {
    printf("PASSED => list is successfully malloced\n");
  }

  // check if size of list is correct
  if (list->size != 0) {
    fprintf(stderr, "FAILED => list was not initialized with size zero\n");
    return_value++;
  } else {
    printf("PASSED => list correct size\n");
  }

  // check if head of list has been set correctly
  if (list->head != NULL) {
    fprintf(stderr, "FAILED => list head was not initialized with 'NULL'\n");
    return_value++;
  } else {
    printf("PASSED => list head is NULL\n");
  }

  free(list);

  return return_value;
}

int append_list_tests(void) {
  int return_value = 0;
  printf(" -- list append tests\n");

  list_t *list = init_list();

  // append
  reminder_t rem1 = {.reminder_date.day = 1};
  append_to_list(list, &rem1);
  reminder_t rem2 = {.reminder_date.day = 2};
  append_to_list(list, &rem2);
  reminder_t rem3 = {.reminder_date.day = 3};
  append_to_list(list, &rem3);

  // check list size
  if (list->size != 3) {
    fprintf(stderr, "FAILED => list size was not updated correctly\n");
    return_value++;
  } else {
    printf("PASSED => list size updated correctly\n");
  }

  node_t *node = list->head;
  // check head
  if (!list->head) {
    fprintf(stderr, "FAILED => head not updated correctly\n");
    return_value++;
  } else {
    printf("PASSED => head updated correctly\n");
  }

  // check appended reminders
  if (node->reminder->reminder_date.day != 1 || node->next->reminder->reminder_date.day != 2 ||
      node->next->next->reminder->reminder_date.day != 3) {
    fprintf(stderr, "FAILED => reminders not appended correctly\n");
    return_value++;
  } else {
    printf("PASSED => reminders appended correctly\n");
  }

  free_list(list);

  return return_value;
}

int remove_list_tests(void) {
  int return_value = 0;
  printf(" -- list remove tests\n");

  list_t *list = init_list();

  // append
  reminder_t rem1 = {.reminder_date.day = 1};
  append_to_list(list, &rem1);
  reminder_t rem2 = {.reminder_date.day = 2};
  append_to_list(list, &rem2);
  reminder_t rem3 = {.reminder_date.day = 3};
  append_to_list(list, &rem3);

  remove_from_list(list);
  remove_from_list(list);
  remove_from_list(list);

  if (list->size != 0) {
    fprintf(stderr, "FAILED => list size not updated correctly\n");
    return_value++;
  } else {
    printf("PASSED => list size updated correctly\n");
  }

  return return_value;
}

int filepath_tests(void) {
  int return_value = 0;
  printf(" -- filepath tests\n");

  

  return return_value;
}

int file_tests(void) {
  int return_value = 0;
  printf("--------------- FILE TESTS ---------------\n");

  const int filepath_ret = filepath_tests();

  return return_value;
}


int list_tests(void) {
  int return_value = 0;
  printf("--------------- LIST TESTS ---------------\n");

  const int init_list_ret = init_list_tests();

  const int append_list_ret = append_list_tests();

  const int remove_list_ret = remove_list_tests();

  return return_value;
}

int main(void) {
  int return_value = 0;
  int list_tests_ret = list_tests();

  printf("\n--------------- ALL TESTS ----------------\n");
  if (return_value) {
    fprintf(stderr, "FAILED %d TESTS\n", return_value);
  } else {
    printf("PASSED ALL\n");
  }

  return return_value;
}
