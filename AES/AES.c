#include <stdio.h>
#include <stdlib.h>

typedef unsigned int WORD;
typedef unsigned char BYTE;

//Macro & Constant
#define HIHEX(x) (x>>4)
#define LOWHEX(x) (x&0x0F)
#define BTOW(b1, b2, b3, b4) (((WORD)b1<<24)|((WORD)b2<<16)|((WORD)b3<<8)|(WORD)b4)
#define Nr 10 //AES Round Number
#define Nb 4  //AES Block Size (4 WOrd)
#define Nk 4  //AES Key Size (4 Word)
static WORD Rcon[Nr+1] = {0x00000000, 0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000, 0x1B000000, 0x36000000};

//Lookup Table
BYTE sbox[16][16] =
{
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
    0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
    0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
    0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
    0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
    0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
    0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
    0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
    0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
    0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
    0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

BYTE isbox[16][16] =
{
    0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
    0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
    0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
    0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
    0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
    0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
    0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
    0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
    0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
    0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
    0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
    0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
    0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
    0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
    0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D
};


//Funcion
void SubByte(BYTE state[][4]);
void Inv_SubByte(BYTE state[][4]);
void InToState(BYTE* in, BYTE state[][4]);
void StateToOut(BYTE state[][4], BYTE* out);

void KeyExpansion(BYTE *key, WORD* w);
WORD RotWord(WORD w);
WORD SubWord(WORD w);

void ShiftRows(BYTE state[][4]);
void CirShiftRows(BYTE *row);
void Inv_ShiftRows(BYTE state[][4]);
void Inv_CirShiftRows(BYTE *row);

void AddRoundKey(BYTE state[][4], WORD *rkey);

BYTE x_time(BYTE n, BYTE b);
void MixColumns(BYTE state[][4]);
void Inv_MixColumns(BYTE state[][4]);

void printState(BYTE *state);

void AES_Encryption(BYTE *in, BYTE *out, BYTE *key);
void AES_Decryption(BYTE *in, BYTE *out, BYTE *key);

void SubByte(BYTE state[][4]){
    int i, k;
    BYTE x, y;
    for(i=0;i<4;i++){
        for(k=0;k<4;k++){
            x = HIHEX(state[i][k]);
            y = LOWHEX(state[i][k]);
            state[i][k] = sbox[x][y];
        }
    }
}
void Inv_SubByte(BYTE state[][4]){
    int i, k;
    BYTE x, y;
    for(i=0;i<4;i++){
        for(k=0;k<4;k++){
           x = HIHEX(state[i][k]);
           y = LOWHEX(state[i][k]);
           state[i][k] = isbox[x][y];
        }
    }
}
void InToState(BYTE* in, BYTE state[][4]){
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

void KeyExpansion(BYTE *key, WORD *w){  //128bit??? ?????? ????????? 44?????? WORD??? ??????
    int i;

    //Round 0
    w[0] = BTOW(key[0], key[1], key[2], key[3]);
    w[1] = BTOW(key[4], key[5], key[6], key[7]);
    w[2] = BTOW(key[8], key[9], key[10], key[11]);
    w[3] = BTOW(key[12], key[13], key[14], key[15]);

    //Round 1~10, My Method
    // for(i=1;i<=Nr;i++){
    //     w[i*Nb] = (SubWord(RotWord(w[i*Nb-1]))^Rcon[i])^w[(i-1)*Nb];
    //     w[i*Nb+1] = w[(i-1)*Nb+1]^w[i*Nb];
    //     w[i*Nb+2] = w[(i-1)*Nb+2]^w[i*Nb+1];
    //     w[i*Nb+3] = w[(i-1)*Nb+3]^w[i*Nb+2];
    // }

    //Round 1~10. Pf Method
    for(i=1;i<=Nr;i++){
        w[i*Nb] = (SubWord(RotWord(w[i*Nb-1]))^Rcon[i])^w[(i-1)*Nb];
        w[i*Nb+1] = w[i*Nb]^w[i*Nb-3];
        w[i*Nb+2] = w[i*Nb+1]^w[i*Nb-2];
        w[i*Nb+3] = w[i*Nb+2]^w[i*Nb-1];
    }
}

WORD RotWord(WORD w){
    return ((w>>24)|(w<<8));
}

WORD SubWord(WORD w){
    WORD t1 = 0, t2 = 0, t3 = 0, t4 = 0;
    WORD o1 = 0, o2 = 0, o3 = 0, o4 = 0;

    t1 = (w&0xFF000000)>>24;
    t2 = (w&0x00FF0000)>>16;
    t3 = (w&0x0000FF00)>>8;
    t4 = (w&0x000000FF);

    o1 = ((WORD)(sbox[HIHEX(t1)][LOWHEX(t1)]))<<24;
    o2 = ((WORD)(sbox[HIHEX(t2)][LOWHEX(t2)]))<<16;
    o3 = ((WORD)(sbox[HIHEX(t3)][LOWHEX(t3)]))<<8;
    o4 = ((WORD)(sbox[HIHEX(t4)][LOWHEX(t4)]));

    return (o1|o2|o3|o4);
}


void ShiftRows(BYTE state[][4]){
    int i, k;
    for(i=1;i<4;i++){ //1?????? Shift????????? ?????? ?????????
        for(k=0;k<i;k++){ //2?????? 1???, 3?????? 2???, 4?????? 3??? ???????????? ?????? ???????????? k<i??? ??????
            CirShiftRows(state[i]);
        }
    }
}

void CirShiftRows(BYTE *row){ //0~3, ??????????????? 0 1 2 3????????? ??????, ???????????? ??????
    BYTE temp = row[0];
    row[0] = row[1];
    row[1] = row[2];
    row[2] = row[3];
    row[3] = temp;
}

void Inv_ShiftRows(BYTE state[][4]){
    int i, k;
    for(i=1;i<4;i++){ //1?????? Shift????????? ?????? ?????????
        for(k=0;k<i;k++){ //2?????? 1???, 3?????? 2???, 4?????? 3??? ???????????? ?????? ???????????? k<i??? ??????
            Inv_CirShiftRows(state[i]);
        }
    }
}

void Inv_CirShiftRows(BYTE *row){ //0~3 ??????????????? 0 1 2 3????????? ??????, ??????????????? ??????
    BYTE temp = row[3];
    row[3] = row[2];
    row[2] = row[1];
    row[1] = row[0];
    row[0] = temp;
}

void AddRoundKey(BYTE state[][4], WORD *rkey){ //state??? 2?????? ??????, rkey??? 1?????? ??????
    int i, k;
    WORD mask, shift;

    for(i=0;i<4;i++){
        mask = 0xFF000000;
        shift = 24;

        for(k=0;k<4;k++){
            state[k][i] ^= (rkey[i]&mask)>>shift; //AddRound?????? state??? ???????????? xor???????????? ????????? state[k][i]????????? ??????
            shift -= 8;
            mask>>=8;
        }
    }
}

BYTE x_time(BYTE n, BYTE b){ //????????? ?????? ????????? ??? BYTE??? ?????????, n??? ?????????, GF(2^8)??? ????????? ????????? ????????? 8????????? ???????????? ??????(BYTE?????? ????????? 7???????????? ??????) ?????? ?????? ?????? X
    int i;
    BYTE temp = 0, mask = 0x01;
    for(i=0;i<8;i++){
        if(n&mask){
            temp ^= b;
        }
        if(b&0x80){ //???????????? x??? ?????? ??? carry??? ???????????? modulo ?????? ??????, ????????? ???????????????(0x1B)??? ?????????????????? ????????????/ ???????????? ????????? GF(2^8)?????? ???????????? ???
            b = (b<<1)^0x1B;
        }
        else{
            b<<=1; //carry??? ???????????? ????????? ???????????? x??? ??????, shift????????? ??????, ????????? 1??? ???????????? ??????
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
                b[i][k] ^= x_time(a[i][m], state[m][k]); //????????? ?????? ???????????? xor????????? ?????? ????????? ?????? ??????
            }
        }
    }

    for(i=0;i<4;i++){
        for(k=0;k<4;k++){
            state[i][k] = b[i][k]; //b(temp)??? ????????? ?????? state??? ??????
        }
    }
}

void Inv_MixColumns(BYTE state[][4]){
    int i, k, m;
    BYTE a[4][4] = { //MixColumns??? ????????? ?????????
        0x0E, 0x0B, 0x0D, 0x09,
        0x09, 0x0E, 0x0B, 0x0D,
        0x0D, 0x09, 0x0E, 0x0B,
        0x0B, 0x0D, 0x09, 0x0E
    };
    BYTE b[4][4] = {0,};

    for(i=0;i<4;i++){
        for(k=0;k<4;k++){
            for(m=0;m<4;m++){
                b[i][k] ^= x_time(a[i][m], state[m][k]); //????????? ?????? ???????????? xor????????? ?????? ????????? ?????? ??????
            }
        }
    }

    for(i=0;i<4;i++){
        for(k=0;k<4;k++){
            state[i][k] = b[i][k]; //b(temp)??? ????????? ?????? state??? ??????
        }
    }
}

void printState(BYTE *state){
    int i;

    for(i=0;i<(Nb*4);i++){
        if(i%4==0) printf("\n");
        printf(" %02X", state[i]);
    }
    printf("\n");
}

void AES_Encryption(BYTE *in, BYTE *out, BYTE *key){
    int i = 0;
    BYTE state[4][4] = {0,};
    WORD w[Nb*(Nr+1)] = {0,};

    InToState(in, state);

    KeyExpansion(key, w);

    AddRoundKey(state, w);

    for(i=1;i<Nr;i++){ //1~9 Round, 10Round??? MixColumns????????? ?????? ?????? ????????? ??????/for??? ????????? ????????? ????????? ??? ??? ??????
        SubByte(state);
        ShiftRows(state);
        MixColumns(state);
        AddRoundKey(state, w+(i*Nb));
    }

    //10Round
    SubByte(state);
    ShiftRows(state);
    AddRoundKey(state, w+(Nr*Nb));

    StateToOut(state, out);
}

void AES_Decryption(BYTE *in, BYTE *out, BYTE *key){
    int i = 0;
    BYTE state[4][4] = {0,};
    WORD w[Nb*(Nr+1)] = {0,};

    InToState(in, state);

    KeyExpansion(key, w);

    AddRoundKey(state, w+(Nr*Nb));

    for(i=1;i<Nr;i++){
        Inv_ShiftRows(state);
        Inv_SubByte(state);
        AddRoundKey(state, w+((Nr-i)*Nb));
        Inv_MixColumns(state);
    }

    Inv_ShiftRows(state);
    Inv_SubByte(state);
    AddRoundKey(state, w);

    StateToOut(state, out);
}

//Main
void main(){
    BYTE in[Nb*4] = {
        0x12, 0x13, 0x14, 0x15,
        0x21, 0x22, 0x23, 0x21,
        0x31, 0x32, 0x33, 0x34,
        0xA3, 0xB1, 0xC1, 0XD2
    };
    BYTE out[Nb*4] = {0,};
    BYTE inv_in[Nb*4] = {0,};
    BYTE key[Nb*4] = {
        0xA2, 0xB3, 0xC4, 0xD5,
        0x2E, 0x2F, 0x23, 0x21,
        0x31, 0x32, 0x33, 0x34,
        0xA3, 0xB1, 0xC1, 0xD2
    };

    printf("\nPlainText:");
    printState(in);

    AES_Encryption(in, out, key);

    printf("\nEncryptionText:");
    printState(out);

    AES_Decryption(out, inv_in, key);

    printf("\nDecryptionText:");
    printState(inv_in);
}