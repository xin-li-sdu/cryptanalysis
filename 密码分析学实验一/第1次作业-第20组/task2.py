m = 'wetter'
c = 'betrwpeer'
res = ''
print(m)
for i in range(len(c)-len(m)+1):
    tmp = c[i:i+len(m)]
    flag = 0
    for j in range(len(m)):
        if m[j] == tmp[j]:
            flag = 1
            break
    if flag == 1:
        continue
    print(tmp)
    res = tmp    #在字符串c中查找与字符串m不完全相同的子串，并将第一个满足条件的子串存储在变量res中
    
m_raw = m    
c = res
c_raw = c

flag = [0]*len(m)
def dfs(s,u):
    # print(u)
    if m[s] == u:
        return 1
    for i in range(len(m)):
        if(flag[i] == 1): 
            continue
        if m[i] == u:
            flag[i] = 1
            if dfs(s,c[i]) == 1:
                print((i,m[i],c[i]))
                flag[i] = 0
                return 1
            flag[i] = 0
    
    return 0
for i in range(len(m)):
    flag[i] = 1
    if dfs(i,c[i]) == 1:
        print((i,m[i],c[i]))
        print()
    flag[i] = 0

#(0,1,3),(1,2,5)

R0_raw = [0, 18, 24, 10, 12, 20, 8, 6, 14, 2, 11, 15, 22, 3, 25, 7, 17, 13, 1, 5, 23, 9, 16, 21, 19, 4]
R1_raw = [0, 10, 4, 2, 8, 1, 18, 20, 22, 19, 13, 6, 17, 5, 9, 3, 24, 14, 12, 25, 21, 11, 7, 16, 15, 23]
T = [10, 20, 14, 8, 25, 15, 16, 21, 3, 18, 0, 23, 13, 12, 2, 5, 6, 19, 9, 17, 1, 7, 24, 11, 22, 4]

R0 = [0]*26
R1 = [0]*26

for i in range(26):
    R0[R0_raw[i]] = i
    R1[R1_raw[i]] = i

def Rotate(k):
    k[0] -= 1
    if(k[0]==-1):
        k[1] -= 1
        k[0] = 25
        if(k[1]==-1):
            k[1] = 25
    return k

def Crypt(k,m):
    tmp = m
    # print(tmp)
    tmp = (R0[(tmp + k[0])%26]-k[0]+26)%26
    tmp = (R1[(tmp + k[1])%26]-k[1]+26)%26
    tmp = T[tmp]
    tmp = (R1_raw[(tmp + k[1])%26]-k[1]+26)%26
    tmp = (R0_raw[(tmp + k[0])%26]-k[0]+26)%26
    c = tmp
    # k = Rotate(k)
    return c

def R(k,r):
    tmp = k
    for _ in range(r):
        tmp = Rotate(tmp)
    return tmp

def check(l1,l0,l2):
    l = [-1] * 26
    # print(l)
    for i in range(len(l1)):
        print(l1[i],ord(m_raw[l2[i]])-ord('a'),end=' ')
        if (l[l1[i]] != -1) and (l[l1[i]] != ord(m_raw[l2[i]])-ord('a')):
            print()
            return 0
        if (l[ord(m_raw[l2[i]])-ord('a')] != -1) and (l1[i] != l[ord(m_raw[l2[i]])-ord('a')]):
            print()
            return 0
        l[ord(m_raw[l2[i]])-ord('a')] = l1[i]
        l[l1[i]] = ord(m_raw[l2[i]])-ord('a')
        print(l)
    
    for i in range(len(l0)):
        print(l0[i],ord(c_raw[l2[i]])-ord('a'),end=' ')
        if (l[l0[i]] != -1) and (l[l0[i]] != ord(c_raw[l2[i]])-ord('a')):
            print()
            return 0
        if (l[ord(c_raw[l2[i]])-ord('a')] != -1) and (l0[i] != l[ord(c_raw[l2[i]])-ord('a')]):
            print()
            return 0
        l[ord(c_raw[l2[i]])-ord('a')] = l0[i]
        l[l0[i]] = ord(c_raw[l2[i]])-ord('a')
        print(l)
    return l  #通过比较字符的映射关系来判断是否满足特定条件。在每次比较和赋值后，代码会输出映射关系列表l的当前状态。如果存在冲突或无法满足条件，则返回0。
#如果成功找到满足条件的字符映射关系，则返回最终的映射关系列表l。

ans = []
for i in range(26):
    for j in range(26):
        print([i,j])
        for m in range(26):
            k = [i,j]
            k0 = R([i,j],0)
            k1 = R([i,j],1)
            k2 = R([i,j],2)
            k3 = R([i,j],3)
            k5 = R([i,j],5)
            # print(k1,k2,k3)
            m1 = m
            c1 = Crypt(k3,Crypt(k1,Crypt(k0,m1)))
            m2 = Crypt(k0,m1)
            c2 = Crypt(k5,Crypt(k2,Crypt(k1,m2)))
            if c1 == m1 and c2 == m2:
                m0 = m
                m1 = Crypt(k0,m0)
                m3 = Crypt(k1,m1)
                m2 = m3
                m5 = Crypt(k2,m2)
                c0 = Crypt(k0,m0)
                c1 = Crypt(k1,m1)
                c2 = Crypt(k2,m2)
                c3 = Crypt(k3,m3)
                c5 = Crypt(k5,m5)
                if check([m0,m1,m2,m3,m5],[c0,c1,c2,c3,c5],[0,1,2,3,5]) == 0:
                    print()
                    continue
                print()
                S = check([m0,m1,m2,m3,m5],[c0,c1,c2,c3,c5],[0,1,2,3,5])
                for l in range(26):
                    if S[l] == -1:
                        S[l] = l
                print(k)
                ans.append([k,S])
                print('ans:',ans)
                break
            # print(k)

print(ans)
