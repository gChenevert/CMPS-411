#Basic i2c connection

import smbus
b = smbus.SMBus(1)

DEVICE = 0x08
while(0 == 0):
    var = input("Value to Write: ")
    b.write_byte_data(DEVICE, 1, int(var))
