#ifndef _FILE_H
#define _FILE_H

#include <stdbool.h>

bool file_exist(const char* file_name);

bool file_create_dir(const char* dir_name);

bool file_create_file(const char* file_name);
bool file_delete_file(const char* file_name);
#endif
