#include <stdio.h>
#include <stdlib.h>

typedef unsigned char BYTE;
typedef unsigned int WORD;

void ShiftRows(BYTE state[][4]);
void CirShiftRows(BYTE *row);
void Inv_ShiftRows(BYTE state[][4]);
void Inv_CirShiftRows(BYTE *row);
void printState(BYTE state[][4]);

void ShiftRows(BYTE state[][4]){
    int i, k;
    for(i=1;i<4;i++){ //1행은 Shift연산을 하지 않는다
        for(k=0;k<i;k++){ //2행은 1번, 3행은 2번, 4행은 3번 연산하기 위해 종료문을 k<i로 설정
            CirShiftRows(state[i]);
        }
    }
}

void CirShiftRows(BYTE *row){ //0~3, 배열이므로 0 1 2 3순서로 배치, 왼쪽으로 이동
    BYTE temp = row[0];
    row[0] = row[1];
    row[1] = row[2];
    row[2] = row[3];
    row[3] = temp;
}

void Inv_ShiftRows(BYTE state[][4]){
    int i, k;
    for(i=1;i<4;i++){ //1행은 Shift연산을 하지 않는다
        for(k=0;k<i;k++){ //2행은 1번, 3행은 2번, 4행은 3번 연산하기 위해 종료문을 k<i로 설정
            Inv_CirShiftRows(state[i]);
        }
    }
}

void Inv_CirShiftRows(BYTE *row){ //0~3 배열이므로 0 1 2 3순서로 배치, 오른쪽으로 이동
    BYTE temp = row[3];
    row[3] = row[2];
    row[2] = row[1];
    row[1] = row[0];
    row[0] = temp;
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

    printf("State: ");
    printState(state);

    ShiftRows(state);
    printf("After ShiftRows:");
    printState(state);

    Inv_ShiftRows(state);
    printf("After Inv_ShiftRows:");
    printState(state);
}