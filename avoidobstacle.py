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

port.write("revR63")

while (angle < 90):
    #repeats until on the opposite side of object
    port.read("getdist")
    while(dist >= radius + 0.2 or dist <= radius - 0.2):
        port.read("getsensor4")
        port.write("f")
    oldAngle = angle
    if (dist < radius):
        #turn 5 degrees right by running left motor forward
        port.write("fL5")
    else:
        port.write("fR5")
        #turn 5 degrees left by running right motor forward

port.write("fL90")