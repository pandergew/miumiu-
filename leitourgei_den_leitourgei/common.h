#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <semaphore.h>

#define MAX_TEXT 250
#define MAX_CHECKSUM 8 
// message shared memory 
#define SHM_NAME "/msgshm"

enum message_type { DATA, ACK, NACK, TERM };

struct message {
    enum message_type type;
    char text [MAX_TEXT + 1];
    char checksum [MAX_CHECKSUM + 1];
};

struct buffer {
    struct message msg;
    sem_t empty;
    sem_t full;
};

struct shared {
    struct buffer p1_to_enc1;
    struct buffer enc1_to_p1;
    struct buffer enc1_to_chan;
    struct buffer chan_to_enc1;
    struct buffer chan_to_enc2;
    struct buffer enc2_to_chan;
    struct buffer enc2_to_p2;
    struct buffer p2_to_enc2;
};

#endif