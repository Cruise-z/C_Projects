1、安装一个IIC工具sudo apt-get install i2c-tools
2、用i2cdetect -l 测试所有总线
   用i2cdetect -y -r 2 测试总线0~3上的设备，在i2c-2找到一个地址为38的设备
3、则本案例用到40pin引脚中的引脚3和5，对应的IIC设备节点为"/dev/i2c-2"，设备地址为38
