#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* Initial Permutation Table */
static char IP[] = {
    58, 50, 42, 34, 26, 18, 10,  2, 
    60, 52, 44, 36, 28, 20, 12,  4, 
    62, 54, 46, 38, 30, 22, 14,  6, 
    64, 56, 48, 40, 32, 24, 16,  8, 
    57, 49, 41, 33, 25, 17,  9,  1, 
    59, 51, 43, 35, 27, 19, 11,  3, 
    61, 53, 45, 37, 29, 21, 13,  5, 
    63, 55, 47, 39, 31, 23, 15,  7
};

/* Inverse Initial Permutation Table */
static char PI[] = {
    40,  8, 48, 16, 56, 24, 64, 32, 
    39,  7, 47, 15, 55, 23, 63, 31, 
    38,  6, 46, 14, 54, 22, 62, 30, 
    37,  5, 45, 13, 53, 21, 61, 29, 
    36,  4, 44, 12, 52, 20, 60, 28, 
    35,  3, 43, 11, 51, 19, 59, 27, 
    34,  2, 42, 10, 50, 18, 58, 26, 
    33,  1, 41,  9, 49, 17, 57, 25
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

/* Permuted Choice 1 Table */
static char PC1[] = {
    57, 49, 41, 33, 25, 17,  9,
     1, 58, 50, 42, 34, 26, 18,
    10,  2, 59, 51, 43, 35, 27,
    19, 11,  3, 60, 52, 44, 36,
    
    63, 55, 47, 39, 31, 23, 15,
     7, 62, 54, 46, 38, 30, 22,
    14,  6, 61, 53, 45, 37, 29,
    21, 13,  5, 28, 20, 12,  4
};

/* Permuted Choice 2 Table */
static char PC2[] = {
    14, 17, 11, 24,  1,  5,
     3, 28, 15,  6, 21, 10,
    23, 19, 12,  4, 26,  8,
    16,  7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

/* Iteration Shift Array */
static char iteration_shift[] = {
 /* 1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16 */
    1,  1,  2,  2,  2,  2,  2,  2,  1,  2,  2,  2,  2,  2,  2,  1
};

#define LB32_MASK   0x00000001
#define LB64_MASK   0x0000000000000001
#define L64_MASK    0x00000000ffffffff
#define H64_MASK    0xffffffff00000000

uint64_t des_dec(uint64_t input, uint64_t sub_key) {
    
    int i, j;
    
    /* 8 bits */
    char row, column;
    
    /* 28 bits */
    uint32_t C                  = 0;
    uint32_t D                  = 0;
    
    /* 32 bits */
    uint32_t L                  = 0;
    uint32_t R                  = 0;
    uint32_t s_output           = 0;
    uint32_t f_function_res     = 0;
    uint32_t temp               = 0;
    
    /* 48 bits */
    uint64_t s_input            = 0;
    
    /* 56 bits */
    uint64_t permuted_choice_1  = 0;
    uint64_t permuted_choice_2  = 0;
    
    /* 64 bits */
    uint64_t init_perm_res      = 0;
    uint64_t inv_init_perm_res  = 0;
    uint64_t pre_output         = 0;
    
    // /* initial permutation */
    // for (i = 0; i < 64; i++) {
        
    //     init_perm_res <<= 1;
    //     init_perm_res |= (input >> (64-IP[i])) & LB64_MASK;
        
    // }
    
    L = (uint32_t) (input >> 32) & L64_MASK;
    R = (uint32_t) input & L64_MASK;

    
    // round轮DES加密
    for (i = 0; i < 1; i++) {
        
        /* f(R,k) function */
        s_input = 0;
        
        for (j = 0; j< 48; j++) {
            
            s_input <<= 1;
            s_input |= (uint64_t) ((R >> (32-E[j])) & LB32_MASK);
            
        }
        
        /* 
         * Encryption/Decryption 
         * XORing expanded Ri with Ki
         */
        
        s_input = s_input ^ sub_key;
        
        /* S-Box Tables */
        for (j = 0; j < 8; j++) {
            // 00 00 RCCC CR00 00 00 00 00 00 s_input
            // 00 00 1000 0100 00 00 00 00 00 row mask
            // 00 00 0111 1000 00 00 00 00 00 column mask
            
            row = (char) ((s_input & (0x0000840000000000 >> 6*j)) >> 42-6*j);
            row = (row >> 4) | row & 0x01;
            
            column = (char) ((s_input & (0x0000780000000000 >> 6*j)) >> 43-6*j);
            
            s_output <<= 4;
            s_output |= (uint32_t) (S[j][16*row + column] & 0x0f);
            
        }
        
        f_function_res = 0;
        
        for (j = 0; j < 32; j++) {
            
            f_function_res <<= 1;
            f_function_res |= (s_output >> (32 - P[j])) & LB32_MASK;
            
        }
        
        temp = R;
        R = L ^ f_function_res;
        L = temp;
        // uint64_t temp1 = (((uint64_t) L) << 32) | (uint64_t) R;
        // printf ("inner: %016llx\n", temp1);
    }
    
    pre_output = (((uint64_t) L) << 32) | (uint64_t) R;
    // /* inverse initial permutation */
    // for (i = 0; i < 64; i++) {
        
    //     inv_init_perm_res <<= 1;
    //     inv_init_perm_res |= (pre_output >> (64-PI[i])) & LB64_MASK;
        
    // }
    
    return pre_output;
    // return inv_init_perm_res;
    
}

uint16_t subkey[8] = {0x3b, 0x21, 0x28, 0x36, 0x14, 0x3c, 0x05, 0x30};//no use

#define PL_MASK 0x80000000  //1
#define PR_MASK 0x00938202    
#define CL_MASK 0x80000000
#define CR_MASK 0x00938202

int subkey_mask[8] = {1,0,1,0,0,0,1,1};//这四个S盒和五轮线近似式有关，这是第6轮，
int known_mask[48] = {1,0,1,1,0,1,
                      1,0,0,1,0,0,
                      1,0,1,1,1,0,
                      0,0,0,0,1,0,
                      1,1,1,0,0,0,
                      0,1,1,1,1,0,
                      1,0,1,0,1,1,
                      1,1,0,0,0,1,};//老师给出的32bit推出的24bit位置
uint64_t known_key[48] =  {1,1,1,0,1,1,
                           1,0,0,0,0,1,
                           1,0,1,0,0,0,
                           1,1,0,1,1,0,
                           0,1,0,1,0,0,
                           1,1,1,1,0,0,
                           0,0,0,1,0,1,
                           1,1,0,0,0,0};//

int get_xor(uint64_t m,uint64_t c)
{
    uint32_t PL = 0,PR = 0;
    uint32_t CL = 0,CR = 0;
    PL = (uint32_t) (m >> 32) & L64_MASK;
    PR = (uint32_t) m & L64_MASK;
    CL = (uint32_t) (c >> 32) & L64_MASK;
    CR = (uint32_t) c & L64_MASK;
    
    PL = PL & PL_MASK;
    PR = PR & PR_MASK;

    CL = CL & CL_MASK;
    CR = CR & CR_MASK;

    uint32_t tmp = PL ^ PR ^ CL ^ CR;

    int res = 0;

    for(int i = 0; i < 32; i++){
        // res ^= ((PL >> i) & 1);
        // res ^= ((PR >> i) & 1);
        // res ^= ((CL >> i) & 1);
        // res ^= ((CR >> i) & 1);
        res ^= ((tmp >> i) & 1);
    }
    return res;
}

int m0 = 19000;
int n = 0,n0;
int T[1<<24];
uint64_t k[1<<24];

uint64_t subkey_init()//把已知的密钥比特加载进去。
{
    uint64_t key_init = 0;
    for(int i = 0; i < 8; i++){
        if(subkey_mask[i] == 1){
            for(int j = 0; j < 6; j++){
                if(known_mask[i*6+j] == 1){
                    key_init = (key_init << 1) | known_key[i*6+j];
                }else{
                    key_init = (key_init << 1);
                }
            }
            // key_init = (key_init << 6)|subkey[i];
        }else{
            key_init = (key_init << 6);
        }
    }
    // printf("%llx\n",key_init);
    return key_init;
}

uint64_t gen_subkey(uint64_t key_init, uint64_t buf)//根据遍历的4个S盒的未知bit，生成并返回塞完的密钥
{
    uint64_t sub_key = key_init;
    int cnt = 0;
    // for(int i = 0; i < 48; i++){
    //     if(known_mask[i] == 0){
    //         sub_key |= (((buf>>(n-cnt+1))&(uint64_t)1) << (47-i));
    //     }
    // }

    for(int i = 0; i < 8; i++){
        if(subkey_mask[i] == 1){
            for(int j = 0; j < 6; j++){
                if(known_mask[i*6+j] == 0){
                    sub_key |= (((buf>>(n0-cnt-1))&((uint64_t)1)) << (47-(i*6+j)));
                    cnt++;
                }
            }
        }
    }
    
    // sub_key = key_init | buf;
    // printf("%d %d %d\n",buf,sub_key&0b111111,cnt);
    return sub_key;
}

uint64_t key0;

void get_cnt(uint64_t m, uint64_t c0)
{              //遍历所有密钥并jishu，
    uint64_t key, key_init;
    uint64_t c;
    key_init = key0;
    for(uint64_t i = 0; i < n; i++){
        key = key_init | i;
        key = gen_subkey(key_init, i);
        k[i] = i;
        c = des_dec(c0, key);
        int res = get_xor(m,c);
        if(res == 0)
        {
            T[i]++;
        }
    }
    return ;
}

int main() 
{
    int flag = 0;
    freopen("1.txt","r",stdin);
    uint64_t P,C;
    for(int i = 0; i < 8; i++){
        if(subkey_mask[i] == 1){
            for(int j = 0; j < 6; j++){
                if(known_mask[i*6+j] == 0){
                    n++;
                }
            }
        }
    }
    printf("%d\n",n);
    n0 = n;
    n = 1<<n;
    
    //for (int num = 0; num < 5; num++)
    //{
        key0 = subkey_init();
        for (int i = 0; i < m0; i++) 
        {
            scanf("%llx %llx", &P, &C);
            get_cnt(P, C);
        }
        int maxn = 0, t_max;
        int minn = 10086, t_min;
        for (uint64_t i = 0; i < n; i++) {
            if (T[i] > maxn) {
                t_max = i;
                maxn = T[i];
            }
            if (T[i] < minn) {
                t_min = i;
                minn = T[i];
            }
        }
        if (maxn - (m0 / 2) > (m0 / 2) - minn)
        {
            uint64_t ans1 = gen_subkey(key0, k[t_max]);
            printf("%llx,%llx\n", k[t_max],ans1);
            if (ans1 == 0xa40a00000072) flag++;
        }
        else {
            
            uint64_t ans2 = gen_subkey(key0, k[t_min]);
            printf("%llx,%llx\n", k[t_min], ans2);
            if (ans2 == 0xa40a00000072) flag++;
        }
           
    //}
    printf("%d", flag);
    return 0;
}