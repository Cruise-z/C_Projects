from hashlib import sha256, sha1


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


p = int(input())
q = int(input())
a = int(input())
m = input()
# m = int.from_bytes(sha256(m).digest(), 'big')
mode = input()
if mode == 'Sign':
    s = int(input())
    r = int(input())
    x = pow(a, r, p)
    e = int.from_bytes(sha1((m + str(x)).encode('utf-8')).digest(),'big')
    y = (r + s * e) % q
    print(e, y)
else:
    v = int(input())
    e, y = map(int, input().split())
    _x = (pow(a, y, p) * pow(v, e, p))%p
    _e = int.from_bytes(sha1((m + str(_x)).encode('utf-8')).digest(),'big')
    print(e==_e)
