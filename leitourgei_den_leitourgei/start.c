#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//shared memory
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <sys/wait.h>
#include <time.h>

#include "common.h"

//hash
//ftiaxnontas thn sunarthsh pou pairnei to keimeno kai deinei 32-bit arithmo
unsigned int hash(const char *s){
    unsigned int h = 5381; /*djb2:arxikh timh 5381 (einai h arxikh timh(seed) tou hash (thn epeleje o daniel j bernstein) den jekina apo to 0 giati ta keimena pou jekinanai me 0  den tha ephreazan to hash kai ena kaino keimeno tha edine 0 )*/
    while (*s){
        //to hash trwei to keimeno xarakthra-xarakthra kai kathe bhma anakateuei ton xarakthra me oti uparxei hdh mexri tote
        h = h * 33 + (unsigned char)*s; /*h*33 poll/zei to prohgoumeno apotel kai auto sproxnei ta prohgoumena stoixeia wsteh h thesh tou xarakthra na exei shmaseia(bazoume 33 giati 32+1 ginetai grhgora)
        + (unsigned char)*s : prosthetei thn timh tou trexonta xarakthra. to *s einai o xarakthras stonopoio deixnei o deikths s (xazh shmeiwsh to unsigned char ejasfalizei oti oloi oi xarakthres einai 0-255) */
        s++;
    }
    return h;
}

//corrupt
//testing 
void corrupt(char *s, double p){
    for(int i = 0; s[i] != '\0'; i++){
        double r = (double)rand() / RAND_MAX; /*tuxaios arithmos 0-1*/

        if(r<p) s[i] = 'a' + rand() % 26; /*tuxaio gramma*/ 
    }
}


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


    //to pid_t used for storing process ids leei sto man.archlinux.org
    pid_t pid = fork(); /*creating a new process (called child process) skecou to san dentro tha pw kai h phgh apo thn opoia to peira einai to geeks*/
    if(pid == -1){
        perror("fork fail (pid)");
        return 1;
    }
    if(pid == 0){
        //enc1: diabazei apo thn p1
        sem_wait(&sh->p1_to_enc1.full); /*koimatai mexri na uparjei mhnuma*/
        
        //testing
        //char cs[MAX_CHECKSUM + 1];
        //sprintf(cs, "%08x", hash(sh->p1_to_enc1.msg.text)); /* grafei ton arithmo se hex me 8 chfia (me mhdenika mprosta) ara xwraei akribws sta max-checksum+1 byte*/
        //printf("ENC1 received: %s (checksum %s)\n", sh->p1_to_enc1.msg.text, cs);
        // telos testing

        struct message m = sh->p1_to_enc1.msg; /*antigraf to mhnuma */        
        sem_post(&sh->p1_to_enc1.empty); /*o buffer ths p1 eleutherwthhke*/

        sprintf(m.checksum, "%08x", hash(m.text)); /*prosthetei checksum*/

        /*grafei pros chan*/
        sem_wait(&sh->enc1_to_chan.empty);
        sh->enc1_to_chan.msg = m; 
        sem_post(&sh->enc1_to_chan.full);

        return 0;
    }

    //prepei to 3o paidi prin ton gonea (gitai o goneas eipame exei wait)
    pid_t pid3 = fork();
    if(pid3 == -1){
        perror("fail fork (pid3)");
        return 1;
    }
    if(pid3 == 0){
        //pali to idio me tis alles duo fores + kai to testing 
        sem_wait(&sh->chan_to_enc2.full);
        struct message m = sh->chan_to_enc2.msg;
        sem_post(&sh->chan_to_enc2.empty);

        char cs[MAX_CHECKSUM + 1];
        sprintf(cs, "%08x", hash(m.text));

        if(strcmp(cs, m.checksum) == 0){
            printf("ENC2: ok, message accepted: %s\n", m.text);
            
            sem_wait(&sh->chan_to_enc2.empty);
            sh->enc2_to_p2.msg = m;
            sem_post(&sh->enc2_to_p2.full);
        }
        else{
            printf("ENC2: checksum mismatch would send NACK\n");
        }
        return 0;
    }


    //apo edw kai katw sunexizei o gonas

    pid_t pid2 = fork();
    if(pid2 == -1){
        perror("fail fork gonea (pid2)");
        return 1;
    }
    if(pid2 == 0 ){
        srand(time(NULL) ^ getpid()); /*diaforetikoi tuxaioi arithmoi */

        //ta idia me apo panw den ta jana ejhgw sugnwmh 
        sem_wait(&sh->enc1_to_chan.full);
        struct message m  = sh->enc1_to_chan.msg;
        sem_post(&sh->enc1_to_chan.empty);

        corrupt(m.text, p);
        corrupt(m.checksum, p);

        sem_wait(&sh->chan_to_enc2.empty);
        sh->chan_to_enc2.msg = m;
        sem_post(&sh->chan_to_enc2.full);
        return 0;

    }


    wait(NULL); /*trith fora*/
    wait(NULL); /*deuterh fora*/
    wait(NULL); /*mexri na teleiwsei to paidi*/
    printf("Parent: child finished\n");

    //printf("in enc1-to-chan: %s | %s\n", sh->enc1_to_chan.msg.text, sh->enc1_to_chan.msg.checksum);    
    printf("in chan-to-enc2: %s | %s\n", sh->chan_to_enc2.msg.text, sh->chan_to_enc2.msg.checksum);

    return 0;
}