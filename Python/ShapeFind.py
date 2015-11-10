#from picamera.array import PiRGBArray
#from picamera import PiCamera
import numpy as np
import cv2
import io
import time
import sys
#global variables
#up_red1 = np.array([0,100,0], np.uint8)
#up_red2 = np.array([50,255,255], np.uint8)
up_red1 = np.array([140,80,80], np.uint8)
up_red2 = np.array([179,255,255], np.uint8)

up_blue1 = np.array([90,82,89], np.uint8)
up_blue2 = np.array([105,177,177], np.uint8)

up_green1 = np.array([29,86,6], np.uint8)
up_green2 = np.array([64,255,255], np.uint8)

up_orange1 = np.array([0,136,164], np.uint8)
up_orange2 = np.array([20,255,255], np.uint8)
shapes = [];


#PiCamera Class
class OpenCVCapture(object):
        def read(self):
                """Read a single frame from the camera and return the data as an OpenCV
		image (which is a numpy array).
		"""
		# This code is based on the picamera example at:
		# http://picamera.readthedocs.org/en/release-1.0/recipes1.html#capturing-to-an-opencv-object
		# Capture a frame from the camera.
                data = io.BytesIO()
                with PiCamera() as camera:
                        camera.capture(data, format='jpeg')
                data = np.fromstring(data.getvalue(), dtype=np.uint8)
		# Decode the image data and return an OpenCV image.
                image = cv2.imdecode(data, 1)
		# Return the captured image data.
                return image

def get_camera():
        #comment out the picamera inport lines as needed
	# Camera to use for capturing images.
	# Use this code for capturing from the Pi camera:
	# return OpenCVCapture()
	# Use this code for capturing from a webcam:
	 return cv2.VideoCapture(0)



#dictionary for switch case and the relevant methods
def angle_cos(p0, p1, p2):
    d1, d2 = (p0-p1).astype('float'), (p2-p1).astype('float')
    return abs( np.dot(d1, d2) / np.sqrt( np.dot(d1, d1)*np.dot(d2, d2) ) )
	
def square(cnt):
    shapes = [];
    print (len(cnt));
    if len(cnt) == 4 and cv2.contourArea(cnt) > 1000 and cv2.isContourConvex(cnt):
        
        cnt = cnt.reshape(-1,2)
        max_cos = np.max([angle_cos( cnt[i], cnt[(i+1) % 4], cnt[(i+2) % 4] ) for i in range(4)])
        print(max_cos)
        if max_cos < 0.1 and cv2.contourArea(cnt) < 285200:
            shapes.append(cnt)
            M = cv2.moments(cnt)
            centerX = int(M['m10']/M['m00'])
            centerY = int(M['m01']/M['m00'])
            if(centerX > 350):
                print ("move right")
            elif(centerX < 290):
                print ("move left")
            else:
                print ("centered")
            return shapes
    return shapes

def circle(cnt):
    shapes = [];
    if len(cnt) > 5 and cv2.contourArea(cnt) > 10 and cv2.isContourConvex(cnt):
        print("got it")
        cnt = cnt.reshape(-1,2)
        max_cos = np.max([angle_cos( cnt[i], cnt[(i+1) % 4], cnt[(i+2) % 4] ) for i in range(4)])
        print (max_cos)
        if cv2.contourArea(cnt) < 285200:
            shapes.append(cnt)
            M = cv2.moments(cnt)
            centerX = int(M['m10']/M['m00'])
            centerY = int(M['m01']/M['m00'])
            if(centerX > 350):
                print ("move right")
            elif(centerX < 290):
                print ("move left")
            else:
                print ("centered")
            return shapes
    return shapes


#definition table, effectively works like a switch case switch[what to switch on](parameter to send to function)	
switch = {
    "square" : square,
    "circle" : circle,
    }
color1 = {
    "red" : up_red1,
    "blue":up_blue1,
    "green":up_green1,
    "orange":up_orange1,
    }
color2 = {
    "red" : up_red2,
    "blue": up_blue2,
    "green":up_green2,
    "orange":up_orange2,
    }

#generic find object method
def find_object(img, objectType, colorLower, colorUpper):
    #color detection, effectively only shows objects that are the correcct color
    hsv_image = None
    colorShift = None
    hsv_image = cv2.cvtColor(img,40)
    colorShift = cv2.inRange(hsv_image, colorLower, colorUpper)
    #colorShift = cv2.erode(colorShift, None, iterations=2)
    #colorShift = cv2.dilate(colorShift, None, iterations=2)
    #old get_square method
    bw = cv2.GaussianBlur(colorShift,(5,5),0)
    #bw = cv2.blur(colorShift, (3,3))
    bw = cv2.Canny(bw, 80, 240, 3)
    cv2.imshow("bw", bw)
    for gray in cv2.split(bw):
        for thrs in range(0,255,26):
            if thrs == 0:
                bin = cv2.Canny(gray, 0, 50, apertureSize=5)
                bin = cv2.dilate(bin, None)
            else:
                retval, bin = cv2.threshold(gray, thrs, 255, cv2.THRESH_BINARY)
            bin, contours, hierarchy = cv2.findContours(bin, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
            for cnt in contours:
                cnt_len = cv2.arcLength(cnt, True)
                cnt = cv2.approxPolyDP(cnt, 0.02*cnt_len, True)
                return switch[objectType](cnt)


    

#main method
capture = get_camera()
print("start")
while(True):
    if(cv2.waitKey(1) & 0xFF == ord('q')):
        break
    try:
        ret, src = capture.read()
    except:
        src = capture.read()
    if(src is not None):
        toFind = sys.argv[1]
        print("hello")
        shapes = find_object(src, sys.argv[1], color1[sys.argv[2]], color2[sys.argv[2]])
        cv2.drawContours(src, shapes, -1, (0, 255, 0), 3)
        cv2.imshow('squares', src)
cv2.destroyAllWindows()
