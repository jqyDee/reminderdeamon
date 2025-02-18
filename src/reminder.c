#include "reminder.h"

#include <stdio.h>
#include <sys/errno.h>

#include "log.h"
#include "storagefile.h"
#include "times.h"

reminder_t create_reminder(date_t reminder_date, unsigned alerts[MAX_ALERTS],
                           size_t alerts_num, char *label, char *note) {
  if (alerts_num > MAX_ALERTS) {
    alerts_num = MAX_ALERTS;
  }

  reminder_t new_rem = {
      .done = 0,
      .creation_date = get_current_time_as_date_t(),
      .reminder_date = reminder_date,
      .label = label,
      .note = note,
      .alerts_num = alerts_num,
  };

  for (size_t i = 0; i < alerts_num; i++) {
    new_rem.alerts[i] = create_date_from_offset(reminder_date, alerts[i]);
  }

  return new_rem;
}

int print_reminder(reminder_t *reminder) {
  printf("---------------------\n");
  printf("Reminder at:\n");
  printf("---------------------\n");
  if (print_date(reminder->reminder_date)) {
    return 1;
  }

  if (reminder->done) {
    printf("Done: Yes\n");
  } else {
    printf("Done: No\n");
  }

  printf("Label: %s\n", reminder->label);
  printf("Note: %s\n", reminder->note);

  printf("\nAlerts: \n");
  for (size_t i = 1; i <= reminder->alerts_num; i++) {
    printf("  -  ");
    if (print_alert(reminder->alerts[i - 1])) {
      return 1;
    }

    if (i != reminder->alerts_num) {
      printf(",\n");
    }
  }
  printf("\n");

  printf("Created at: ");
  if (print_alert(reminder->creation_date)) {
    return 1;
  }
  printf("\n");

  printf("---------------------\n");

  return 0;
}

int main(void) {
  date_t reminder_date = {
      .year = 2004,
      .month = JAN,
      .day = 1,
      .weekday = calculate_weekday(2004, JAN, 1),
      .hour = 0,
      .minute = 1,
  };

  size_t alerts_num = 3;
  unsigned alerts[3] = {24, 48, 72};

  reminder_t reminder = create_reminder(reminder_date, alerts, alerts_num,
                                        "My Brithday", "I was born there");

  print_reminder(&reminder);

  return 0;
}
