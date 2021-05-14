#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int WORD;
typedef unsigned char BYTE;

void InToState(BYTE* in, BYTE (*state)[4]);
void StateToOut(BYTE state[][4], BYTE* out);
void printState(BYTE state[][4]);

void InToState(BYTE* in, BYTE (*state)[4]){
    int i, k;
    for(i=0;i<4;i++){
        for(k=0;k<4;k++){
            state[i][k] = in[i*4+k];
        }
    }
    
}

void StateToOut(BYTE state[][4], BYTE* out){
    int i, k;
    for(i=0;i<4;i++){
        for(k=0;k<4;k++){
            out[i*4+k] = state[i][k];
        }
    }
   
}

void printState(BYTE state[][4]){
    int i, k;
    for(i=0;i<4;i++){
        printf("\n");
        for(k=0;k<4;k++){
            printf("%3x",state[i][k]);
        }
    }
    printf("\n");
}

void main(){
    int i, k;
    BYTE state[4][4];
    BYTE in[100] = {0,};
    BYTE out[100] = {0,};

    printf("\nWrite Input String>>");
    gets(in);

    printf("\n\nInput String:\t");
    for(i=0;i<16;i++){
        printf("%3x",in[i]);
    }

    InToState(in, state);

    printf("\n\nState:");
    printState(state);

    StateToOut(state, out);
    printf("\n\nOut String:\t");
    for(i=0;i<16;i++){
        printf("%3x", out[i]);
    }
}