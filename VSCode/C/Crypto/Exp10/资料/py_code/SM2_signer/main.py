from sm3 import sm3
from sm2_func import *

if __name__ == '__main__':
    p = int(input())
    a = int(input())
    b = int(input())
    ecc = ECC(a, b, p)
    g = input_point()
    n = int(input())
    id_A = input().encode('utf-8')
    pub_A = input_point()
    m = input().encode('utf-8')
    mode = input()
    if mode == 'Sign':
        d_A = int(input())
        k = int(input())
        sm2 = SM2(ecc=ecc, g=g, ord_g=n, p_pub=pub_A, d_pri=d_A,id_A=id_A)
        r, s = sm2.sign(m, k)
        print(r)
        print(s)
    else:
        r = int(input())
        s = int(input())
        sm2 = SM2(ecc=ecc, g=g, ord_g=n, p_pub=pub_A,id_A=id_A)
        print(sm2.verify(m, r, s))
