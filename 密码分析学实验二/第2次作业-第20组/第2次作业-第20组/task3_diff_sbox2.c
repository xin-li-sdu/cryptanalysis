#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* The S-Box tables */
static char S[8][64] = {{
    /* S1 */
    14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,  
     0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,  
     4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0, 
    15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13
},{
    /* S2 */
    15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,  
     3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,  
     0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15, 
    13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9
},{
    /* S3 */
    10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,  
    13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,  
    13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
     1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12
},{
    /* S4 */
     7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,  
    13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,  
    10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
     3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14
},{
    /* S5 */
     2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9, 
    14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6, 
     4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14, 
    11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3
},{
    /* S6 */
    12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
    10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
     9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
     4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13
},{
    /* S7 */
     4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
    13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
     1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
     6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12
},{
    /* S8 */
    13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
     1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
     7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
     2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
}};

/* Post S-Box permutation */
static char P[] = {
    16,  7, 20, 21, 
    29, 12, 28, 17, 
     1, 15, 23, 26, 
     5, 18, 31, 10, 
     2,  8, 24, 14, 
    32, 27,  3,  9, 
    19, 13, 30,  6, 
    22, 11,  4, 25
};

/*Expansion table */
static char E[] = {
    32,  1,  2,  3,  4,  5,  
     4,  5,  6,  7,  8,  9,  
     8,  9, 10, 11, 12, 13, 
    12, 13, 14, 15, 16, 17, 
    16, 17, 18, 19, 20, 21, 
    20, 21, 22, 23, 24, 25, 
    24, 25, 26, 27, 28, 29, 
    28, 29, 30, 31, 32,  1
};

#define LB32_MASK   0x00000001
#define LB64_MASK   0x0000000000000001
#define L64_MASK    0x00000000ffffffff
#define H64_MASK    0xffffffff00000000


uint64_t E_ex(uint32_t R){
    uint64_t s_input = 0;
        
    for (int j = 0; j< 48; j++) {
        s_input <<= 1;
        s_input |= (uint64_t) ((R >> (32-E[j])) & LB32_MASK);
            
    }
    return s_input;
}

uint32_t get_mask(int num){
    /* 8 bits */
    char row = 0, column = 0;
    uint32_t s_output = 0;
    uint32_t f_function_res = 0;
    
    s_output = (uint32_t)((uint32_t)0b1111 <<(uint32_t)((7-num)*4)); 

    f_function_res = 0;
        
    for (int j = 0; j < 32; j++) {
            
        f_function_res <<= 1;
        f_function_res |= (s_output >> (32 - P[j])) & LB32_MASK;
            
    }
    // printf("%lx\n",f_function_res);
    return f_function_res;
}

uint32_t rr(uint64_t s_input, uint64_t sub_key, int num){
    /* 8 bits */
    char row = 0, column = 0;
    uint32_t s_output = 0;
    uint32_t f_function_res = 0;
    
    s_input = s_input ^ sub_key;
    for (int j = num; j < num+1; j++) {
    // 00 00 RCCC CR00 00 00 00 00 00 s_input
    // 00 00 1000 0100 00 00 00 00 00 row mask
    // 00 00 0111 1000 00 00 00 00 00 column mask
            
    row = (char) ((s_input & (0x0000840000000000 >> 6*j)) >> 42-6*j);
    row = (row >> 4) | row & 0x01;
            
    column = (char) ((s_input & (0x0000780000000000 >> 6*j)) >> 43-6*j);
            
    s_output |= (uint32_t) (S[j][16*row + column] & 0x0f);
    s_output <<= 4*(7-num);
            
    }
    // printf("%llx,%llx,%llx\n",row,column,s_output);
    f_function_res = 0;
        
    for (int j = 0; j < 32; j++) {
            
        f_function_res <<= 1;
        f_function_res |= (s_output >> (32 - P[j])) & LB32_MASK;
            
    }
    // printf("%lx\n",f_function_res);
    return f_function_res;
}

uint32_t F[300],f0[300],f1[300];
uint32_t c = 0x04000000;

int get_cnt(int num, uint16_t key){
    uint32_t mask = get_mask(num);
    // printf("%lx\n",mask);
    int cnt = 0;
    for(int i = 0; i < 300; i++){
        uint64_t f_0 = E_ex(f0[i]) & (uint64_t)((uint64_t)0b111111<<((7-num)*6));
        uint64_t f_1 = E_ex(f1[i]) & (uint64_t)((uint64_t)0b111111<<((7-num)*6));
        uint64_t sub_key = (uint64_t)((uint64_t)key<<(7-num)*6);
        // printf("%llx %llx %llx\n",f_0,f_1,sub_key);
        uint32_t F_0 = rr(f_0, sub_key, num);
        uint32_t F_1 = rr(f_1, sub_key, num);
        uint32_t F_x = F_0^F_1;
        // printf("%lx %lx %d\n",F[i]&F_x, F_x, F[i]&F_x==F_x);
        // printf("%lx\n",(F_x&mask) == F_x)
        if((F[i]&mask) == F_x){
            // printf("ok\n");
            cnt++;
        }
    }
    return cnt;
}

int main() {
    freopen("1.txt","r",stdin);
    uint32_t F0,F1;
    for(int i = 0; i < 300; i++){
        scanf("%lx %lx",&F0,&f0[i]);
        scanf("%lx %lx",&F1,&f1[i]);
        F[i] = F0 ^ F1 ^ c;
        // printf("%lx %lx %lx\n",f0[i],f1[i],F[i]);
    }
    int t[5] = {1,4,5,6,7},num = 0;
    for(int i = 0; i < 5; i++){
        num = t[i];
        int maxn = 0,key;
        for(uint16_t j = 0; j < 64; j++){
            int cnt = get_cnt(num,j);
            if(maxn < cnt){
                maxn = cnt;
                key = j;
            }
            // printf("%lx, %d\n",j,cnt);
        }
        printf("%lx, %d\n",key,maxn);
    }
    return 0;
}