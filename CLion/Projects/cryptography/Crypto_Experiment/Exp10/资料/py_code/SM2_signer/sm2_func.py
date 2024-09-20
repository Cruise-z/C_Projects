from math import ceil, log
from typing import Tuple
from sm3 import sm3

O = (0, 0)
# 安全参数（可以理解为椭圆曲线上点坐标的比特长度）
L = 256 // 8
H = 1
K_LEN = 128 // 8


def exgcd(a, b):
    if a == 0:
        return 0, 1, b
    if b == 0:
        return 1, 0, a

    px, ppx = 0, 1
    py, ppy = 1, 0

    while b:
        q = a // b
        a, b = b, a % b
        x = ppx - q * px
        y = ppy - q * py
        ppx, px = px, x
        ppy, py = py, y

    return ppx, ppy, a


def invmod(a, n):
    x, y, g = exgcd(a, n)

    assert g == 1
    return x % n


def input_point():
    x, y = map(int, input().split())
    return x, y


class ECC:
    def __init__(self, a: int, b: int, p: int):
        self.a = a
        self.b = b
        self.module = p

    def is_opposite(self, p1, p2):
        return p1[0] == p2[0] and p1[1] == -p2[1] % self.module

    def add(self, p1, p2):
        if p1 == O:
            return p2
        if p2 == O:
            return p1
        if self.is_opposite(p1, p2):
            return O
        x1, y1 = p1
        x2, y2 = p2
        l = 0
        if x1 != x2:
            l = (y2 - y1) * invmod(x2 - x1, self.module)
        else:
            l = (3 * x1 ** 2 + self.a) * invmod(2 * y1, self.module)
        x = (l * l - x1 - x2) % self.module
        y = (l * (x1 - x) - y1) % self.module
        return x, y

    def sub(self, p1, p2):
        p2 = p2[0], -p2[1]
        return self.add(p1, p2)

    def power(self, p: Tuple[int, int], n) -> Tuple[int, int]:
        if n == 0 or p == O:
            return O
        res = O
        # 快速模幂！
        while n:
            if n & 1:
                res = self.add(res, p)
            p = self.add(p, p)
            n >>= 1
        return res

    def inv_power(self, p, n):
        n = invmod(n, self.module)
        return self.power(p, n)

    def encrypt(self, p_m: tuple, p_pub: tuple, g: tuple, k: int):
        return self.power(g, k), self.add(p_m, self.power(p_pub, k))

    # c1=kg,c2=pm+kpa
    def decrypt(self, c1: tuple, c2: tuple, n_pri: int):
        return self.sub(c2, self.power(c1, n_pri))

    def is_curve_point(self, p: tuple):
        return (p[1] ** 2) % self.module == (p[0] ** 3 + self.a * p[0] + self.b) % self.module

    def DH(self, p_g, r, p_pub):
        return self.power(p_pub, r)


# z字节串，klen字节长度
def kdf(z: bytes, klen: int) -> bytes:
    hash_len = 256 // 8
    ha = [sm3(z + (i + 1).to_bytes(4, 'big')) for i in range(ceil(klen / hash_len))]
    return (b''.join(ha))[:klen]


def point_to_bytes(p: Tuple[int, int]):
    return b'\x04' + p[0].to_bytes(L, 'big') + p[1].to_bytes(L, 'big')


def bytes_to_point(b: bytes):
    return int.from_bytes(b[1:1 + L], 'big'), int.from_bytes(b[1 + L:], 'big')


def xor(a: bytes, b: bytes):
    assert len(a) == len(b)
    return b''.join([(a[i] ^ b[i]).to_bytes(1, 'big') for i in range(len(a))])


class SM2:
    def __init__(
            self, ecc: ECC, g: Tuple[int, int], ord_g: int = None,
            p_pub: Tuple[int, int] = None, d_pri: int = None,
            id_A: bytes = None, id_B: bytes = None
    ):
        self.ecc = ecc
        self.p_pub, self.d_pri = p_pub, d_pri
        self.g = g
        self.ord_g = ord_g
        self.id_A = id_A
        self.id_B = id_B

    def encrypt(self, m: bytes, k: int):
        c1 = point_to_bytes(self.ecc.power(self.g, k))
        # kg =
        x, y = self.ecc.power(self.p_pub, k)
        x_bytes = x.to_bytes(L, 'big')
        y_bytes = y.to_bytes(L, 'big')
        t = kdf(x_bytes + y_bytes, len(m))
        c2 = xor(m, t)
        c3 = sm3(x_bytes + m + y_bytes)
        return c1 + c2 + c3

    def decrypt(self, c: bytes):
        point_len = 2 * L + 1
        c1 = bytes_to_point(c[:point_len])
        x, y = self.ecc.power(c1, self.d_pri)
        x_bytes = x.to_bytes(L, 'big')
        y_bytes = y.to_bytes(L, 'big')
        hash_len = 256 // 8
        len_m = len(c) - point_len - hash_len
        t = kdf(x_bytes + y_bytes, len_m)
        c2 = c[point_len:point_len + len_m]
        m = xor(c2, t)
        c3 = c[point_len + len_m:]
        h = sm3(x_bytes + m + y_bytes)
        assert h == c3
        return m

    def Z_Gen(self, ID: bytes, p_pub: tuple):
        entl = (8 * len(ID)).to_bytes(2, 'big')

        a = self.ecc.a.to_bytes(L, 'big')
        b = self.ecc.b.to_bytes(L, 'big')
        x_g = self.g[0].to_bytes(L, 'big')
        y_g = self.g[1].to_bytes(L, 'big')
        x_A = p_pub[0].to_bytes(L, 'big')
        y_A = p_pub[1].to_bytes(L, 'big')
        Z = sm3(
            entl + ID + a + b + x_g + y_g + x_A + y_A
        )
        return Z

    def key_exchange_A(self, pub_B: tuple, RB: tuple, rA: int):
        x1, y1 = RA = self.ecc.power(self.g, rA)
        assert self.ecc.is_curve_point(RB)
        x2, y2 = RB
        w = ceil(ceil(log(self.ord_g, 2)) / 2) - 1
        # print('w',w)
        # w=127
        _x1 = (1 << w) + (x1 & ((1 << w) - 1))
        _x2 = (1 << w) + (x2 & ((1 << w) - 1))

        tA = (self.d_pri + _x1 * rA) % self.ord_g

        xu, yu = U = self.ecc.power(
            self.ecc.add(
                pub_B, self.ecc.power(RB, _x2)
            ),
            H * tA
        )
        assert U != O

        xu_bytes = xu.to_bytes(L, 'big')
        yu_bytes = yu.to_bytes(L, 'big')

        x1_bytes = x1.to_bytes(L, 'big')
        y1_bytes = y1.to_bytes(L, 'big')

        x2_bytes = x2.to_bytes(L, 'big')
        y2_bytes = y2.to_bytes(L, 'big')
        Z_A = self.Z_Gen(self.id_A, self.p_pub)
        Z_B = self.Z_Gen(self.id_B, pub_B)
        # assert Z_A == (8975752424761421080654347143368664550497303703175300729348341264760863063732).to_bytes(L,'big')
        # assert Z_B == (70508660685970560886879094163378225748672022479558429512974904072101864894629).to_bytes(L,'big')
        KA = kdf(xu_bytes + yu_bytes + Z_A + Z_B, K_LEN)
        SA = sm3(
            b'\x03' + yu_bytes + sm3(
                xu_bytes + Z_A + Z_B + x1_bytes
                + y1_bytes + x2_bytes + y2_bytes
            )
        )
        S1 = sm3(
            b'\x02' + yu_bytes + sm3(
                xu_bytes + Z_A + Z_B + x1_bytes
                + y1_bytes + x2_bytes + y2_bytes
            )
        )
        return KA, SA, S1

    def key_exchange_B(self, pub_A: tuple, RA: tuple, rB: int):
        x1, y1 = RA
        assert self.ecc.is_curve_point(RA)
        x2, y2 = RB = self.ecc.power(self.g, rB)
        w = ceil(ceil(log(self.ord_g, 2)) / 2) - 1
        # print('w',w)
        # w=127
        _x1 = (1 << w) + (x1 & ((1 << w) - 1))
        _x2 = (1 << w) + (x2 & ((1 << w) - 1))

        tB = (self.d_pri + _x2 * rB) % self.ord_g

        xu, yu = U = self.ecc.power(
            self.ecc.add(
                pub_A, self.ecc.power(RA, _x1)
            ),
            H * tB
        )
        assert U != O

        xu_bytes = xu.to_bytes(L, 'big')
        yu_bytes = yu.to_bytes(L, 'big')

        x1_bytes = x1.to_bytes(L, 'big')
        y1_bytes = y1.to_bytes(L, 'big')

        x2_bytes = x2.to_bytes(L, 'big')
        y2_bytes = y2.to_bytes(L, 'big')
        Z_A = self.Z_Gen(self.id_A, pub_A)
        Z_B = self.Z_Gen(self.id_B, self.p_pub)
        # assert Z_A == (8975752424761421080654347143368664550497303703175300729348341264760863063732).to_bytes(L,'big')
        # assert Z_B == (70508660685970560886879094163378225748672022479558429512974904072101864894629).to_bytes(L,'big')
        KA = kdf(xu_bytes + yu_bytes + Z_A + Z_B, K_LEN)
        SA = sm3(
            b'\x03' + yu_bytes + sm3(
                xu_bytes + Z_A + Z_B + x1_bytes
                + y1_bytes + x2_bytes + y2_bytes
            )
        )
        S1 = sm3(
            b'\x02' + yu_bytes + sm3(
                xu_bytes + Z_A + Z_B + x1_bytes
                + y1_bytes + x2_bytes + y2_bytes
            )
        )
        return KA, SA, S1

    def sign(self, m: bytes, k: int):
        ZA = self.Z_Gen(self.id_A, self.p_pub)
        # print('ZA',ZA)
        _m = ZA + m
        e = int.from_bytes(sm3(_m), 'big')
        x1, y1 = self.ecc.power(self.g, k)
        r = (e + x1) % self.ord_g
        s = (invmod(1 + self.d_pri, self.ord_g) * (k - r * self.d_pri)) % self.ord_g
        return r, s

    def verify(self, m, r, s):
        ZA = self.Z_Gen(self.id_A, self.p_pub)
        _m = ZA + m
        e = int.from_bytes(sm3(_m), 'big')
        t = (r + s) % self.ord_g
        x1, y1 = self.ecc.add(
            self.ecc.power(self.g, s),
            self.ecc.power(self.p_pub, t)
        )
        R = (e+x1)%self.ord_g
        return R==r

def input_bytes():
    bytes_str = input().strip().replace('\n', '').replace('\r', '')
    if bytes_str[:2] == '0x':
        bytes_str = bytes_str[2:]
    return bytes.fromhex(bytes_str)


if __name__ == '__main__':
    part = input().strip().replace('\n', '').replace('\r', '')
    p = int(input())
    a = int(input())
    b = int(input())
    g = input_point()
    ord_g = int(input())
    id_A = input_bytes()
    id_B = input_bytes()
    d = int(input())
    pub_A = input_point()
    pub_B = input_point()
    r = int(input())
    R = input_point()

    ecc = ECC(a, b, p)
    if part == 'A':
        sm2 = SM2(
            ecc=ecc, g=g, p_pub=pub_A, d_pri=d, ord_g=ord_g,
            id_A=id_A, id_B=id_B,
        )
        KA, SA, S1 = sm2.key_exchange_A(pub_B, R, r)
        print(int.from_bytes(KA, 'big'))
        print(
            int.from_bytes(S1, 'big'), int.from_bytes(SA, 'big')
        )
    elif part == 'B':
        sm2 = SM2(
            ecc=ecc, g=g, p_pub=pub_B, d_pri=d, ord_g=ord_g,
            id_A=id_A, id_B=id_B,
        )
        KB, S2, SB = sm2.key_exchange_B(pub_A, R, r)
        print(int.from_bytes(KB, 'big'))
        print(
            int.from_bytes(SB, 'big'), int.from_bytes(S2, 'big')
        )
