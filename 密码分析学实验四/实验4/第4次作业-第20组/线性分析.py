import time
import numpy as np
lenoflat=64
sbox = [[0xd6, 0x90, 0xe9, 0xfe, 0xcc, 0xe1, 0x3d, 0xb7, 0x16, 0xb6, 0x14, 0xc2, 0x28, 0xfb, 0x2c, 0x05,],
    [0x2b, 0x67, 0x9a, 0x76, 0x2a, 0xbe, 0x04, 0xc3, 0xaa, 0x44, 0x13, 0x26, 0x49, 0x86, 0x06, 0x99,],
    [0x9c, 0x42, 0x50, 0xf4, 0x91, 0xef, 0x98, 0x7a, 0x33, 0x54, 0x0b, 0x43, 0xed, 0xcf, 0xac, 0x62,],
    [0xe4, 0xb3, 0x1c, 0xa9, 0xc9, 0x08, 0xe8, 0x95, 0x80, 0xdf, 0x94, 0xfa, 0x75, 0x8f, 0x3f, 0xa6,],
    [0x47, 0x07, 0xa7, 0xfc, 0xf3, 0x73, 0x17, 0xba, 0x83, 0x59, 0x3c, 0x19, 0xe6, 0x85, 0x4f, 0xa8,],
    [0x68, 0x6b, 0x81, 0xb2, 0x71, 0x64, 0xda, 0x8b, 0xf8, 0xeb, 0x0f, 0x4b, 0x70, 0x56, 0x9d, 0x35,],
    [0x1e, 0x24, 0x0e, 0x5e, 0x63, 0x58, 0xd1, 0xa2, 0x25, 0x22, 0x7c, 0x3b, 0x01, 0x21, 0x78, 0x87,],
    [0xd4, 0x00, 0x46, 0x57, 0x9f, 0xd3, 0x27, 0x52, 0x4c, 0x36, 0x02, 0xe7, 0xa0, 0xc4, 0xc8, 0x9e,],
    [0xea, 0xbf, 0x8a, 0xd2, 0x40, 0xc7, 0x38, 0xb5, 0xa3, 0xf7, 0xf2, 0xce, 0xf9, 0x61, 0x15, 0xa1,],
    [0xe0, 0xae, 0x5d, 0xa4, 0x9b, 0x34, 0x1a, 0x55, 0xad, 0x93, 0x32, 0x30, 0xf5, 0x8c, 0xb1, 0xe3,],
    [0x1d, 0xf6, 0xe2, 0x2e, 0x82, 0x66, 0xca, 0x60, 0xc0, 0x29, 0x23, 0xab, 0x0d, 0x53, 0x4e, 0x6f,],
    [0xd5, 0xdb, 0x37, 0x45, 0xde, 0xfd, 0x8e, 0x2f, 0x03, 0xff, 0x6a, 0x72, 0x6d, 0x6c, 0x5b, 0x51,],
    [0x8d, 0x1b, 0xaf, 0x92, 0xbb, 0xdd, 0xbc, 0x7f, 0x11, 0xd9, 0x5c, 0x41, 0x1f, 0x10, 0x5a, 0xd8,],
    [0x0a, 0xc1, 0x31, 0x88, 0xa5, 0xcd, 0x7b, 0xbd, 0x2d, 0x74, 0xd0, 0x12, 0xb8, 0xe5, 0xb4, 0xb0,],
    [0x89, 0x69, 0x97, 0x4a, 0x0c, 0x96, 0x77, 0x7e, 0x65, 0xb9, 0xf1, 0x09, 0xc5, 0x6e, 0xc6, 0x84,],
    [0x18, 0xf0, 0x7d, 0xec, 0x3a, 0xdc, 0x4d, 0x20, 0x79, 0xee, 0x5f, 0x3e, 0xd7, 0xcb, 0x39, 0x48]]

def S(x):                       #s盒8位输入，查表字节代换8位输出
    row = (x>>4) & 0xf          #前4位行索引
    col = x & 0xf               #后4位列索引
    return sbox[row][col]

LAT_T=[]
t=[]#存储每一行偏差的最大值
mask_1 = 0x1

for t_out in range(256):#遍历输出掩码
    temp=[]
    for t_in in range(lenoflat):#遍历输入掩码
        ans=-128
        for s_in in range(256):  #遍历s盒的256对输入输出
            s_out = S(s_in)  #计算s盒输出
            res = 0  #线性近似表达式的值
            #计算线性近似表达式
            for x in range(8):
                if((t_in>>x) & mask_1==1):
                    res ^= ((s_in>>x) & mask_1)
            for y in range(8):
                if((t_out>>y) & mask_1==1):
                    res ^= ((s_out>>y) & mask_1)
            if(res==1): ans+=1
        temp.append(abs(ans))
        t.append(max(temp))
    LAT_T.append(temp)

#构造线性运算矩阵的转置Lt
lt=[[0]*32]*32


Lt=np.array([lt])

#将整数转换为32位向量
def int_to_vector(int_val):
    binary_str = np.binary_repr(int_val, width=32)
    vector = np.array(list(binary_str), dtype=int)
    return vector


result=[]
Gamma=[[0]*4,[0]*4,[0]*4,[0]*4]#每一轮的输入
for l in range(2,17):
    for j in range(4):#一组4个S盒
        #for遍历j位置的所有不为0的s盒输出掩码，同时其他S盒输出掩码为0：
        Lam=[[0]*4,[0]*4,[0]*4]#三轮S盒输出掩码,非j位置的所有s盒输出为0
        Gam=[[0]*4,[0]*4,[0]*4]#三轮S盒输入掩码
        
        for Lam0j in range(256):
            for Lam1j in range(256):
                for Lam2j in range(256):
                    Lam[0][j]=Lam0j
                    Lam[1][j]=Lam1j
                    Lam[2][j]=Lam2j
                    R0=[]
                    R1=[]
                    R2=[]
                    for i0 in range(lenoflat):#找到偏差最大的
                        if (LAT_T[Lam0j][i0]==t[Lam0j]): R0.append(i0)
                    for i1 in range(lenoflat):#找到偏差最大的
                        if (LAT_T[Lam1j][i1]==t[Lam1j]): R1.append(i1)
                    for i2 in range(lenoflat):#找到偏差最大的
                        if (LAT_T[Lam2j][i1]==t[Lam2j]): R2.append(i2)
                    #
                    gamma=[[0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0]]
                    for k0 in range(len(R0)):
                        Gam[0][j]=R0[k0]
                        for k1 in range(len(R1)):
                            Gam[1][j]=R1[k1]
                            for k2 in range(len(R2)):
                                Gam[2][j]=R2[k2]
                                gam=[0,0,0]
                                for i in range(3):
                                    tt=((Lam[i][0]&0xff)<<24) + ((Lam[i][1]&0xff)<<16) + ((Lam[i][2]&0xff)<<8) + ((Lam[i][3]&0xff))
                                    lam=int_to_vector(tt)
                                    Gamma[i][0]= np.dot(Lt,lam)
                                for i in range(3):
                                    gam[i]=((Gam[i][0]&0xff)<<24)+((Gam[i][1]&0xff)<<16)+((Gam[i][2]&0xff)<<8)+((Gam[i][3]&0xff))
                                    Gamma[3][0]=Gamma[0][0]
                                    Gamma[3][1]=Gamma[0][0]^gam[1]^gam[2]
                                    Gamma[3][2]=Gamma[1][0]^gam[2]
                                    Gamma[0][3]=Gamma[0][0]^gam[0]^gam[1]^gam[2]
                                    Gamma[0][1]=Gamma[1][0]^gam[0]
                                    Gamma[0][2]=Gamma[2][0]^gam[0]^gam[1]
                                    if ((Gamma[0][3]==Gamma[2][0])and(Gamma[0][1]==Gamma[3][1])and(Gamma[0][2]==Gamma[3][2])):
                                        result= [Gam[0][0],Gam[1][0],Gam[2][0],Gam[3][0],gam[0],gam[1],gam[2]]
                                else:
                                    continue
    l=l-2
