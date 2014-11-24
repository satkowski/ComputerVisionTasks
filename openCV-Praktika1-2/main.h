#ifndef MAIN_H
#define MAIN_H

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"

#include <stdio.h>
#include <iostream>

#define PI 3.14159265

using namespace cv;

static Mat SOURCE_IMAGE;
static Mat DESTINATION_IMAGE(512, 512, CV_8UC4);
static vector<Point2f> SOURCE_POINTS_ORIG, SOURCE_POINTS_SHIFT;
static const int ALPHA = 100;
static int U0, V0;
static Mat INTRINSIC_CAM_PAR_MAT = Mat_<double>(3, 3);

//Method that calculate the image inside the camera
static void calcCameraImage(int, void* userdata);
//Method that calculate the camera position
static void calcCameraPosition(int, void* userdata);
//Method that calculate the affine transformation
static void calcAffineTransformation(int, void* userdata);
//Method that calculate the new rotation matrix
static void calcRotationMatrix(int value, void* userdata);

//Main method
int main( int argc, const char** argv );

#endif // MAIN_H
