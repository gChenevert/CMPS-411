
#include "stdafx.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/video/video.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <stdio.h>
#include "../packages/OpenCV.2.4.10/build/native/include/opencv2/core/core.hpp"

using namespace std;
using namespace cv;


/** Function Headers */
void detectAndDisplay(Mat frame);
void homography(Mat img_object, Mat img_scene);

/** Global variables */
String face_cascade_name = "haarcascades/haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "haarcascades/haarcascade_eye_tree_eyeglasses.xml";
string smile_cascade_name = "haarcascades/haarcascade_smile.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
CascadeClassifier smile_cascade;
String window_name = "Capture - Face detection";


/**Colors*/
Scalar up_red1 = cv::Scalar(160, 100, 100);
Scalar up_red2 = cv::Scalar(179, 255, 255);



/**
* Helper function to find a cosine of angle between vectors
* from pt0->pt1 and pt0->pt2
*/
static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}
/**
* Helper function to display text in the center of a contour
*/
void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour)
{
	int fontface = cv::FONT_HERSHEY_SIMPLEX;
	double scale = 0.4;
	int thickness = 1;
	int baseline = 0;

	cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
	cv::Rect r = cv::boundingRect(contour);

	cv::Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
	cv::rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), CV_RGB(255, 255, 255), CV_FILLED);
	cv::putText(im, label, pt, fontface, scale, CV_RGB(0, 0, 0), thickness, 8);
}


/** @function main */
int main(void)
{
	//*******************************************old testing********************************
	//VideoCapture capture;
	//Mat frame;

	////-- 1. Load the cascades
	//if (!face_cascade.load(face_cascade_name)){ printf("--(!)Error loading face cascade\n"); return -1; };
	//if (!eyes_cascade.load(eyes_cascade_name)){ printf("--(!)Error loading eyes cascade\n"); return -1; };
	//if (!eyes_cascade.load(smile_cascade_name)){ printf("--(!)Error loading smile cascade\n"); return -1; };

	////-- 2. Read the video stream
	//capture.open(0);
	//if (!capture.isOpened()) { printf("--(!)Error opening video capture\n"); return -1; }

	//while (capture.read(frame))
	//{
	//	if (frame.empty())
	//	{
	//		printf(" --(!) No captured frame -- Break!");
	//		break;
	//	}

	//	//-- 3. Apply the classifier to the frame
	//	detectAndDisplay(frame);

	//	int c = waitKey(10);
	//	if ((char) c == 27) { break; } // escape
	//}
	//return 0;
	bool testing = false;
	if (testing){
		VideoCapture capture;
		Mat frame;
		Mat img_object = imread("Resources/ref.jpg");
		Mat img_scene;

		capture.open(0);
		if (!capture.isOpened()) { printf("--(!)Error opening video capture\n"); return -1; }
		if (capture.read(frame))
		{
			if (frame.empty())
			{
				printf(" --(!) No captured frame -- Break!");
				//break;
			}
			//cvtColor(frame, img_scene, COLOR_BGR2GRAY);
			homography(img_object, frame);
		}
		waitKey(0);
	}
	else
	{
		cv::Mat src;
		cv::Mat gray;
		cv::Mat bw;
		cv::Mat dst;
		std::vector<std::vector< cv::Point>> contours;
		std::vector<cv::Point> approx;

		VideoCapture capture(0);
		int q;
		auto color = false;
		while (cvWaitKey(30) != 'q')
		{
			capture >> src;
			

			if (true)
			{
				// Convert to grayscale
				if (!color){
					cv::cvtColor(src, gray, CV_BGR2GRAY);
				}
				else{
					Mat hsv_image;
					Mat lower_red_hue_range;
					cvtColor(src, hsv_image, COLOR_BGR2HSV);
					cv::inRange(hsv_image, up_red1, up_red2, lower_red_hue_range);
					gray = lower_red_hue_range;
				}

				// Use Canny instead of threshold to catch squares with gradient shading
				blur(gray, bw, Size(3, 3));
				cv::Canny(gray, bw, 80, 240, 3);
				cv::imshow("bw", bw);
				//cv::bitwise_not(bw, bw);

				// Find contours
				cv::findContours(bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

				src.copyTo(dst);
				system("cls");
				for (int i = 0; i < contours.size(); i++)
				{

					cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);

					if (std::fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
						continue;

					if (approx.size() == 3)
					{
						setLabel(dst, "TRI", contours[i]);    // Triangles
						cout << "Triangle\n";
					}
					else if (approx.size() >= 4 && approx.size() <= 6)
					{

						int vtc = approx.size();


						vector<double> cos;
						for (int j = 2; j < vtc + 1; j++)
							cos.push_back(angle(approx[j%vtc], approx[j - 2], approx[j - 1]));


						std::sort(cos.begin(), cos.end());


						double mincos = cos.front();
						double maxcos = cos.back();

						if (vtc == 4){
							setLabel(dst, "RECT", contours[i]);
							cout << "Rect\n";
						}
						else if (vtc == 5){
							setLabel(dst, "PENTA", contours[i]);
							cout << "Penta\n";
						}
						else if (vtc == 6){
							setLabel(dst, "HEXA", contours[i]);
							cout << "Hexa\n";
						}
					}
					else
					{
						double area = cv::contourArea(contours[i]);
						cv::Rect r = cv::boundingRect(contours[i]);
						int radius = r.width / 2;

						if (std::abs(1 - ((double) r.width / r.height)) <= 0.2 &&
							std::abs(1 - (area / (CV_PI * (radius*radius)))) <= 0.2){
							setLabel(dst, "CIR", contours[i]);
							cout << "Circle\n";
						}
					}
				}
				
				cv::imshow("src", src);
				cv::imshow("dst", dst);

			}
			else
			{
				break;
			}
		}
	}
}

void homography(Mat img_object, Mat img_scene)
{


	if (!img_object.data || !img_scene.data)
	{
		std::cout << " --(!) Error reading images " << std::endl; exit(-1);
	}

	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 400;

	SurfFeatureDetector detector(minHessian);

	std::vector<KeyPoint> keypoints_object, keypoints_scene;

	detector.detect(img_object, keypoints_object);
	detector.detect(img_scene, keypoints_scene);

	//-- Step 2: Calculate descriptors (feature vectors)
	SurfDescriptorExtractor extractor;

	Mat descriptors_object, descriptors_scene;

	extractor.compute(img_object, keypoints_object, descriptors_object);
	extractor.compute(img_scene, keypoints_scene, descriptors_scene);

	//-- Step 3: Matching descriptor vectors using FLANN matcher
	FlannBasedMatcher matcher;
	std::vector< DMatch > matches;
	matcher.match(descriptors_object, descriptors_scene, matches);

	double max_dist = 0; double min_dist = 100;

	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < descriptors_object.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}

	printf("-- Max dist : %f \n", max_dist);
	printf("-- Min dist : %f \n", min_dist);

	//-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
	std::vector< DMatch > good_matches;

	for (int i = 0; i < descriptors_object.rows; i++)
	{
		if (matches[i].distance < 3 * min_dist)
		{
			good_matches.push_back(matches[i]);
		}
	}

	Mat img_matches;
	drawMatches(img_object, keypoints_object, img_scene, keypoints_scene,
		good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	//-- Localize the object
	std::vector<Point2f> obj;
	std::vector<Point2f> scene;

	for (int i = 0; i < good_matches.size(); i++)
	{
		//-- Get the keypoints from the good matches
		obj.push_back(keypoints_object[good_matches[i].queryIdx].pt);
		scene.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
	}

	Mat H = findHomography(obj, scene, CV_RANSAC);

	//-- Get the corners from the image_1 ( the object to be "detected" )
	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(0, 0); obj_corners[1] = cvPoint(img_object.cols, 0);
	obj_corners[2] = cvPoint(img_object.cols, img_object.rows); obj_corners[3] = cvPoint(0, img_object.rows);
	std::vector<Point2f> scene_corners(4);

	perspectiveTransform(obj_corners, scene_corners, H);

	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	line(img_matches, scene_corners[0] + Point2f(img_object.cols, 0), scene_corners[1] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[1] + Point2f(img_object.cols, 0), scene_corners[2] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[2] + Point2f(img_object.cols, 0), scene_corners[3] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[3] + Point2f(img_object.cols, 0), scene_corners[0] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);

	//-- Show detected matches
	imshow("Good Matches & Object detection", img_matches);
}

/** @function detectAndDisplay */
void detectAndDisplay(Mat frame)
{
	std::vector<Rect> faces;
	Mat frame_gray;

	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	//-- Detect faces
	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

	for (size_t i = 0; i < faces.size(); i++)
	{
		Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
		ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);

		Mat faceROI = frame_gray(faces[i]);
		std::vector<Rect> eyes;

		//-- In each face, detect eyes
		eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

		for (size_t j = 0; j < eyes.size(); j++)
		{
			Point eye_center(faces[i].x + eyes[j].x + eyes[j].width / 2, faces[i].y + eyes[j].y + eyes[j].height / 2);
			int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
			circle(frame, eye_center, radius, Scalar(255, 0, 0), 4, 8, 0);
		}

		std::vector<Rect> smiles;
		smile_cascade.detectMultiScale(faceROI, smiles, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(3, 3));

		for (size_t j = 0; j < smiles.size(); j++)
		{
			Point smileCenter(faces[i].x + smiles[j].x + smiles[j].width / 2, faces[i].y + smiles[j].y + smiles[j].height / 2);
			int radius = cvRound((smiles[j].width + smiles[j].height)*0.25);
			circle(frame, smileCenter, radius, Scalar(255, 0, 0), 4, 8, 0);

		}

	}
	//-- Show what you got
	imshow(window_name, frame);
}

