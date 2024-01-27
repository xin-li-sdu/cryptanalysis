from scipy.special import comb, perm
def C(n,i):
    return int(comb(n,i))
import math
tmp = 1
res = 1
for i in range(1,14):
    tmp *= C(26-2*(i-1),2)
    tmp = tmp//i
    res += tmp
res = res*26*26
print('密钥数量：',res,'改写为底为2的指数：',len(bin(res))-2,'改写为底为10的指数：',math.log(res/(10**6),10),'破解所需年数：',res//(31536000*10**6))