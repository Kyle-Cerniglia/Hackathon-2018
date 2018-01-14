
import pyttsx3
import random
def speak(category):
	#random.seed('int')
	file=open(category+'.txt','r')
	lines=file.readlines()
	engine=pyttsx3.init()
	engine.setProperty('rate',120)  #120 words per minute
	engine.setProperty('volume',0.9) 
	num = random.randint(0,len(lines)-1)
	print(num)
	engine.say(lines[num])
	engine.runAndWait()

speak('unknown')
