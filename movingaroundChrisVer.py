import serial
import picamera
import time

def encounterObj(objectColor):
	#assigns the distance the robot should stay around the obstacle
	if (objectColor == Red):
		radius = 3
	else:
		radius = 1

	port.write("~2632")

	while (angle < 90):
		#repeats until on the opposite side of object
		dist = sensor4Data
		while(dist >= radius + 0.2 or dist <= radius - 0.2):
			readSerial()
			dist = sensor4Data
			port.write("~6021")
		if (dist < radius):
			#turn 5 degrees right by running left motor forward
			port.write("~1052")
		else:
			#turn 5 degrees left by running right motor forward
			port.write("~4052")
			

	port.write("~1902")


def readSerial():
	port.write("~9000")
	if(port.in_waiting >= 20):
		inputData = str(port.read(31))
		print(inputData)
		dataList = inputData.split(",")
		global sensor1Data = dataList[1]
		global sensor2Data = dataList[2]
		global sensor3Data = dataList[3]
		global sensor4Data = dataList[4]
		heading = dataList[5]
		if (sensor1Data <= 40):#this compares sensor value to see if object is within 4 inches
			sensor1 = 1
		if (sensor2Data <= 40):
			ensor2 = 1
		if (sensor3Data <= 40):
			sensor3 = 1
		if (sensor4Data <= 40):
			sensor4 = 1		


port = serial.Serial("/dev/ttyAMA0",baudrate=9600,timeout=3.0)
#sensor1 is middle front sensor
#sensor2 is left front sensor
#sensor3 is right front sensor
#sensor4 is rear sensor

seeGreen = 0
global sensor1 = 0
global sensor2 = 0
global sensor3 = 0
global sensor4 = 0

while True:
	readSerial()
	port.write("~6201")
	time.sleep(1)
	port.write("~8000")
#camera = picamera.PiCamera()

for x in range(0,6): #does an inital sweep of surroundings looking for green objective
	#camera.capture('image.jpg')
	#objectType = identifyImage('image.jpg')
	if(objectType == green):
		seeGreen = 1
		break
	port.write("left 60")#left 60
while goal != 1:	
	while seeGreen != 1:
		readSerial()
		port.write("left 60")#left 60
		readSerial()
		if(sensor1 == 1 or sensor2 == 1 or sensor3 == 1):
			encounterObj(objectType)
			sensor1 = 0
			sensor2 = 0
			sensor3 = 0
			sensor4 = 0
	while seeGreen == 1:
		port.write("forward")#forward
		if(sensor1 == 1 or sensor2 == 1 or sensor3 == 1):
			encounterObj(objectType)
			sensor1 = 0
			sensor2 = 0
			sensor3 = 0
			sensor4 = 0
