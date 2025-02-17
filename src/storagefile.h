#pragma once

#include <stdio.h>

#define MAX_PATH 1024

FILE *open_storage_file(void);
int close_storage_file(FILE *storagefile_fd);

