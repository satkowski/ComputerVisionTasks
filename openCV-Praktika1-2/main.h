#ifndef MAIN_H
#define MAIN_H

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"

#include <stdio.h>

using namespace cv;

static Mat SOURCEIMAGE;
static Mat DESTINATIONIMAGE(512, 512, CV_8UC4);
static vector<Point> SOURCEPOINTS;
static const int ALPHA = 10;
static int U0, V0;

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
