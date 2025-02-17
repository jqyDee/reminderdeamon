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

  // TODO: Move in times.c and fix issue with hours getting lower than 0
  for (size_t i = 0; i < alerts_num; i++) {
    log("i = %zu; alerts[i] = %u\n", alerts_num, alerts[i]);
    unsigned hours_before_reminder = alerts[i];
    unsigned days_before_reminder = 0;
    unsigned years_before_reminder = 0;

    unsigned buffer = 0;
    // check if more than 24
    if (hours_before_reminder >= 24) {
      buffer = hours_before_reminder % 24;
      days_before_reminder = (hours_before_reminder - buffer) / 24;
      hours_before_reminder = buffer;
      // check if more than 365 days
      if (days_before_reminder >= 365) {
        buffer = days_before_reminder % 365;
        years_before_reminder = (days_before_reminder - buffer) / 24;
        days_before_reminder = buffer;
      }
    }

    date_t new_date = reminder_date;
    new_date.year = new_date.year - years_before_reminder;
    // TODO: new_date.month = new_date.month - years_before_reminder;
    new_date.day = new_date.day - days_before_reminder;
    new_date.hour = new_date.hour - hours_before_reminder;
    new_rem.alerts[i] = new_date;
    print_alert(new_date);
    printf("\n");
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

  printf("\nAlerts: ");
  for (size_t i = 1; i <= reminder->alerts_num; i++) {
    if (print_alert(reminder->alerts[i - 1])) {
      return 1;
    }

    if (i != reminder->alerts_num) {
      printf(", ");
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
  FILE *storagefile_fd = open_storage_file();
  if (!storagefile_fd) {
    return 1;
  }

  if (close_storage_file(storagefile_fd)) {
    return 1;
  }

  date_t reminder_date = {
      .year = 2004,
      .month = 1,
      .day = 1,
      .weekday = calculate_weekday(2004, 1, 1),
      .hour = 1,
      .minute = 1,
  };

  size_t alerts_num = 3;
  unsigned alerts[3] = {2, 4, 6};

  reminder_t reminder = create_reminder(reminder_date, alerts, alerts_num, "My Brithday",
                                        "I was born there");

  print_reminder(&reminder);

  return 0;
}
