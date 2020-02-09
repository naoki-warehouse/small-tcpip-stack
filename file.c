#include <sys/stat.h>
#include <stdio.h>
#include "file.h"

bool file_exist(const char* file_name){
    struct stat st;
    if(stat(file_name, &st) == 0){
        return true;
    }
    return false;
}

bool file_create_dir(const char* dir_name){
    if(mkdir(dir_name, 0775) == 0){
        return true;
    }
    return false;
}

bool file_create_file(const char* file_name){
    FILE *fp;
    fp = fopen(file_name, "w");
    if(fp == NULL){
        return false;
    }
    fclose(fp);
    return true;
}


bool file_delete_file(const char* file_name){
    if(remove(file_name) == 0){
        return true;
    }
    return false;
}

