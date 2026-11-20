#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <fcntl.h>
#include <unistd.h>
//#include <sys/mman.h>
//#include <sys/stat.h>

#include "common.h"

int main(int argc, char *argv[]){
    if(argc != 2){
        fprintf(stderr, "Usage: %s <1|2>\n", argv[0]);
        return 1;
    }

    int id = atoi(argv[1]);
    if(id != 1 && id != 2){
        fprintf(stderr, "Usage: %s <1|2>\n", argv[0]);
        return 1;
    }

    printf("I am P%d\n", id);
    return 0;
}



