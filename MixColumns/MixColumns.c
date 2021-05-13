#include <stdio.h>
#include <stdlib.h>

typedef unsigned char BYTE;

BYTE x_time(BYTE n, BYTE b);
void MixColumns(BYTE state[][4]);
void Inv_MixColumns(BYTE state[][4]);

BYTE x_time(BYTE n, BYTE b){ //다항식 꼴로 변환된 두 BYTE의 곱연산, n을 고정함, GF(2^8)체 안에서 다루기 때문에 8차항은 존재하지 않음(BYTE이기 때문에 7차까지만 있음)
    int i;
    BYTE temp = 0, mask = 0x01;
    printf("n:%x, b:%x\n",n,b);
    for(i=0;i<8;i++){
        if(n&mask){
             temp ^= b;
        }
        printf("\n%x",b);
        if(b&0x80){ //다항식에 x를 곱한 후 carry된 다항식의 modulo 연산 처리, 크기를 기약다항식(0x1B)의 최대차수보다 낮게만듦/ 처리하지 않으면 GF(2^8)체를 벗어나게 됨
            b = (b<<1)^0x1B;
        }
        else{
            b<<=1; //carry가 발생하지 않으면 다항식에 x를 곱함, shift연산과 동일, 차수가 1씩 증가하게 된다
        }

        mask<<=1;
    }
    printf("\n\n%3x",temp);
    return temp;                                                                                                    
}

void main(){
    BYTE n = 0x26, b = 0x9E;
    x_time(n,b);
}