#pragma once

#include <stdlib.h>

typedef enum month_e {
  JAN,
  FEB,
  MAR,
  APR,
  MAI,
  JUN,
  JUL,
  AUG,
  SEP,
  OKT,
  NOV,
  DEC,
} month_t;

typedef enum weekday_e {
  SUN,
  MON,
  TUE,
  WED,
  THU,
  FRI,
  SAT,
} weekday_t;

typedef struct date_s {
  unsigned year;
  month_t month;
  unsigned short day;
  unsigned short minute;
  unsigned short hour;
  weekday_t weekday;
} date_t;

weekday_t calculate_weekday(unsigned y, unsigned m, unsigned d);

date_t get_current_time_as_date_t(void);

int print_date(date_t date);
int print_alert(date_t date);
