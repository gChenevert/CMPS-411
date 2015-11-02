#!/usr/bin/env python

'''
Simple "Square Detector" program.

Loads several images sequentially and tries to find squares in each image.
'''

from picamera.array import PiRGBArray
from picamera import PiCamera
import time
import numpy as np
import cv2

#PiCamera
class OpenCVCapture(object):
    def read(self):
        data = io.BytesIO()
        with picamera.PiCamera() as camera:
            camera.capture(data, format='jpeg')
        data = np.fromstring(data.getValue(), dtype=np.unit8)
        image = cv2.imdecode(data, 1)
        return image

def get_camera():
    # Camera to use for capturing images.
    # Use this code for capturing from the Pi camera:
    return OpenCVCapture()
    # Use this code for capturing from a webcam:
    # import webcam
    # return cv2.VideoCapture(0)

def angle_cos(p0, p1, p2):
    d1, d2 = (p0-p1).astype('float'), (p2-p1).astype('float')
    return abs( np.dot(d1, d2) / np.sqrt( np.dot(d1, d1)*np.dot(d2, d2) ) )

def find_squares(img):
	img = cv2.GaussianBlur(img, (5, 5), 0)
	squares = []
	for gray in cv2.split(img):
		for thrs in range(0, 255, 26):
			if thrs == 0:
				bin = cv2.Canny(gray, 0, 50, apertureSize=5)
				bin = cv2.dilate(bin, None)
			else:
				retval, bin = cv2.threshold(gray, thrs, 255, cv2.THRESH_BINARY)
			bin, contours, hierarchy = cv2.findContours(bin, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
			for cnt in contours:
				cnt_len = cv2.arcLength(cnt, True)
				cnt = cv2.approxPolyDP(cnt, 0.02*cnt_len, True)
				if len(cnt) == 4 and cv2.contourArea(cnt) > 1000 and cv2.isContourConvex(cnt):
					cnt = cnt.reshape(-1, 2)
					max_cos = np.max([angle_cos( cnt[i], cnt[(i+1) % 4], cnt[(i+2) % 4] ) for i in range(4)])
					if max_cos < 0.1:
						if cv2.contourArea(cnt) < 285200:
							squares.append(cnt)
							M = cv2.moments(cnt)
							centerX = int(M['m10']/M['m00'])
							centerY = int(M['m01']/M['m00'])
							if(centerX > 350):
								print ("move right")
							elif(centerX < 290):
								print ("move left")
							else:
								print ("centered")
							return squares
	return squares

if __name__ == '__main__':
	from glob import glob
	camera = PiCamera()
	#camera.resolution = (640, 480)
	#camera.framerate = 12
	rawCapture = PiRGBArray(camera, size=(640, 480))
	time.sleep(1.0)
	for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
		img = frame.array
		squares = find_squares(img)
		cv2.drawContours( img, squares, -1, (0, 255, 0), 3 )
		cv2.imshow('squares', img)
		key = cv2.waitKey(1) & 0xFF
		rawCapture.truncate(0)
		if key == ord('a'):
			break
		#cv2.destroyAllWindows()
