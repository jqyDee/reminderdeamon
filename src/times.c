#include "times.h"

#include <time.h>

#include "log.h"

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

  log("current time: %s\n", asctime(timeinfo));

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
  return (weekday_t)((d += m < 3 ? y-- : y - 2, 23*m/9 + d + 4 + y/4- y/100 + y/400)%7);
}

int print_alert(date_t date) {
  char *weekday_str = get_weekday(date.weekday);
  if (!weekday_str) {
    fprintf(stderr, "ERROR: weekday isn't correct\n");
    return 1;
  }

  printf("[%u-%02d-%02u: %02u:%02u; %s]", date.year, date.month, date.day,
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
  printf("[%u-%d-%u: %02u:%02u; %s]\n", date.year, date.month, date.day,
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
