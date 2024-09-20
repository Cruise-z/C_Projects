import io
import struct

T1 = 0x79cc4519
T2 = 0x7a879d8a


# 循环左移
def LR(n, b):
    return ((n << b) | (n >> (32 - b))) & 0xffffffff


def ADD(*arg):
    res = 0
    for i in arg:
        res = (res + i) & 0xFFFF_FFFF
    return res


def p0(x):
    return x ^ LR(x, 9) ^ LR(x, 17)


def p1(x):
    return x ^ LR(x, 15) ^ LR(x, 23)


def ff1(x, y, z):
    return x ^ y ^ z


def ff2(x, y, z):
    return (x & y) | (x & z) | (y & z)


def gg1(x, y, z):
    return x ^ y ^ z


def gg2(x, y, z):
    return (x & y) | ((~x) & z)


def process_chunk(chunk, h0, h1, h2, h3, h4, h5, h6, h7):
    assert len(chunk) == 64
    w = [0] * 68
    for i in range(16):
        # 解包为4字节的大端字w[i]
        w[i] = struct.unpack('>I', chunk[i * 4:i * 4 + 4])[0]
    for i in range(16, 68):
        w[i] = p1(w[i - 16] ^ w[i - 9] ^
                  LR(w[i - 3], 15)) ^ \
               LR(w[i - 13], 7) ^ w[i - 6]
    _w = [w[i] ^ w[i + 4] for i in range(64)]
    a, b, c, d, e, f, g, h = h0, h1, h2, h3, h4, h5, h6, h7
    for i in range(64):
        T = T1 if 0 <= i <= 15 else T2
        ss1 = LR(ADD(LR(a, 12), e, LR(T, i % 32)), 7)
        ss2 = ss1 ^ LR(a, 12)
        ff = ff1 if 0 <= i <= 15 else ff2
        gg = gg1 if 0 <= i <= 15 else gg2
        tt1 = ADD(ff(a, b, c), d, ss2, _w[i])
        tt2 = ADD(gg(e, f, g), h, ss1, w[i])
        d = c
        c = LR(b, 9)
        b = a
        a = tt1
        h = g
        g = LR(f, 19)
        f = e
        e = p0(tt2)
    h0, h1, h2, h3, \
    h4, h5, h6, h7 = a ^ h0, b ^ h1, c ^ h2, d ^ h3, \
                     e ^ h4, f ^ h5, g ^ h6, h ^ h7
    return h0, h1, h2, h3, h4, h5, h6, h7


class hash_sm3:
    def __init__(self):
        self._mdbuf = (
            0x7380166f,
            0x4914b2b9,
            0x172442d7,
            0xda8a0600,
            0xa96f30bc,
            0x163138aa,
            0xe38dee4d,
            0xb0fb0e4e
        )
        self._unproc = b''
        self._len_proc = 0

    def update(self, arg):
        if isinstance(arg, str):
            arg = arg.encode('utf-8')
        if isinstance(arg, (bytes, bytearray)):
            arg = io.BytesIO(arg)
        chunk = self._unproc + arg.read(64 - len(self._unproc))
        while len(chunk) == 64:
            self._mdbuf = process_chunk(chunk, *self._mdbuf)
            self._len_proc += 64
            chunk = arg.read(64)
        self._unproc = chunk
        return self

    def digest(self):
        return b''.join(struct.pack('>I', h) for h in self._produce_digest())

    def hexdigest(self):
        return '%08x%08x%08x%08x%08x%08x%08x%08x' % self._produce_digest()

    def _produce_digest(self):
        m = self._unproc
        m_byte_len = self._len_proc + len(m)
        # 填充首位是1，第一个填充字节为1000 0000
        m += b'\x80'
        # 剩余都填充0，使得填充后的比特数与448 mod 512同余
        # 字节数即 56 mod 64
        m += b'\x00' * ((56 - (m_byte_len + 1) % 64) % 64)
        m_bit_len = m_byte_len * 8
        # 在结尾加上比特长度，将其打包成64位大端无符号长整型
        m += struct.pack('>Q', m_bit_len)
        mdbuf = process_chunk(m[:64], *self._mdbuf)
        return mdbuf if len(m) == 64 else process_chunk(m[64:], *mdbuf)


def input_bytes():
    bytes_str = input().strip().replace('\n', '').replace('\r', '')
    if bytes_str[:2] == '0x':
        bytes_str = bytes_str[2:]
    return bytes.fromhex(bytes_str)


def xor(a: bytes, b: bytes):
    assert len(a) == len(b)
    return b''.join([(a[i] ^ b[i]).to_bytes(1, 'big') for i in range(len(a))])


def sm3(m):
    return hash_sm3().update(m).digest()


if __name__ == '__main__':
    m = input().encode('utf-8')
    print(sm3(m))
