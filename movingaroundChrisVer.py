import serial
import picamera
import time

def encounterObj(objectColor):
    #assigns the distance the robot should stay around the obstacle
	if (objectColor == Red):
		radius = 3
	else:
		radius = 1

	readSerial()
	dist = sensor1data

	while(dist > radius):
		port.write("~6022") #until the robot gets too close the object move forward

	port.write("~2632") #turn right 63 deg

	while (angle < 90): #repeats until on the opposite side of object
		readSerial()
		dist = sensor4data
		if not(radius - 0.2 < dist < radius + 0.2):
			if (dist <= radius): #if the robot is too close to the obstacle
				port.write("~1052")
			else (dist >= radius): #if the robot is too far from the obstacle
				port.write("~4052")
		else:
			port.write("~6022")

	port.write("~1902") #turns right 90 deg


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
'''
#test for read and write
while True:
	readSerial()
	port.write("~6201")
	time.sleep(1)
	port.write("~8000")
'''	
camera = picamera.PiCamera()

for x in range(0,6): #does an inital sweep of surroundings looking for green objective
	camera.capture('image.jpg')
	objectType = identifyImage('image.jpg')
	if(objectType == green):
		seeGreen = 1
		break
	port.write("left 60")#left 60
while goal != 1:
	while seeGreen != 1:
		port.write("6203")#forward 2 seconds
		readSerial()
		fcounter += 1
		if(sensor1 == 1 or sensor2 == 1 or sensor3 == 1):
			port.write("8000")#stop
			camera.capture('image.jpg')
			objectType = identifyImage('image.jpg')
			encounterObj(objectType)
			sensor1 = 0
			sensor2 = 0
			sensor3 = 0
			sensor4 = 0
		elif(fcounter == 5):
			for x in range(0,6): #does an inital sweep of surroundings looking for green objective
				camera.capture('image.jpg')
				objectType = identifyImage('image.jpg')
				if(objectType == green):
					seeGreen = 1
					break
				port.write("left 60")#left 60
		if(seeGreen == 1):
			break
	while seeGreen == 1:
		port.write("6203")#forward 2 seconds 
		readSerial()
		if(sensor1 == 1 or sensor2 == 1 or sensor3 == 1):
			port.write("8000")#stop
			camera.capture('image.jpg')
			objectType = identifyImage('image.jpg')
			if(objectType == green):
				goal = 1;
				break
			encounterObj(objectType)
			sensor1 = 0
			sensor2 = 0
			sensor3 = 0
			sensor4 = 0