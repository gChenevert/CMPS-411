import numpy as np
import cv2
import io
import time

#global variables
up_red1 = np.array([160,100,100], np.uint8)
up_red2 = np.array([179,255,255], np.uint8)

#constants
CV_PI = 3.14159265359

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
		with picamera.PiCamera() as camera:
			camera.capture(data, format='jpeg')
		data = np.fromstring(data.getvalue(), dtype=np.uint8)
		# Decode the image data and return an OpenCV image.
		image = cv2.imdecode(data, 1)
		# Save captured image for debugging.
		cv2.imwrite(config.DEBUG_IMAGE, image)
		# Return the captured image data.
		return image

def get_camera():	
	# Camera to use for capturing images.
	# Use this code for capturing from the Pi camera:
	 return OpenCVCapture()
	# Use this code for capturing from a webcam:
	# import webcam
	# return cv2.VideoCapture(0)

	
def main():
    # Display the resulting frame
    #cv2.imshow('frame',gray)
    src = None
    gray = None
    bw = None
    dst = None
    contours = None
    approx = None


    #use if running on USB camera
    capture = get_camera()
    color = False
    i = 0
    
    while (True):
        if(cv2.waitKey(1) & 0xFF == ord('q')):
            break
        
        ret, src = capture.read()
        if(True):
            if(not color):
                #in C++ 6 = CV_BGR2GRAY
                gray = cv2.cvtColor(src, cv2.COLOR_BGR2GRAY)
            else:
                hsv_image = None
                lower_red_hue_range = None
                #in c++ 40 = COLOR_BGR2HSV
                cv2.cvtColor(0, 0, 40) 
                cv2.inRange(hsv_image, up_red1, up_red2, lower_red_hue_range)
                gray = lower_red_hue_range

            bw = cv2.blur(gray, (3,3))
            bw = cv2.Canny(gray, 80,240,3)
            cv2.imshow("bw", bw)


            #find contours
            im, contours, hierarchy = cv2.findContours(bw, 0, 2)
            dst = src


            for i in range(0, len(contours)):
                ret = cv2.arcLength(contours[i], True)*.02
                approx = cv2.approxPolyDP(contours[i], ret, True)
                if(abs(cv2.contourArea(contours[i])) < 100 or cv2.isContourConvex(approx)):
                    continue
                if(len(approx) == 3):
                    print "Triangle\n"
                elif(len(approx) >= 4 and len(approx) <= 6):
                    vtc = len(approx)

                    #cos = None
                    #for j in range(2, vtc+1):
                        #cos.push(angle(approx[j%vtc], approx[j - 2], approx[j - 1]))

                    #cos.sort()

                    if(vtc == 4):
                        print "Rect\n"
                    elif(vtc == 5):
                        print "Penta\n"
                    elif(vtc == 6):
                        print "Hexa\n"

                else:
                    area = cv2.contourArea(contours[i])
                    x,y,w,h = cv2.boundingRect(contours[i])
                    radius = w / 2
                    if(abs(1 - (w/h)) <= .2 and abs(1 - (area / (CV_PI * (radius * radius)))) < .2):
                        print "Circle\n"
                    

        src = capture.read()


    cv2.waitKey(0)
    cv2.destroyAllWindows()



def angle(pt1, pt2, pt0):
    dx1 = pt1.x - pt0.x
    dy1 = pt1.y - pt0.y
    dx2 = pt2.x - pt0.x
    dy2 = pt2.y - pt0.y
    return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);

    
main()
