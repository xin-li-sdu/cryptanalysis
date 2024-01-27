class U_way:
    def __init__(self,E,D) -> None:
        self.E = E
        self.D = D

    def mul(self,x,y):
        # print(x,y)
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
        # print(x,y)
        if x == '0':
            return y
        elif y == '0':
            return x
        # elif (x == '1') & (y == '1'):
        #     return 't'
        elif ((x == '1') & (y == '1*')) | ((x == '1*') & (y == '1')):
            return '2*'
        # elif ((x == '1') & (y == '2*')) | ((x == '2*') & (y == '1')):
        #     return 't'
        else:
            return 't'
        
    def mat_mul(self,X,M):
        res = ['0']*len(M)
        for i in range(len(M)):
            for j in range(len(M[i])):
                tmp = self.mul(X[j],M[i][j])
                # print(tmp)
                res[i] = self.add(res[i],tmp)
        return res

    def enc(self,P,r) -> list:
        l = []
        for _ in range(r):
            P = self.mat_mul(P,self.E)
            l.append(P)
            # print(_+1,P)
        return l

    def dec(self,C,r):
        l = []
        for _ in range(r):
            C = self.mat_mul(C,self.D)
            l.append(C)
            # print(_+1,C)
        return l

E = [['0','1','0','0'],
     ['0','0','1','0'],
     ['0','0','0','1'],
     ['1','f','f','f']]

D = [['f','f','f','1'],
     ['1','0','0','0'],
     ['0','1','0','0'],
     ['0','0','1','0']]


# E = [['f','1'],
#      ['1','0']]

CLF = U_way(E,D)
# tmp.enc(['1','0','0','0'],5)
# CLF.enc(['0','0','0','1*'],10)
# tmp.dec(['1*','0','0','0'],10)
# tmp.enc(['1*','0','0','0'],10)

pc = [['0' if j == '0' else '1*' for j in bin(i)[2:].zfill(4)] for i in range(0,16)]

pi = []
ci = []
for t in pc[1:]:
    pi.append(CLF.enc(t,10))
    ci.append(CLF.dec(t,10))

def check(x,y):
    for i in range(4):
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
        maxn = 0
        pl0,cl0 = 0,0
        for pl in range(10):
            for cl in range(10):
                if(check(p[pl],c[cl])==0) & (maxn < pl+cl+3):
                    pl0 = pl
                    cl0 = cl
                    maxn = pl+cl+2
        if(maxn >= 6):
            print(maxn,pc[i+1],reshape(pc[j+1]),pl0,cl0,p[pl0],c[cl0])
