#ifndef MAIN_H
#define MAIN_H

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"

#include <stdio.h>

using namespace cv;

static Mat SOURCEIMAGE;

//Method that calculate the camera position
static void calcCameraPosition(int, void* userdata);
//Method that calculate the affine transformation
static void calcAffineTransformation(int, void* userdata);

//Main method
int main( int argc, const char** argv );

#endif // MAIN_H
