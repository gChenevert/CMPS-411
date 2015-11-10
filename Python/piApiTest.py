import requests, json
import numpy as np
import cv2
import io
import time
import sys
import base64


url = "http://api411-orbiter54321.c9users.io/411Api/image/getObject"





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




#main method
capture = get_camera()

while(True):
#if(cv2.waitKey(1) & 0xFF == ord('q')):
   # break
        try:
            ret, src = capture.read()
        except:
            src = capture.read()
        if(src is not None):
                _, data = cv2.imencode('.jpg', src)
                img_str = base64.b64encode(data.tostring())
                data = json.dumps({'image':img_str.decode()})
                #data = json.dumps({'image':'test'})
                if(data is not None):
                        r = requests.post(url, data)
                        print (r.text)
