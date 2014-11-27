#ifndef MAIN_H
#define MAIN_H

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"

#include <stdio.h>
#include <iostream>

#define PI 3.14159265

using namespace cv;

//Images
static Mat SOURCE_IMAGE;
static Mat CAMERA_IMAGE(512, 512, CV_8UC4);
static Mat AFFINE_IMAGE(512, 512, CV_8SC4);
//Corner points
static vector<Point2f> SOURCE_POINTS_ORIG;
static vector<Point2f> SOURCE_POINTS_SHIFT;
static vector<Point2f> CAMERA_IMAGE_POINTS;
//Other
static const int ALPHA = 100;
static int U0, V0;
static Mat INTRINSIC_CAM_PAR_MAT = Mat_<double>(3, 3);


//Method that fill all variables
void fillVariables(Mat& rotMat);

//Method that calculate all the transformation
static void onTrackbar(int, void* userdata);
//Method that calculate the new rotation matrix
static void onTrackbarRotMat(int, void* userdata);

//Method that calculate the image inside the camera
void calcCameraImage(int, void* userdata);
//Method that calculate the camera position
void calcCameraPosition(int, void* userdata);
//Method that calculate the affine transformation
void calcAffineTransformation(int, void* userdata);

//Main method
int main( int argc, const char** argv );

#endif // MAIN_H
