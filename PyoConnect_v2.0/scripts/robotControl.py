import serial
import time
port = serial.Serial("/dev/ttyAMA0",baudrate=9600,timeout=3.0)

def onPoseEdge(pose, edge):
	#myo.unlock("hold")
	time.sleep(.5)
	control=1;
	if(myo.isLocked()):
		myo.unlock("timed")
	if(myo.getArm()=="right"):
		if(pose=="waveOut"):
			pose="waveRight"
		elif(pose=="waveIn"):
			pose="waveLeft"
	else:
		if(pose=="waveOut"):
			pose="waveLeft"
		elif(pose=="waveIn"):
			pose="waveRight"
	#print("onPoseEdge: "+pose+", "+edge)
	if(pose=="waveRight" and edge=='on'):
		port.write("~0152")
		#print("R")
	elif(pose=="waveLeft" and edge=='on'):
		port.write("~3152")
		#print("L")
	elif(pose=='fist' and edge=='on'):
		port.write("~6053")
		#print("F")
	elif(pose=='rest' and edge=='on'):
		port.write("~8000")
		#print("H")

control=0

#myo.onWear(arm,xdirection)=onPoseEdge(myo.getPoseSide(), myo.getEdge())

