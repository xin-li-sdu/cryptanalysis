import itertools
import tqdm

c_raw = 'etrwpe'

def Crypt(S,k,t):
    k0 = k.copy()
    # S = [2, 1, 0, 4, 3, 5, 7, 6, 8, 9, 24, 11, 20, 13, 14, 15, 16, 17,18, 19, 12, 21, 22, 23, 10, 25]
    if t == 0:
        R0_raw = [0, 18, 24, 10, 12, 20, 8, 6, 14, 2, 11, 15, 22, 3, 25, 7, 17, 13, 1, 5, 23, 9, 16, 21, 19, 4]
        R1_raw = [0, 10, 4, 2, 8, 1, 18, 20, 22, 19, 13, 6, 17, 5, 9, 3, 24, 14, 12, 25, 21, 11, 7, 16, 15, 23]
    else :
        R1_raw = [0, 18, 24, 10, 12, 20, 8, 6, 14, 2, 11, 15, 22, 3, 25, 7, 17, 13, 1, 5, 23, 9, 16, 21, 19, 4]
        R0_raw = [0, 10, 4, 2, 8, 1, 18, 20, 22, 19, 13, 6, 17, 5, 9, 3, 24, 14, 12, 25, 21, 11, 7, 16, 15, 23]
    T = [10, 20, 14, 8, 25, 15, 16, 21, 3, 18, 0, 23, 13, 12, 2, 5, 6, 19, 9, 17, 1, 7, 24, 11, 22, 4]

    R0 = [0]*26
    R1 = [0]*26

    for i in range(26):
        R0[R0_raw[i]] = i
        R1[R1_raw[i]] = i

    # k = [4,4]

    m = 'wetter'
    c = ''

    def Rotate(k):
        k[0] -= 1
        if(k[0]==-1):
            k[1] -= 1
            k[0] = 25
            if(k[1]==-1):
                k[1] = 25

    for i in m:
        tmp = ord(i)-ord('a')
        tmp = S[tmp]
        tmp = (R0[(tmp + k0[0])%26]-k0[0]+26)%26
        tmp = (R1[(tmp + k0[1])%26]-k0[1]+26)%26
        tmp = T[tmp]
        tmp = (R1_raw[(tmp + k0[1])%26]-k0[1]+26)%26
        tmp = (R0_raw[(tmp + k0[0])%26]-k0[0]+26)%26
        tmp = S[tmp]
        c = c + chr(tmp+ord('a'))
        Rotate(k0)
    if(c == c_raw):
        print(S,k,c)
    # print(ord(c[4])-ord('a'),S[ord(c[4])-ord('a')],ord('p')-ord('a'),S[ord('p')-ord('a')])
    # if ord(c[4])-ord('a') == S[ord(c[4])-ord('a')] and ord('p')-ord('a') == S[ord('p')-ord('a')]:
    #     print(S,k0,c,ord(c[4])-ord('a'),S[ord(c[4])-ord('a')],ord('p')-ord('a'),S[ord('p')-ord('a')])
    S[ord(c[4])-ord('a')] = ord('p')-ord('a')
    S[ord('p')-ord('a')] = ord(c[4])-ord('a')
    return S


ans = [[[2, 3], [0, 1, 2, 19, 21, 5, 22, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 20, 18, 3, 17, 4, 6, 23, 24, 25]], [[5, 16], [0, 1, 2, 3, 6, 5, 4, 7, 17, 9, 10, 11, 12, 13, 14, 15, 16, 8, 18, 25, 20, 21, 22, 23, 24, 19]], [[8, 6], [17, 1, 2, 3, 20, 5, 6, 19, 8, 9, 10, 11, 12, 13, 14, 15, 16, 0, 18, 7, 4, 21, 23, 22, 24, 25]], [[9, 7], [0, 19, 2, 3, 25, 5, 22, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 20, 18, 1, 17, 21, 6, 23, 24, 4]], [[12, 16], [0, 1, 17, 3, 24, 5, 6, 7, 8, 9, 10, 11, 12, 22, 14, 15, 16, 2, 18, 20, 19, 21, 13, 23, 4, 25]], [[18, 4], [0, 19, 2, 22, 24, 5, 17, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 6, 18, 1, 20, 21, 3, 23, 4, 25]], [[19, 6], [0, 1, 2, 4, 3, 5, 6, 7, 17, 19, 10, 22, 12, 13, 14, 15, 16, 8, 18, 9, 20, 21, 11, 23, 24, 25]], [[19, 20], [0, 1, 2, 3, 24, 5, 6, 17, 8, 9, 10, 11, 12, 13, 14, 15, 16, 7, 18, 19, 20, 21, 25, 23, 4, 22]], [[20, 20], [0, 1, 2, 3, 24, 22, 6, 7, 8, 17, 10, 11, 12, 13, 14, 15, 16, 9, 18, 25, 20, 21, 5, 23, 4, 19]], [[25, 1], [0, 1, 2, 17, 11, 5, 6, 22, 8, 19, 10, 4, 12, 13, 14, 15, 16, 3, 18, 9, 20, 21, 7, 23, 24, 25]], [[25, 8], [0, 1, 2, 3, 15, 5, 6, 7, 8, 19, 10, 11, 12, 13, 14, 4, 22, 24, 18, 9, 20, 21, 16, 23, 17, 25]], [[25, 16], [0, 22, 4, 3, 2, 5, 6, 7, 8, 19, 10, 11, 12, 13, 14, 15, 16, 23, 18, 9, 20, 21, 1, 17, 24, 25]]]

for k,S in ans:
    S0 = Crypt(S,k,0)
    Crypt(S0, k, 0)
    # Crypt(S,k,1)
    # print()