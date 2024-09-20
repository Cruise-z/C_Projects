import hashlib
from hashlib import sha1
from math import ceil


class RSA_PSS:
    def __init__(self, M, n: int, emBits: int, salt):
        self.M = M
        self.n = n
        self.emBits = emBits
        self.emLen = ceil(self.emBits / 8)
        self.maskLen = self.emLen - 21
        self.salt = salt

    def EM(self):
        byte_salt = bytes.fromhex(self.salt)
        M1 = b'\x00' * 8 + hash_1(self.M.encode('utf-8')) + byte_salt
        H = hash_1(M1)
        DB = b'\x00' * (self.emLen - 20 - 20 - 2) + b'\x01' + byte_salt
        mgf_H = mgf(H, maskLen=self.maskLen)
        maskDB = xor(mgf_H, DB)
        new_byte = _and_byte(maskDB[0], 255 >> (8 * self.emLen - self.emBits))
        maskDB = new_byte + maskDB[1:]
        EM = maskDB + H + b'\xbc'
        return EM

    def sign(self, d: int):
        em = self.EM()
        s = pow(int.from_bytes(em, byteorder='big'), d, self.n)
        k = 256
        # S=s.to_bytes(k,byteorder='big')
        S = hex(s)[2:]
        return S

    def verify(self, e: int, S: int):
        em = pow(S, e, self.n)
        EM = hex(em)[2:]
        L = len(EM)
        for i in range(self.emLen * 2 - L):
            EM = '0' + EM
        maskDB = EM[:self.maskLen * 2]
        if EM[-2:] != 'bc' or self.emLen < 42:
            return False
        H = EM[(self.maskLen) * 2:(self.maskLen) * 2 + 40]
        dbMask = mgf(bytes.fromhex(H), maskLen=self.maskLen)
        # print('H:',H)
        DB = xor(dbMask, bytes.fromhex(maskDB))
        new_byte = _and_byte(DB[0], 255 >> (8 * self.emLen - self.emBits))
        DB = new_byte + DB[1:]
        salt = DB[-20:]
        M1 = b'\x00' * 8 + hash_1(self.M.encode('utf-8')) + salt
        H1 = sha1(M1).hexdigest()
        # print('H1',H1)
        return H1 == H


def hash_1(a: bytes):
    sha = hashlib.sha1(a)
    return sha.digest()


def mgf(z: bytes, maskLen: int):
    hash_len = 20
    ha = [hash_1(z + i.to_bytes(4, 'big')) for i in range(ceil(maskLen / hash_len))]
    return (b''.join(ha))[:maskLen]


def xor(a: bytes, b: bytes):
    assert len(a) == len(b)
    return b''.join([(a[i] ^ b[i]).to_bytes(1, 'big') for i in range(len(a))])


def _and_byte(a, b):
    return bytes([a & b])


if __name__ == '__main__':
    M = input().strip().replace('\n', '').replace('\r', '')
    n = int(input())
    emBits = int(input())
    op = input().strip().replace('\n', '').replace('\r', '')
    if op == 'Sign':
        d = int(input())
        salt = input().strip().replace('\n', '').replace('\r', '')
        rsa = RSA_PSS(M, n, emBits, salt)
        print(rsa.sign(d))
    else:
        e = int(input())
        S = int(input().strip().replace('\n', '').replace('\r', ''), 16)
        rsa = RSA_PSS(M, n, emBits, salt='92')
        print(rsa.verify(e, S))