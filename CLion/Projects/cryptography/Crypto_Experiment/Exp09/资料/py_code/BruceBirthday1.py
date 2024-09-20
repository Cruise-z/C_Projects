import hashlib

import random
import string
import time


def loop(Hx,bit_len):
    for i in range(28,62):
        for j in range(26, 62):
            for k in range(2, 62):
                for l in range(2, 62):
                    for p in range(3, 62):
                        # for q in range(4, 62):
                        s=table[i]+table[j]+table[k]+table[l]+table[p]
                        ha = hashlib.sha1(s.encode('utf-8')).hexdigest()
                        if ha[:bit_len // 4] == Hx[:bit_len // 4]:
                            return s, ha
if __name__ == '__main__':
    table = string.ascii_letters+string.digits
    bit_len = int(input())
    Hx = input()
    start = time.time()
    s,ha=loop(Hx,bit_len)
    end = time.time()
    print(s)
    # print(end-start)