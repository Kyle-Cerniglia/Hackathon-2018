import serial
import picamera

port = serial.Serial("\dev\ttyAMA0",baudrate=9600,timeout=3.0)
#sensor1 is middle front sensor
#sensor2 is left front sensor
#sensor3 is right front sensor
#sensor4 is rear sensor

seeGreen = 0
sensor1 = 0
sensor2 = 0
sensor3 = 0
sensor4 = 0

while True
	readSerial()
	
'''
camera = picamera.PiCamera()
for x in range(0,6): #does an inital sweep of surroundings looking for green objective
	camera.capture('image.jpg')
	objectType = identifyImage('image.jpg')
	if(objectType == green):
		seeGreen = 1
		break
	port.write("left 60")
while goal != 1:	
	while seeGreen != 1:
		port.write("left 60")
		if(sensor1 == 1 or sensor2 == 1 or sensor3 == 1):
			port.write("right 63")
	while seeGreen == 1:
		port.write("forward")
		if(sensor1 == 1 or sensor2 == 1 or sensor3 == 1):
			port.write("right 63")
'''						
						
def readSerial():
	inputData = port.read(20)
	print(inputData)
	dataList = inputData.split(",")
	sensor1Data = dataList[0]
	sensor2Data = dataList[1]
	sensor3Data = dataList[2]
	sensor4Data = dataList[3]
	heading = dataList[4]
	if (sensor1Data <= 40):#this compares sensor value to see if object is within 4 inches
		sensor1 = 1
	if (sensor2Data <= 40):
		sensor2 = 1
	if (sensor3Data <= 40):
		sensor3 = 1
	if (sensor4Data <= 40):
		sensor4 = 1