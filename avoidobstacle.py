import serial
port = serial.Serial("\dev\ttyAMA0",baudrate=9600,timeout=3.0)

#sensor1 is middle front sensor
#sensor2 is left front sensor
#sensor3 is right front sensor
#sensor4 is rear sensor

#assigns the distance the robot should stay around the obstacle
if ("""object is red"""):
    radius = 3
else:
    radius = 1

readSerial()
dist = sensor1data

while(dist > radius):
    port.write("~60.22") #until the robot gets too close the object move forward

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
        port.write("~60.22")

port.write("~1902") #turns right 90 deg
