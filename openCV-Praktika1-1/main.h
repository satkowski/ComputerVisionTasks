#ifndef MAIN_H
#define MAIN_H

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"

#include <stdio.h>

using namespace cv;

static Mat SOURCEIMAGE, DESTINATIONIMAGE;
static vector<Point2f> SOURCEPOINTS;

//Creates and show the solution image
void showFinal(Mat destination,Mat source);

//What will happend if the mouse trigger an event
void mouseListener(int event, int x, int y, int, void* userdata);

//Main method
int main( int argc, const char** argv );

#endif // MAIN_H
