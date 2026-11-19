#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"


int main(int argc, char *argv[]) {
    //pithanothta alloiwshs
    double p = -1.0; 
    // megistos arithmos prospatheiwn 
    int n = -1; 

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


    return 0;
}