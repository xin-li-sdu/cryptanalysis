class U_way:
    def __init__(self,E,D) -> None:
        self.E = E
        self.D = D

    def mul(self,x,y):
        if y == '1':
            return x
        elif y == '0':
            return '0'
        elif x == '1*':
            return '1'
        elif x == '2*':
            return 't'
        else:
            return x

    def add(self,x,y):

        if x == '0':
            return y
        elif y == '0':
            return x

        elif ((x == '1') & (y == '1*')) | ((x == '1*') & (y == '1')):
            return '2*'

        else:
            return 't'
        
    def mat_mul(self,X,M):
        res = ['0']*len(M)
        for i in range(len(M)):
            for j in range(len(M[i])):
                tmp = self.mul(X[j],M[i][j])
                res[i] = self.add(res[i],tmp)
        return res

    def enc(self,P,r) -> list:
        l = []
        for _ in range(r):
            P = self.mat_mul(P,self.E)
            l.append(P)
        return l

    def dec(self,C,r):
        l = []
        for _ in range(r):
            C = self.mat_mul(C,self.D)
            l.append(C)
        return l

E = [['f','1','0','0'],#加密时，输出0为 F(p0) XOR p1
     ['0','0','1','0'],#输出1为 p2
     ['0','0','f','1'],#输出2为 F(p2) XOR p3
     ['1','0','0','0']]#输出3为 p0

D = [['0','0','0','1'],#解密时，输出0为 p3
     ['1','0','0','f'],#输出1为 p0 XOR F(p3)
     ['0','1','0','0'],#输出2为 p1
     ['0','f','1','0']]#输出3为 F(p1) XOR p2


CLF = U_way(E,D)

pc = [['0' if j == '0' else '1*' for j in bin(i)[2:].zfill(4)] for i in range(0,16)]

pi = []
ci = []
for t in pc[1:]:
    pi.append(CLF.enc(t,10))
    ci.append(CLF.dec(t,10))

def check(x,y):
    for i in range(4):#冲突判定
        if(x[i] == '0'):
            if (y[i] == '1')|(y[i] == '1*'):
                return 0
        elif x[i] == '1':
            if y[i] == '0':
                return 0
        elif x[i] == '1*':
            if (y[i] != '1')&(y[i] != 't'):
                return 0
        elif x[i] == '2*':
            if y[i] == '1*':
                return 0
        else:
            return 1
        
def reshape(l):
    return [l[1],l[0],l[3],l[2]]

for i in range(len(pi)):
    for j in range(len(ci)):
        p = pi[i]
        c = ci[j]
        #轮数
        maxn = 0
        pl0,cl0 = 0,0
        for pl in range(10):
            for cl in range(10):
                if(check(p[pl],c[cl])==0) & (maxn < pl+cl+3):
                    pl0 = pl
                    cl0 = cl
                    maxn = pl+cl+2
        if(maxn >= 9):
            print('轮数：',maxn,'头部差分',pc[i+1],'尾部差分',reshape(pc[j+1]),
                  '加密轮数',pl0,'解密轮数',cl0,'加密差分情况',p[pl0],'解密差分情况',c[cl0])
