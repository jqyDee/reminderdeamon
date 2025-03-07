#include "times.h"

#include <string.h>
#include <time.h>

#include "log.h"

static int check_if_leap_year(unsigned year) {
  if (year % 4 == 0) {
    if (year % 100 != 0) {
      return 1;
    }
    if (year % 400 == 0) {
      return 1;
    }
  }
  return 0;
}

date_t get_current_time_as_date_t(void) {
  time_t rawtime;
  struct tm *timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);

  date_t curr_time = {.year = timeinfo->tm_year + 1900,
                      .month = timeinfo->tm_mon,
                      .day = timeinfo->tm_mday,
                      .hour = timeinfo->tm_hour,
                      .minute = timeinfo->tm_min,
                      .weekday = timeinfo->tm_wday};

  log("current time: %s", asctime(timeinfo));

  return curr_time;
}

static char *get_weekday(weekday_t weekday) {
  switch (weekday) {
  case MON:
    return "Monday";
  case TUE:
    return "Tuesday";
  case WED:
    return "Wednesday";
  case THU:
    return "Thursday";
  case FRI:
    return "Friday";
  case SAT:
    return "Saturday";
  case SUN:
    return "Sunday";
  default:
    return NULL;
  }
}

static char *get_month(month_t month) {
  switch (month) {
  case JAN:
    return "January";
  case FEB:
    return "February";
  case MAR:
    return "March";
  case APR:
    return "April";
  case MAI:
    return "Mai";
  case JUN:
    return "June";
  case JUL:
    return "July";
  case AUG:
    return "August";
  case SEP:
    return "September";
  case OKT:
    return "Oktober";
  case NOV:
    return "November";
  case DEC:
    return "December";
  default:
    return NULL;
  }
}

weekday_t calculate_weekday(unsigned y, unsigned m, unsigned d) {
  return (weekday_t)((d += m < 3 ? y-- : y - 2,
                      23 * m / 9 + d + 4 + y / 4 - y / 100 + y / 400) %
                     7);
}

int print_alert(date_t date) {
  char *weekday_str = get_weekday(date.weekday);
  if (!weekday_str) {
    fprintf(stderr, "ERROR: weekday isn't correct\n");
    return 1;
  }

  printf("[%u-%02d-%02u: %02u:%02u; %s]", date.year, date.month + 1, date.day,
         date.hour, date.minute, weekday_str);
  return 0;
}

int print_date(date_t date) {
  char *weekday_str = get_weekday(date.weekday);
  if (!weekday_str) {
    fprintf(stderr, "ERROR: weekday isn't correct\n");
    return 1;
  }
  char *month_str = get_month(date.month);
  if (!month_str) {
    fprintf(stderr, "ERROR: month isn't correct\n");
    return 1;
  }

#ifdef DEBUG
  printf("[%u-%d-%u: %02u:%02u; %s]\n", date.year, date.month + 1, date.day,
         date.hour, date.minute, weekday_str);
#endif

  if (date.day <= 31) {
    // Monday, 1st of January 2025 at 01:01
    if (date.day == 1) {
      printf("%s, %02dst of %s %u at %02u:%02u\n", weekday_str, date.day,
             month_str, date.year, date.hour, date.minute);
      // Monday, 2nd of January 2025 at 01:01
    } else if (date.day == 2) {
      printf("%s, %02dnd of %s %u at %02u:%02u\n", weekday_str, date.day,
             month_str, date.year, date.hour, date.minute);
      // Monday, 3rd of January 2025 at 01:01
    } else if (date.day == 3) {
      printf("%s, %02drd of %s %u at %02u:%02u\n", weekday_str, date.day,
             month_str, date.year, date.hour, date.minute);
      // Monday, 4th of January 2025 at 01:01
    } else {
      printf("%s, %02dth of %s %u at %02u:%02u\n", weekday_str, date.day,
             month_str, date.year, date.hour, date.minute);
    }
  } else {
    fprintf(stderr, "ERROR: day is not correct\n");
    return 1;
  }
  return 0;
}

static unsigned short month_lookup[12] = {
    31, // JAN
    28, // FEB
    31, // MAR
    30, // APR
    31, // MAI
    30, // JUN
    31, // JUL
    31, // AUG
    30, // SEP
    31, // OCT
    30, // NOV
    31, // DEC
};

static int calculate_abs_days(const unsigned short day, const month_t month,
                              const unsigned year) {
  unsigned sum = day;
  for (month_t i = 0; i < month; i++) {
    if (i == 1 && check_if_leap_year(year)) {
      sum++;
    }
    sum += month_lookup[i];
  }

  return sum;
}

static month_t calculate_month(const unsigned abs_days, const unsigned year) {
  size_t index = 0;
  int days_from_start_of_month_i = abs_days;
  while (abs_days > 28) {
    days_from_start_of_month_i -= month_lookup[index];

    // check if leapyear
    if (index == 1 && check_if_leap_year(year)) {
      days_from_start_of_month_i--;
    }

    if (days_from_start_of_month_i <= 0) {
      break;
    }

    index++;
  }
  return index;
}

static unsigned short calculate_day_of_month(const unsigned abs_days,
                                             const month_t month,
                                             const unsigned year) {
  int days_from_start_of_month_i = abs_days;

  for (size_t i = 0; i <= month; i++) {
    days_from_start_of_month_i -= month_lookup[i];

    // leapyear -> FEB 29 days
    if (i == 1 && check_if_leap_year(year)) {
      days_from_start_of_month_i--;
    }

    if (days_from_start_of_month_i <= 0) {
      days_from_start_of_month_i += month_lookup[i];

      // leapyear -> FEB 29 days
      if (i == 1 && check_if_leap_year(year)) {
        days_from_start_of_month_i++;
      }

      break;
    }
  }

  return days_from_start_of_month_i;
}

// TODO: Implement so it is not only possible to subtract but add to date
date_t create_date_from_offset(const date_t date_source,
                               const unsigned offset) {
  unsigned abs_days =
      calculate_abs_days(date_source.day, date_source.month, date_source.year);

  unsigned hours_offset = offset;
  unsigned days_offset = 0;
  unsigned years_offset = 0;

  unsigned buffer = 0;

  // translate hour offset to hour, day and year offset
  if (hours_offset >= 24) {
    buffer = hours_offset % 24;
    days_offset = (hours_offset - buffer) / 24;
    hours_offset = buffer;
    if (days_offset >= 365) {
      buffer = days_offset % 365;
      years_offset = (days_offset - buffer) / 365;
      days_offset = buffer;
    }
  }

  date_t new_date = {
      .minute = date_source.minute,
  };

  if ((int)(date_source.hour - hours_offset) < 0) {
    new_date.hour = 24 + (int)(date_source.hour - hours_offset);
    days_offset++;
  }

  // day offset goes over year edge
  if ((int)(abs_days - days_offset) <= 0) {
    abs_days = 365 - abs((int)(abs_days - days_offset));
    years_offset++;
  } else {
    abs_days -= days_offset;
  }

  new_date.year = date_source.year - years_offset;

  new_date.month = calculate_month(abs_days, new_date.year);
  new_date.day =
      calculate_day_of_month(abs_days, new_date.month, new_date.year);
  new_date.weekday =
      calculate_weekday(new_date.year, new_date.month, new_date.day);

  return new_date;
}

char *create_string_from_date(date_t date) {
  char *string = malloc(sizeof(char) * DATE_STRING_LENGTH + 1);

  char *weekday_str = get_weekday(date.weekday);
  if (!weekday_str) {
    fprintf(stderr, "ERROR: weekday isn't correct\n");
    return NULL;
  }

  snprintf(string, DATE_STRING_LENGTH, "%u-%02d-%02u: %02u:%02u; %s",
           date.year, date.month, date.day, date.hour, date.minute,
           weekday_str);

  return string;
}

weekday_t string_to_weekday(char *string) {
  if (strcmp(string, "Sunday") == 0) {
    return SUN;
  }
  if (strcmp(string, "Monday") == 0) {
    return MON;
  }
  if (strcmp(string, "Tuesday") == 0) {
    return TUE;
  }
  if (strcmp(string, "Wednesday") == 0) {
    return WED;
  }
  if (strcmp(string, "Thursday") == 0) {
    return THU;
  }
  if (strcmp(string, "Friday") == 0) {
    return FRI;
  }
  if (strcmp(string, "Saturday") == 0) {
    return SAT;
  }

  return MON;
}
