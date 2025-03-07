#pragma once

#include <stdio.h>

#include "reminder.h"

#define MAX_PATH_LENGTH 1024

#define MAX_LINE_LENGTH 1024

FILE *open_storage_file(void);
int close_storage_file(FILE *storagefile_fd);

int append_reminder_to_storage_file(FILE *fd, const reminder_t *reminder);
int read_reminder_from_storage_file(FILE *fd, reminder_t *reminder_dest);

