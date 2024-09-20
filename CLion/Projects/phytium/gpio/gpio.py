'''
以下以操作GPIO3_1作为输入和GPIO1_12作为输出的为例 ，说明GPIO的使用方法。
GPIO1_12归属于gpiochip1 ，gpiochip1对应的编号从480到495 ，GPIO1_12偏移12 ，所以对应的编号为 480+12=492。
同理 ，GPIO3_1的编号归属于gpiochip3 ，gpiochip3对应的编号从480到495，GPIO3_1偏移1，所以对应编号为 448+1=449
以下程序将GPIO1_12每2秒翻转一次电平，然后从GPIO3_1读回，执行5次后退出程序。
'''
import os
import time
os.system("echo 492> /sys/class/gpio/export")
os.system("echo 449> /sys/class/gpio/export")
os.system("echo out >/sys/class/gpio/gpio492/direction")
os.system("echo in >/sys/class/gpio/gpio449/direction")
nCnt = 5
while(nCnt):
    os.system("echo 0 >/sys/class/gpio/gpio492/value")
    time.sleep(1)
    os.system("cat /sys/class/gpio/gpio449/value")
    os.system("echo 1 >/sys/class/gpio/gpio492/value")
    time.sleep(1)
    os.system("cat /sys/class/gpio/gpio449/value")
    nCnt=nCnt-1
os.system("echo 492 > /sys/class/gpio/unexport")
os.system("echo 449 > /sys/class/gpio/unexport")
print("执行完毕!")
