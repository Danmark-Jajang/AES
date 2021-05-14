#include <stdio.h>
#include <stdlib.h>

typedef unsigned char BYTE;

BYTE x_time(BYTE n, BYTE b);
void MixColumns(BYTE state[][4]);
void Inv_MixColumns(BYTE state[][4]);
void printState(BYTE state[][4]);

BYTE x_time(BYTE n, BYTE b){ //다항식 꼴로 변환된 두 BYTE의 곱연산, n을 고정함, GF(2^8)체 안에서 다루기 때문에 8차항은 존재하지 않음(BYTE이기 때문에 7차까지만 있음) 둘의 순서 상관 X
    int i;
    BYTE temp = 0, mask = 0x01;
    for(i=0;i<8;i++){
        if(n&mask){
             temp ^= b;
        }
        if(b&0x80){ //다항식에 x를 곱한 후 carry된 다항식의 modulo 연산 처리, 크기를 기약다항식(0x1B)의 최대차수보다 낮게만듦/ 처리하지 않으면 GF(2^8)체를 벗어나게 됨
            b = (b<<1)^0x1B;
        }
        else{
            b<<=1; //carry가 발생하지 않으면 다항식에 x를 곱함, shift연산과 동일, 차수가 1씩 증가하게 된다
        }

        mask<<=1;
    }
    return temp;                                                                                                    
}

void MixColumns(BYTE state[][4]){
    int i, k, m;
    BYTE a[4][4] = {
        0x02, 0x03, 0x01, 0x01,
        0x01, 0x02, 0x03, 0x01,
        0x01, 0x01, 0x02, 0x03,
        0x03, 0x01, 0x01, 0x02
    };
    BYTE b[4][4] = {0,};

    for(i=0;i<4;i++){
        for(k=0;k<4;k++){
            for(m=0;m<4;m++){
                b[i][k] ^= x_time(a[i][m], state[m][k]); //결과로 나온 다항식과 xor연산을 하여 행렬의 곱셈 수행
            }
        }
    }

    for(i=0;i<4;i++){
        for(k=0;k<4;k++){
            state[i][k] = b[i][k]; //b(temp)에 저장된 값을 state로 복사
        }
    }
}

void Inv_MixColumns(BYTE state[][4]){
    int i, k, m;
    BYTE a[4][4] = { //MixColumns의 행렬의 역행렬
        0x0E, 0x0B, 0x0D, 0x09,
        0x09, 0x0E, 0x0B, 0x0D,
        0x0D, 0x09, 0x0E, 0x0B,
        0x0B, 0x0D, 0x09, 0x0E
    };
    BYTE b[4][4] = {0,};

    for(i=0;i<4;i++){
        for(k=0;k<4;k++){
            for(m=0;m<4;m++){
                b[i][k] ^= x_time(a[i][m], state[m][k]); //결과로 나온 다항식과 xor연산을 하여 행렬의 곱셈 수행
            }
        }
    }

    for(i=0;i<4;i++){
        for(k=0;k<4;k++){
            state[i][k] = b[i][k]; //b(temp)에 저장된 값을 state로 복사
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
        0xDD, 0xEE, 0xFF, 0x21
    };

    printf("\nState:");
    printState(state);

    MixColumns(state);
    printf("\nMixed State:");
    printState(state);

    Inv_MixColumns(state);
    printf("\nInverse Mixed State:");
    printState(state);
}