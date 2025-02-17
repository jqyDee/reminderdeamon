#include <stdio.h>

#ifdef DEBUG
#define log(fmt, ...) printf("%s:%d: " fmt, __FILE__, __LINE__, __VA_ARGS__)
#else
#define log(...)
#endif
