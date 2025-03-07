#include "storagefile.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>

#include "log.h"
#include "reminder.h"
#include "times.h"

static char *get_home_directory(void) {
  char home_dir_path[MAX_PATH_LENGTH];

  snprintf(home_dir_path, MAX_PATH_LENGTH, "%s", getenv("HOME"));
  log("%s\n", home_dir_path);

  return strdup(home_dir_path);
}

static char *get_filepath_to_storage_file(void) {
  char filepath[MAX_PATH_LENGTH];
  char *home_dir = get_home_directory();

  strcpy(filepath, home_dir);
  strcat(filepath, "/.config/.reminderd_store");

  free(home_dir);

  log("%s\n", filepath);

  return strdup(filepath);
}

FILE *open_storage_file(void) {
  char *storagefile_path = get_filepath_to_storage_file();

#ifdef DEBUG
  free(storagefile_path);
  storagefile_path = "./.reminderd_store";
#endif

  FILE *storagefile_fd;

  // try reading file
  storagefile_fd = fopen(storagefile_path, "r+");
  if (!storagefile_fd) {
    log("could not read %s; trying to create\n", storagefile_path);

    // try creating file
    storagefile_fd = fopen(storagefile_path, "w");
    if (!storagefile_fd) {
      log("could not read or create storage file %s\n", storagefile_path);
      fprintf(stderr, "error opening storage_file; errno %d\n", errno);
      return NULL;
    }
    log("storage file created\n");
  }

#ifndef DEBUG
  free(storagefile_path);
#endif

  return storagefile_fd;
}

int close_storage_file(FILE *storagefile_fd) {
  if (fclose(storagefile_fd)) {
    fprintf(stderr, "error closing storage_file; errno %d\n", errno);
    return 1;
  }
  return 0;
}

int append_reminder_to_storage_file(FILE *fd, const reminder_t *reminder) {
  if (!fd) {
    fprintf(stderr, "ERROR: file descriptor is NULL\n");
    return 1;
  }

  fseek(fd, 0, SEEK_END);

  // separator
  fprintf(fd, "\t\t\t\n");

  // done
  fprintf(fd, "done=%d\n", reminder->done);

  // label
  fprintf(fd, "label=%s\n", reminder->label);

  // note
  fprintf(fd, "note=%s\n", reminder->note);

  // creation date
  char *creation_date_string = create_string_from_date(reminder->creation_date);
  fprintf(fd, "creation_date=%s\n", creation_date_string);
  free(creation_date_string);

  // reminder date
  char *reminder_date_string = create_string_from_date(reminder->reminder_date);
  fprintf(fd, "reminder_date=%s\n", reminder_date_string);
  free(reminder_date_string);

  // alerts num
  fprintf(fd, "alerts_num=%zu\n", reminder->alerts_num);

  // alerts
  fprintf(fd, "ALERTS_START:\n");
  for (size_t i = 0; i < reminder->alerts_num; i++) {
    char *alert_string = create_string_from_date(reminder->alerts[i]);
    fprintf(fd, "%s\n", alert_string);
    free(alert_string);
  }
  fprintf(fd, "ALERTS_END:\n");

  rewind(fd);

  return 0;
}

int read_reminder_from_storage_file(FILE *fd, reminder_t *reminder_dest) {
  if (!fd) {
    fprintf(stderr, "ERROR: file descriptor is NULL\n");
    return 1;
  }
  char line[MAX_LINE_LENGTH];

  int separator_read = 0;
  int done_read = 0;
  int label_read = 0;
  int note_read = 0;
  int creation_date_read = 0;
  int reminder_date_read = 0;
  int alerts_num_read = 0;
  int alerts_read = 0;

  char str_buffer[MAX_LINE_LENGTH];
  date_t date_buffer = {0};

  while (fgets(line, sizeof(line), fd) != NULL) {
    if (strcmp(line, "\t\t\t") == 0) {
      separator_read++;
      continue;
    };
    if (sscanf(line, "done=%d", &reminder_dest->done) == 1) {
      done_read++;
      continue;
    }
    if (sscanf(line, "label=%[^\t\n]", str_buffer)) {
      reminder_dest->label = strdup(str_buffer);
      label_read++;
      continue;
    }
    if (sscanf(line, "note=%[^\t\n]", str_buffer)) {
      reminder_dest->note = strdup(str_buffer);
      note_read++;
      continue;
    }
    if (sscanf(line, "creation_date=%u-%02d-%02hd: %02hd:%02hd; %s",
               &date_buffer.year, &date_buffer.month, &date_buffer.day,
               &date_buffer.hour, &date_buffer.minute, str_buffer) == 6) {

      date_buffer.weekday = string_to_weekday(str_buffer);
      reminder_dest->creation_date = date_buffer;
      creation_date_read++;
      continue;
    }
    if (sscanf(line, "reminder_date=%u-%02d-%02hd: %02hd:%02hd; %s",
               &date_buffer.year, &date_buffer.month, &date_buffer.day,
               &date_buffer.hour, &date_buffer.minute, str_buffer) == 6) {

      date_buffer.weekday = string_to_weekday(str_buffer);
      reminder_dest->reminder_date = date_buffer;
      creation_date_read++;
      continue;
    }
    if (sscanf(line, "alerts_num=%zu", &reminder_dest->alerts_num) == 1) {
      alerts_num_read++;
      continue;
    }
    if (strcmp(line, "ALERTS_START:") == 0) {
      size_t index = 0;
      while (fgets(line, sizeof(line), fd) != NULL) {
        if (strcmp(line, "ALERTS_END:") == 0) {
          break;
        } else {
          if (sscanf(line, "%u-%02d-%02hd: %02hd:%02hd; %s",
                     &date_buffer.year, &date_buffer.month, &date_buffer.day,
                     &date_buffer.hour, &date_buffer.minute, str_buffer) == 6) {
            reminder_dest->alerts[index] = date_buffer;
            alerts_read++;
            index++;
          }
        }
      }
    }
  }

  return 0;
}
