import numpy as np
import cv2
import sys
import base64

up_red1 = np.array([140,80,80], np.uint8)
up_red2 = np.array([179,255,255], np.uint8)

up_blue1 = np.array([90,82,89], np.uint8)
up_blue2 = np.array([105,177,177], np.uint8)

shapes = [];

#dictionary for switch case and the relevant methods
def angle_cos(p0, p1, p2):
    d1, d2 = (p0-p1).astype('float'), (p2-p1).astype('float')
    return abs( np.dot(d1, d2) / np.sqrt( np.dot(d1, d1)*np.dot(d2, d2) ) )
	
def square(cnt):
    shapes = [];
    print("squares")
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
    }
color2 = {
    "red" : up_red2,
    "blue": up_blue2,
    }


#generic find object method
def find_object(img, objectType, colorLower, colorUpper):
    #color detection, effectively only shows objects that are the correcct color
    hsv_image = None
    colorShift = None
    hsv_image = cv2.cvtColor(img,40)
    colorShift = cv2.inRange(hsv_image, colorLower, colorUpper)
    #old get_square method
    #img = cv2.GaussianBlur(img,(5,5),0)
    bw = cv2.blur(colorShift, (3,3))
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



print("got here")
if(sys.argv[3] is not None):
    print("pls")

src = cv2.imread(sys.argv[3])
print("found src")
#data = base64.b64encode(sys.argv[3])
#almost = cv2.imdecode(data)
shapes = find_object(src, sys.argv[1], color1[sys.argv[2]], color2[sys.argv[2]])

           


