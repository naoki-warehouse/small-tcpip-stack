#include <stdio.h>
#include <stdbool.h>

#include "file.h"

const char* sts_dir = "/tmp/sts/";
const char* sock = "/tmp/sts/sock";

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

    if(file_exist(sock)){
        fprintf(stdout, "Socket %s exists. Deleting...\n", sock);
        if(file_delete_file(sock)){
            fprintf(stdout, "Delete Socket %s\n", sock);
        }else{
            fprintf(stderr, "Fialed to delete Socket %s\n", sock);
            return 1;
        }
    }

    return 0;
}
