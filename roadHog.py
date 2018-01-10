#import statements
import cv2
import numpy as np
#import matplotlib.pyplot as plt
import serial
import time

#connect to arduino
ser = serial.Serial('COM10', 9600, timeout = 0)

#get video capture from webcam
cap = cv2.VideoCapture(0)

#cascades
cascPath = ["Haar Cascade/Nav/goCascade.xml", "Haar Cascade/Nav/stopCascade.xml", 
			"Haar Cascade/Nav/leftCascade.xml", "Haar Cascade/Nav/newRightCascade.xml"]
			
goCascade = cv2.CascadeClassifier(cascPath[0])			
stopCascade = cv2.CascadeClassifier(cascPath[1])			
leftCascade = cv2.CascadeClassifier(cascPath[2])
rightCascade = cv2.CascadeClassifier(cascPath[3])

while True:
	ret, frame = cap.read()
	gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
	
	go = goCascade.detectMultiScale(gray)
	stop = stopCascade.detectMultiScale(gray)
	left = leftCascade.detectMultiScale(gray)
	right = rightCascade.detectMultiScale(gray)
	
	for(x,y,w,h) in go:
		cv2.rectangle(frame, (x,y), (x+w, y+h), (0,255,255), 2)
		print("Go")
		ser.write("<1>")
		
	for(x,y,w,h) in stop:
		cv2.rectangle(frame, (x,y), (x+w, y+h), (255,0,255), 2)
		print("Stop")
		ser.write("<0>")	
		
	for(x,y,w,h) in left:
		cv2.rectangle(frame, (x,y), (x+w, y+h), (255,255,0), 2)
		print("Turn Left")
		ser.write("<3>")	
	
	for(x,y,w,h) in right:
		cv2.rectangle(frame, (x,y), (x+w, y+h), (0,0,255), 2)
		print("Right")
		ser.write("<2>")
	
	frame = cv2.flip(frame, 1)
	cv2.imshow('RoadHogV1', frame)

	try:
		print(ser.readline())
	except ser.SerialTimeOutException:
		print('Data could not be read')
		time.sleep(1)
		
	if cv2.waitKey(1) & 0xff == ord('q'):
		break
		
cap.release()
cv2.destroyAllWindows		
		
