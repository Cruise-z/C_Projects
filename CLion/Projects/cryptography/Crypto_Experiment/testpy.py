p = 232313
M = "你是煞笔"
ans = (M + str(p)).encode("utf-8")
print(ans)
print(hex(ans[16]))
print(0x31)