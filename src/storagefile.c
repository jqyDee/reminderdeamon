#include "storagefile.h"

#include <sys/errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "log.h"
#include "reminder.h"

int write_to_fd(FILE *fd, reminder_t *rem) {
  return 0;
}

static char *get_home_directory(void) {
  char home_dir_path[MAX_PATH];

  snprintf(home_dir_path, MAX_PATH, "%s", getenv("HOME"));
  log("%s\n", home_dir_path);

  return strdup(home_dir_path);
}

static char *get_filepath_to_storage_file(void) {
  char filepath[MAX_PATH];

  strcpy(filepath, get_home_directory());
  strcat(filepath, "/.config/.reminderd_store");

  log("%s\n", filepath);

  return strdup(filepath);
}

FILE *open_storage_file(void) {
  char *storagefile_path = get_filepath_to_storage_file();

  FILE *storagefile_fd;

  // try reading file
  storagefile_fd = fopen(storagefile_path, "r");
  if (!storagefile_fd) {
    log("could not read %s; trying to create!\n", storagefile_path);
  }

  // try creating file
  storagefile_fd = fopen(storagefile_path, "w");
  if (!storagefile_fd) {
    log("could not read or create storage file %s\n", storagefile_path);
    fprintf(stderr, "error opening storage_file; errno %d\n", errno);
    return NULL;
  }

  return storagefile_fd;
}

int close_storage_file(FILE *storagefile_fd) {
  if (fclose(storagefile_fd)) {
    fprintf(stderr, "error closing storage_file; errno %d\n", errno);
    return 1;
  }
  return 0;
}


