#include <stdio.h>
#include <stdlib.h>

int gcd(int a, int b);

int gcd(int a, int b){
    if(b==0) return a;
    else return gcd(b, a%b);
}

void main(){
    int res = gcd(26, 11);
    printf("gcd: %3d", res);
}