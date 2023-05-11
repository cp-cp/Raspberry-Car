#!/usr/bin/python
# caiji
import smbus
import math

import time

# 绘图导入的模块
import numpy as np
import math
from datetime import datetime
import matplotlib.pyplot as plt

# 动态绘图的一个函数，一定注意
plt.ion() #开启interactive mode 成功的关键函数


# Power management registers
power_mgmt_1 = 0x6b
power_mgmt_2 = 0x6c

x_t = 0
y_t = 0
z_t = 0

x_a = 0
y_a = 0
z_a = 0

x_r = 0
y_r = 0

def read_byte(adr):
    return bus.read_byte_data(address, adr)


def read_word(adr):
    high = bus.read_byte_data(address, adr)
    low = bus.read_byte_data(address, adr+1)
    val = (high << 8) + low
    return val

#读取寄存器中的数据并转为补码
def read_word_2c(adr):
    val = read_word(adr)
    if (val >= 0x8000):
        return -((65535 - val) + 1)
    else:
        return val

def dist(a,b):
    return math.sqrt((a*a)+(b*b))

def get_y_rotation(x,y,z):
    radians = math.atan2(x, dist(y,z))
    return -math.degrees(radians)

def get_x_rotation(x,y,z):
    radians = math.atan2(y, dist(x,z))
    return math.degrees(radians)

bus = smbus.SMBus(1) # 1表示树莓系统是2.0 ，如果是1.0则这里是0
address = 0x68       #  这个是你在第一步测试你的传感器时看到那个数字

# Now wake the 6050 up as it starts in sleep mode
bus.write_byte_data(address, power_mgmt_1, 0)

# 传感器数据获取
def data_sensor():
	# 注意下面全局变量的定义
    global x_t
    global y_t
    global z_t
    global x_a
    global y_a
    global z_a
    global x_r
    global y_r
    print ("--------------gyro data--------------")
    print ("-------------------------------------")

    gyro_xout = read_word_2c(0x43)
    gyro_yout = read_word_2c(0x45)
    gyro_zout = read_word_2c(0x47)

    x_t = (gyro_xout / 131)
    y_t = (gyro_yout / 131)
    z_t = (gyro_zout / 131)
    print ("----------------accel data-----------------")
    print ("----------------------------------------------")

    accel_xout = read_word_2c(0x3b)
    accel_yout = read_word_2c(0x3d)
    accel_zout = read_word_2c(0x3f)

    accel_xout_scaled = accel_xout / 16384.0
    accel_yout_scaled = accel_yout / 16384.0
    accel_zout_scaled = accel_zout / 16384.0
    
    x_a = accel_xout / 16384.0
    y_a = accel_yout / 16384.0
    z_a = accel_zout / 16384.0
    

    print ("---------------x,y rotation data----------------")
    print ("x rotation: " , get_x_rotation(accel_xout_scaled, accel_yout_scaled, accel_zout_scaled))
    print ("y rotation: " , get_y_rotation(accel_xout_scaled, accel_yout_scaled, accel_zout_scaled))

# 动态绘图
def dongtai():
	# 这里主要用x轴方向的陀螺仪数据做测试，其他几个数据类似
    data_x_t = []  # 纵坐标表示x轴方向陀螺仪值，也是data_sensor()函数里面的x_t = (gyro_xout / 131)
    data_y_t = [] # 这是测试的第二个数据，y轴方向陀螺仪数据
    x_time = []  #x轴表示时间坐标
    
    for i in range(2000):
        plt.clf()
        data_sensor()  
        data_x_t.append(x_t)
        x_time.append(time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()))
        data_y_t.append(y_t)
#         data.append(z_t)
#         data.append(x_a)
#         data.append(y_a)
#         data.append(z_a)
#         data.append(x_r)
#         data.append(y_r)
#         fig,ax=plt.subplots()
        plt.subplot(221)
        plt.plot(x_time,data_x_t,label='xtuoluoyidata',color='g')
        plt.title('xtuoluoyidata')
        
        plt.subplot(222)
        plt.plot(x_time,data_y_t,label='ytuoluoyidata',color='b')
        plt.title('ytuoluoyidata')
        
        plt.subplot(223)
        plt.plot(x_time,data_y_t,label='ytuoluoyidata',color='k')
        plt.title('ytuoluoyidata')
        
        plt.subplot(224)
        plt.scatter(x_time,data_x_t,label='xtuoluoyidata',color='r')
        plt.title('ytuoluoyidata')
#       ax.set_title("x_jiasudu")
#       plt.draw()
#       time.sleep(1)
        plt.pause(0.1)
        
# 主函数入口
if __name__ == '__main__': 
    dongtai()
             

