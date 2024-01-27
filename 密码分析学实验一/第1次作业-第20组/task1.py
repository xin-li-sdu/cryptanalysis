S = [2, 1, 0, 4, 3, 5, 7, 6, 8, 9, 24, 11, 20, 13, 14, 15, 16, 17,18, 19, 12, 21, 22, 23, 10, 25]

R0_raw = [0, 18, 24, 10, 12, 20, 8, 6, 14, 2, 11, 15, 22, 3, 25, 7, 17, 13, 1, 5, 23, 9, 16, 21, 19, 4]
R1_raw = [0, 10, 4, 2, 8, 1, 18, 20, 22, 19, 13, 6, 17, 5, 9, 3, 24, 14, 12, 25, 21, 11, 7, 16, 15, 23]
T = [10, 20, 14, 8, 25, 15, 16, 21, 3, 18, 0, 23, 13, 12, 2, 5, 6, 19, 9, 17, 1, 7, 24, 11, 22, 4]

R0 = [0]*26
R1 = [0]*26

for i in range(26):
    R0[R0_raw[i]] = i
    R1[R1_raw[i]] = i

k = [4,4]

m = 'wetter'
#m = 'illgrv'
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
    tmp = (R0[(tmp + k[0])%26]-k[0]+26)%26
    tmp = (R1[(tmp + k[1])%26]-k[1]+26)%26
    tmp = T[tmp]
    tmp = (R1_raw[(tmp + k[1])%26]-k[1]+26)%26
    tmp = (R0_raw[(tmp + k[0])%26]-k[0]+26)%26
    tmp = S[tmp]
    c = c + chr(tmp+ord('a'))
    Rotate(k)
print(c)

# for i in m:
#     tmp = ord(i)-ord('a')
#     print(tmp)
#     tmp = S[tmp]
#     print(tmp)
#     tmp = (R0[(tmp + k[0])%26]-k[0]+26)%26
#     print(tmp)
#     tmp = (R1[(tmp + k[1])%26]-k[1]+26)%26
#     print(tmp)
#     tmp = T[tmp]
#     print(tmp)
#     tmp = (R1_raw[(tmp + k[1])%26]-k[1]+26)%26
#     print(tmp)
#     tmp = (R0_raw[(tmp + k[0])%26]-k[0]+26)%26
#     print(tmp)
#     tmp = S[tmp]
#     print(tmp)
#     c = c + chr(tmp+ord('a'))
#     Rotate(k)
#     print(k)
# print(c)
