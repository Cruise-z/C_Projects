'''
本程序实现串口实时接收数据的功能，并且接收数据成功后回复OK
如果提示缺少串口模块，需要用以下命令安装pyserial
$ pip install pyserial
'''

import serial
from time import sleep
 
def recv(serial):
  while True:
    data = serial.read_all()
    if data == '':
      continue
    else:
      break
    sleep(0.02)
  return data
 
if __name__ == '__main__':
  serial = serial.Serial('/dev/ttyAMA2',115200,timeout=5)
  if serial.isOpen() :
    print("open success")
  else :
    print("open failed")

  while True:
    data =recv(serial)
    if data != b'' :
      print("receive : ",data)
      serial.write(b'ok\r\n') #数据写回，接收成功
