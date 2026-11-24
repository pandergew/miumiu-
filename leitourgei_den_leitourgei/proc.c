#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/mman.h>
#include <sys/stat.h>

#include <sys/wait.h>

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

    int fd = shm_open(SHM_NAME, O_RDWR, 0);
    if (fd == -1) {
        perror("shm_open fail");
        return 1;
    }

    struct shared *sh = mmap(NULL, sizeof(struct shared), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (sh == MAP_FAILED) {
        perror("mmap fail");
        return 1;
    }

    //printf("I am P%d\n", id);
    printf("proc reads: %s\n", sh->p1_to_enc1.msg.text);

    //used for storing process ids leei sto man.archlinux.org
    pid_t pid = fork();
    if (pid == -1){
        perror("fork fail");
        return 1;
    }

    if (pid == 0){
        /* edw trwxei to paidi (tha ginei h enc1)*/
        printf("I am ENC1, child process\n");
        return 0;
    }

    //apo edw sunexizei o goneas
    wait(NULL); /*mexri na teleiwsei to paidi*/
    printf("Parent: child finished\n");

    

    return 0;
}



