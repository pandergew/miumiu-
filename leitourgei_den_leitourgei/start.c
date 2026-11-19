#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//shared memory
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "common.h"


int main(int argc, char *argv[]) {
    //pithanothta alloiwshs
    double p = -1.0; /* arxizei apo ena giati an o xrhsths jexasei ena orisma telikos elegxos to pianei*/
    // megistos arithmos prospatheiwn 
    int n = -1; /*den exei dwthei epithdes/ den exei dothei akoma*/

    //elegxos oti exoume tis swstes 5 times gia na trejei to programma
    if( argc != 5) {
        fprintf(stderr, "Usage: %s -p <probability> -n <attemps>\n", argv[0]);
        return 1;
    }

    //o broxos pou tha diabazei ta zeugaria
    for(int i = 1; i < argc; i+=2){
        if(strcmp(argv[i], "-p") == 0)
            p = atof(argv[i + 1]);
        else if(strcmp( argv[i], "-n") == 0)
            n = atoi(argv[i + 1]);
        else{
            fprintf(stderr, "Usage: %s -p <probability> -n <attempts>\n", argv[0]);
            return  1;
        }
    }

    //last before succes??
    if (p < 0.0 || p > 1.0 || n < 1){
        fprintf(stderr, "Usage: %s -p <probability> -n <attempts>\n", argv[0]);
        return  1;
    }

    //printf("Hello, argc = %d\n", argc );
    //printf("p = %.2f, n = %d\n", p, n);

    //apo edw kai katw prospathises na kaneis shared memory na jereis mporei na exei kapou lathos 
    //xrhsimopioume ta onomata o-creat kai o-rdwr giati οι αριθμοι αλλάζυν ανα σύστημα 
    //epishs to fd einai arithmos pou leitourgei san xerouli gia na mporoume na anaferomaste se auto stis epomenes kleiseis 
    //akoma to 0666 einai ta dikaiwmata pou tha exei h mnhmh otan dhmiourghthei(oloi mporoun na grafoun kai na diabazoun)
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if(fd == -1){
        perror("shm_open fail");
        return 1;
    }

    //arketa sigourh edw tha exei lathos giati to ftrucate tha kocei to fd analoga me to shared pou den uparxei akoma (uparxei mono se DHLWSH struct)
    //ftruncate kanei arxeio mnhmhs pou deixnei to fd na exei to megethos tou shared (to megalwnei/mikrenei analoga)
    if(ftruncate(fd, sizeof(struct shared)) == -1){
        perror("ftruncate fail");
        return 1;
    }

    //shmantikh shmeiwsh to o_rdwr orizei pws anoigeis to arxeio enw ta prot_ orizoun xartografhsh kai gia auto prepei na sumfwnoun
    //xrhsh map_shared giati theloume na grafontai oi allages sthn koinh mnhmh kai na tis blepoume (den theloume se idiotiko xwro)
    struct shared *sh = mmap(NULL, sizeof(struct shared), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(sh == MAP_FAILED){
        perror("mmap fail");
        return 1;
    }

    printf("Shared memory ready (p = %.2f, n = %d)\n", p, n);


    //arxikopoihsh shmatoforwn
    //exoume 8 deiktes ena gia kathe buffer wste meta na mporoume na tous arxikopoihsoume me ena broxo
    struct buffer *all[8] = {
        &sh->p1_to_enc1,
        &sh->enc1_to_p1,
        &sh->enc1_to_chan,
        &sh->chan_to_enc1,
        &sh->chan_to_enc2,
        &sh->enc2_to_chan,
        &sh->enc2_to_p2,
        &sh->p2_to_enc2
    };

    for(int i = 0; i<8; i++) {
        sem_init(&all[i]->empty, 1, 1);
        sem_init(&all[i]->full, 1, 0);
    }
    //parathrhsh apo otan balame auto to kommati exoume total 4 kai oxi 0 opws prin 
    //kai auto mas deixnei oti leitourghse kanonika to programma (auto ginetai logo tou sem_init 
    //pou egrace tis arxikes times twn shmaforwn mesa sthn mnhmh ) 
    //to megethos (2624) den allaje pou epishs deixnei oti douleuei swsta to programma afou exoume orisei to megethos me to ftrucate 

    return 0;
}