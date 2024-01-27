#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include<iostream>
#include <random>
#include <algorithm>
#include <bitset> 
#include"Des.h"
typedef unsigned char Byte;
typedef unsigned long long Bytes;
using namespace std;
char findkey[64];
char rep[8];
int mask_list[64] = { 0 };
Bytes KEY = 0x39F22BF00000;
Bytes Rotate(Bytes a, int n) 
{
    for (int i = 0; i < n; i++)
        a = (a << 1 | (a >> 27 & 1)) & ((1 << 28) - 1);
    return a;
}
void convertToBinary(long long key, char* binaryKey)
{
    for (int i = 0; i < 64; i++) {
        binaryKey[i] = (key >> (63 - i)) & 1 ? '1' : '0';
    }
}
void decconvertToBinary(long long key, char* binaryKey)
{
    for (int i = 0; i < 8; i++) {
        binaryKey[i] = (key >> (7 - i)) & 1 ? '1' : '0';
    }
}
Bytes Distribute(Bytes a, int b[], int m, int n) {
    Bytes c = 0;
    for (int i = m - 1; i >= 0; i--) 
    {
        c = c | ((a & 1) << (n - b[i] - 1));
        a >>= 1;
    }
    return c;
}
void ReBox(int a[], int b[], int m, int n) 
{
    for (int i = 0; i < n; i++) 
    {
        b[i] = -1;
    }
    for (int i = 0; i < m; i++) 
    {
        b[a[i]] = i;
    }
}
Bytes Permutate(Bytes a, int b[], int m, int n) {
    Bytes c = 0;
    for (int i = 0; i < n; i++) {
        c = c << 1 | (a >> (m - b[i] - 1) & 1);
    }
    return c;
}

Byte Permutate(Byte a, Byte b[], int m, int n) {
    Byte c = 0;
    for (int i = 0; i < n; i++) {
        c = c << 1 | (a >> (m - b[i] - 1) & 1);
    }
    return c;
}
Bytes RePC1(Bytes l, Bytes r)
{
    int pc1[56] = {
        56, 48, 40, 32, 24, 16,  8,
         0, 57, 49, 41, 33, 25, 17,
         9,  1, 58, 50, 42, 34, 26,
        18, 10,  2, 59, 51, 43, 35,
        62, 54, 46, 38, 30, 22, 14,
         6, 61, 53, 45, 37, 29, 21,
        13,  5, 60, 52, 44, 36, 28,
        20, 12,  4, 27, 19, 11,  3
    };
    int r_pc1[64] = { 0 };
    ReBox(pc1, r_pc1, 56, 64);

    Bytes combined={};
    
    combined |= l;
    combined <<= 28;
    combined |= r;
    //cout << "num:" << combined << " ";
    return Permutate(combined, r_pc1, 56, 64);
}

Bytes RePC2(Bytes key)
{
    int pc2[48] = {
        13, 16, 10, 23,  0,  4,
         2, 27, 14,  5, 20,  9,
        22, 18, 11,  3, 25,  7,
        15,  6, 26, 19, 12,  1,
        40, 51, 30, 36, 46, 54,
        29, 39, 50, 44, 32, 47,
        43, 48, 38, 55, 33, 52,
        45, 41, 49, 35, 28, 31
    };
    int r_pc2[56] = { 0 };
    ReBox(pc2, r_pc2, 48, 56);
    return Permutate(key, r_pc2, 48, 56);
}
Bytes ReKeygen(Bytes final_key, int round) 
{
    for (int mm = 0; mm <= 47; mm++)
    {
        final_key = RePC2(final_key);
        int loss[1] = { mm };
        Bytes mask = 0;
        for (int i = 0; i < 1; i++) {
            mask = mask | 1ULL << (55 - loss[i]);
        }

        Bytes l = final_key >> 28;
        Bytes r = final_key ^ l << 28;
        Bytes mask_l = mask >> 28;
        Bytes mask_r = mask ^ mask_l << 28;
        int offset[16] = {
             1,  1,  2,  2,  2,  2,  2,  2,
             1,  2,  2,  2,  2,  2,  2,  1
        };

        for (int i = round - 1; i >= 0; i--) {
            l = Rotate(l, 28 - offset[i]);
            r = Rotate(r, 28 - offset[i]);
            mask_l = Rotate(mask_l, 28 - offset[i]);
            mask_r = Rotate(mask_r, 28 - offset[i]);
        }
        Bytes key = RePC1(l, r);

        //cout << hex<<"key:" << key << endl;
        mask = RePC1(mask_l, mask_r);

        //int mask_list[64] = { 0 };
        int j = 0;
        //cout << "mask:" << mask << endl;
        for (int i = 63; i >= 0; i--)
        {
            if (mask & 1)
            {
                mask_list[j] = i;
                j++;
            }
            mask >>= 1;
        }

        convertToBinary(key, findkey);
        //cout << "key:" << bitset<sizeof(key) * 8>(key) << endl;
       // cout << "key:";

        for (int i = 0; i < 64; i++)
        {
            int ch = 0;
            for (int k = 0; k < j; k++)
            {
                if (i == mask_list[k])
                {
                    //ch = 1;
                    //cout << "*";
                    findkey[i] = '*';
                }

            }
            if ((i + 1) % 8 == 0 && i >= 3 && i != 7 && i != 23 && i != 55)
            {
                //ch = 1;
               // cout << "!";
                findkey[i] = '0';
            }
            // cout << findkey[i];
        }

        findkey[7] = '1';
        findkey[23] = '1';
        findkey[55] = '1';
        for (int i = 0; i < 64; i++)
            cout << findkey[i];
        cout << "    ";
        //cout << "KEY:" << bitset<sizeof(KEY) * 8>(KEY) << endl;
        cout << "第六轮的bit--" << mm+1 << " ";
        //cout << "j:" << j << endl;
        for (int i = 0; i < j; i++)
            cout <<"主密钥比特--" << dec << mask_list[i] + 1 << " ";
        cout << endl;

        Bytes guess_key = 0;
        /*for (int i = 0; i < 1 << 8; i++)
        {
            guess_key = Distribute(i, mask_list, 8, 64) | key;
            cout <<hex<<guess_key<<endl;
        }*/
    }
   
    return 0;
}
long long binaryStringToLongLong(const char* binaryStr) 
{
    //int length = strlen(binaryStr);
    long long result = 0;

    for (int i = 0; i < 64; i++) 
    {
        result <<= 1;  // 左移一位，为下一位二进制数腾出空间
        result += binaryStr[i] - '0';  // 将字符 '0' 或 '1' 转换为对应的数字
    }

    return result;
}


void FIND()
{
    for (int i = 0; i < 256; i++)
    {
        decconvertToBinary(i, rep);
        for(int j=0;j<8;j++)
          findkey[mask_list[j]] = rep[j];
        long long result = binaryStringToLongLong(findkey);
        std::memcpy(&result, &result, sizeof(uint64_t));
        uint64_t test = 0x289b47de9785d611;
        uint64_t in = 0x1122334455667788;
        uint64_t res = des(in, result, 6, 'e'); //e:表示加密  d:表示解密
        cout << hex << result << endl;
        if (res == test)
        {
            cout << "find it! The master key is:" << result << endl;
            return ;
        }
    }
    cout << "no!";
}

int main()
{
    ReKeygen(0xee1a3653c170, 6);
    //Can be modified to traverse the complete round key
    //FIND();
    return 0;
}