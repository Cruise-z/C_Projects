from hashlib import sha1
n = 24

origin_str = 'I got these beliefs I think you wanna break, got something here to lose that I think you wanna take from me.'
origin_sha1 = sha1(origin_str.encode('utf-8')).hexdigest()


iter1 = ['I ', 'me ', 'i ', '瓦达西 ']
iter2 = ['got ', 'had ', 'held ']
iter3 = ['these ', 'the ']
iter4 = ['beliefs ', 'assumptions ', 'thought ']
iter5 = ['I ', 'me ', 'i ', '瓦达西 ']
iter6 = ['think ', 'assume ', 'guess ']
iter7 = ['you ', 'u ', 'U ']
iter8 = ['wanna ', 'want to ']
iter9 = ['break']
iter10 = [', ']
iter11 = ['got ', 'caught ', 'had ', 'found ']
iter12 = ['something ', 'little things ', 'some things ']
iter13 = ['here ']
iter14 = ['to lose ', 'lost ']
iter15 = ['that ', 'which ']
iter16 = ['I ', 'me ', 'i ', '瓦达西 ']
iter17 = ['think ', 'assume ', 'guess ']
iter18 = ['you ', 'u ', 'U ']
iter19 = ['wanna ', 'want to ']
iter20 = ['take from me.', 'make away from me.',' bring from me.']

for i1 in iter1:
    for i2 in iter2:
        for i3 in iter3:
            for i4 in iter4:
                for i5 in iter5:
                    for i6 in iter6:
                        for i7 in iter7:
                            for i8 in iter8:
                                for i9 in iter9:
                                    for i10 in iter10:
                                        for i11 in iter11:
                                            for i12 in iter12:
                                                for i13 in iter13:
                                                    for i14 in iter14:
                                                        for i15 in iter15:
                                                            for i16 in iter16:
                                                                for i17 in iter17:
                                                                    for i18 in iter18:
                                                                        for i19 in iter19:
                                                                            for i20 in iter20:
                                                                                t = i1+i2+i3+i4+i5+i6+i7+i8+i9+i10+i11+i12+i13+i14+i15+i16+i17+i18+i19+i20
                                                                                if sha1(t.encode('utf-8')).hexdigest()[:n//4] == origin_sha1[:n//4]:
                                                                                    print(t)
                                                                                    print(sha1(t.encode('utf-8')).hexdigest())

