#include <stdio.h>
#include <stdbool.h>

#include "file.h"

const char* sts_dir = "/tmp/sts/";
const char* config_sock = "/tmp/sts/config_sock";

int main(int argc, char* argv[]){

    if(!file_exist(sts_dir)){
        fprintf(stdout, "STS dir %s does not exist. Creating...\n", sts_dir);
        if(!file_create_dir(sts_dir)){
            fprintf(stderr, "Failed to create STS dir %s\n", sts_dir);
            return 1;
        }
        fprintf(stdout, "Create STS dir %s\n", sts_dir);
    }else{
        fprintf(stdout, "STS dir %s exists\n", sts_dir);
    }

    if(!file_exist(config_sock)){
        fprintf(stdout, "Config Socket %s does not exist. Creating...\n", config_sock);
        if(!file_create_file(config_sock)){
            fprintf(stderr, "Failed to create Config Socket %s\n", config_sock);
            return 1;
        }
        fprintf(stdout, "Create Config Socket %s\n", config_sock);
    }else{
        fprintf(stdout, "Config Socket %s exists\n", config_sock);
    }

    return 0;
}
