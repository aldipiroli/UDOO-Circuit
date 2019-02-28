#LIBRARY
import smbus
import time
import serial
import string

#REG REF
I2C_ACCBUS		= 3
I2C_SLAVEADDR 		= 0x1E	#SA1=0 SA0=0

I2C_STATUS		= 0x00
I2C_OUTXMSB		= 0x01
I2C_OUTXLSB		= 0x02
I2C_OUTYMSB		= 0x03
I2C_OUTYLSB		= 0x04
I2C_OUTZMSB		= 0x05
I2C_OUTZLSB		= 0x06
I2C_FSETUP		= 0x09
I2C_TRIGCFG		= 0x0A
I2C_SYSMOD		= 0x0B
I2C_INTSOURCE		= 0x0C
I2C_WHOAMI		= 0x0D
I2C_XYZDATACFG		= 0x0E
I2C_HPFILTERCUTOFF 	= 0x0F
I2C_CTRLREG1		= 0x2A
I2C_CTRLREG2		= 0x2B
I2C_CTRLREG3		= 0x2C
I2C_CTRLREG4		= 0x2D
I2C_CTRLREG5		= 0x2E
I2C_OFFX		= 0x2F
I2C_OFFY		= 0x30
I2C_OFFZ		= 0x31
I2C_TEMP		= 0x51

ConvToG = 8192

#CREATE ISTANCE
Arduino = serial.Serial('/dev/ttyMCC',9600)
Arduino.flush()

I2C_Acc = smbus.SMBus(I2C_ACCBUS)

#READ REG FROM I2C BUS
XYZDataCFG = I2C_Acc.read_byte_data(I2C_SLAVEADDR, I2C_XYZDATACFG)	#+-2g (+-0.244mg/LSB)
print "XYZDataCFG:", XYZDataCFG

Temp= I2C_Acc.read_byte_data(I2C_SLAVEADDR, I2C_TEMP)			#(0.96C/LSB)
print "Temp:", Temp

#ACTIVE MODE
I2C_Acc.write_byte_data(I2C_SLAVEADDR, I2C_CTRLREG1, 1)

out_file = open("/var/www/html/gpx_data/gpx_data.txt","w")
out_file.close()

LastLtd = 0.0
LastLnd = 0.0

#CORE LOOP
while True:

	RawAccX = float((I2C_Acc.read_byte_data(I2C_SLAVEADDR, I2C_OUTXMSB) << 8 ) | I2C_Acc.read_byte_data(I2C_SLAVEADDR, I2C_OUTXLSB))
	
	if RawAccX > 17000:
		RawAccX = (65536 - RawAccX)*(-1)

	DataAccXG = float(RawAccX/ConvToG)

	DataAccXdegree = int(DataAccXG*90/2)		
	
	StrData = str(DataAccXdegree) + '#'
	Arduino.write(StrData)
	
	ltd = Arduino.readline()
	lnd = Arduino.readline()	
	vel = Arduino.readline()
	
	f_ltd = float(ltd)	
	f_lnd = float(lnd)	
	f_vel = float(vel)

	f_ltd = f_ltd / 1000000
	f_lnd = f_lnd / 1000000	

	ltd = str(f_ltd)
	lnd = str(f_lnd)
	vel = str(f_vel)
	deg = str(DataAccXdegree)

	print "Ltd:" + ltd
	print "Lnd:" + lnd
	print "Vel:" + vel
	print "Deg:" + deg + "\n"

	out_file = open("/var/www/html/gpx_data/gpx_data.txt","a")

	out_file.write(ltd)
	out_file.write(',')
	out_file.write(lnd)
	out_file.write(',')
	out_file.write(vel)
	out_file.write(',')
	out_file.write(deg)
	out_file.write("\n")

	out_file.close()
	
	time.sleep(0.1)

Arduino.close()
