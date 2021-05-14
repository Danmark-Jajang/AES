#include <stdio.h>
#include <stdlib.h>

typedef unsigned char BYTE;
typedef unsigned int WORD;

void AddRoundKey(BYTE state[][4], WORD *rkey);
void printState(BYTE state[][4]);

void AddRoundKey(BYTE state[][4], WORD *rkey){ //state는 2차원 배열, rkey는 1차원 배열
    int i, k;
    WORD mask, shift;

    for(i=0;i<4;i++){
        mask = 0xFF000000;
        shift = 24;

        for(k=0;k<4;k++){
            state[k][i] ^= (rkey[i]&mask)>>shift; //AddRound에서 state는 열단위로 xor연산하기 때문에 state[k][i]순으로 연결
            shift -= 8;
            mask>>=8;
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
    BYTE state[4][4] = {
        0x11, 0x22, 0x33, 0x44,
        0x55, 0x66, 0x77, 0x88,
        0x99, 0xAA, 0xBB, 0xCC,
        0xDD, 0xEE, 0xFF, 0x07
    };

    WORD rkey[4] = {0x12345678, 0x13579ABC, 0x20011107, 0xC0FFEE11};

    printf("\nstate: ");
    printState(state);

    AddRoundKey(state, rkey);
    printf("\nAfter AddRoundKey:");
    printState(state);
}