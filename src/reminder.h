#pragma once

#include "times.h"

#include <stdlib.h>

#define MAX_ALERTS 8

#define MAX_LABEL_LENGTH 32
#define MAX_NOTE_LENGTH 256

typedef struct reminder_s {
  unsigned done;

  char *label;
  char *note;

  date_t creation_date, reminder_date;
  size_t alerts_num;
  date_t alerts[MAX_ALERTS];
} reminder_t;

reminder_t create_reminder(date_t reminder_date, unsigned alerts[MAX_ALERTS],
                           size_t alerts_num, char *label, char *note);
