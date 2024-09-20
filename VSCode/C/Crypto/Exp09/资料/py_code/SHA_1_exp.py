import io
import struct


# 循环左移
def left_rotate(n, b):
    return ((n << b) | (n >> (32 - b))) & 0xffffffff


# 处理数据块,返回新的摘要
def process_chunk(chunk, h0, h1, h2, h3, h4):
    # 每个数据块：512bit == 64byte
    assert len(chunk) == 64
    # w是由512比特报文分组导出的80个32比特字
    w = [0] * 80

    # 前16个字直接取自当前分组中的16个字
    for i in range(16):
        # 解包为4字节的大端字w[i]
        w[i] = struct.unpack('>I', chunk[i * 4:i * 4 + 4])[0]

    # 余下的字定义为 Wt = (Wt−16⊕ Wt−14⊕ Wt−8⊕ Wt−3) <<< 1
    for i in range(16, 80):
        w[i] = left_rotate(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1)

    # 初始化MD缓存
    a = h0
    b = h1
    c = h2
    d = h3
    e = h4
    # 四个循环中的原始逻辑函数
    for i in range(80):
        if 0 <= i <= 19:
            # f = d ^ (b & (c ^ d))
            f = (b & c) | ((~b) & d)
            k = 0x5A827999
        elif 20 <= i <= 39:
            f = b ^ c ^ d
            k = 0x6ED9EBA1
        elif 40 <= i <= 59:
            f = (b & c) | (b & d) | (c & d)
            k = 0x8F1BBCDC
        elif 60 <= i <= 79:
            f = b ^ c ^ d
            k = 0xCA62C1D6

        a, b, c, d, e = ((left_rotate(a, 5) + f + e + k + w[i]) & 0xffffffff,
                         a, left_rotate(b, 30), c, d)

    # mod 2^32
    h0 = (h0 + a) & 0xffffffff
    h1 = (h1 + b) & 0xffffffff
    h2 = (h2 + c) & 0xffffffff
    h3 = (h3 + d) & 0xffffffff
    h4 = (h4 + e) & 0xffffffff

    return h0, h1, h2, h3, h4


class hash_sha1:
    def __init__(self):
        self._mdbuf = (
            0x67452301,
            0xEFCDAB89,
            0x98BADCFE,
            0x10325476,
            0xC3D2E1F0
        )
        # 未处理部分
        self._unproc = b''
        # 已处理长度
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
        return '%08x%08x%08x%08x%08x' % self._produce_digest()

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


def sha1(a):
    return hash_sha1().update(a).hexdigest()


if __name__ == '__main__':
    byte_in = input().encode('utf-8')

    print(sha1(byte_in))
