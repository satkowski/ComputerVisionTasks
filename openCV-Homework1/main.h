#ifndef MAIN_H
#define MAIN_H

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <stdio.h>

using namespace cv;
//Constants for the Color of the image
static const Vec3b BLACK (0, 0, 0);
static const Vec3b WHITE (255, 255, 255);

Mat image, output;
int distanceThreshhold = 0;

//What should happen if the trackbar will be moved
static void onDistanceTrackbar(int, void* userdata);
//What will happend if the mouse trigger an event
void mouseListener(int event, int x, int y, int, void* userdata);
//Search and colour the largest region in the output image
void findLargestArea();

//Main method
int main( int argc, const char** argv );

#endif // MAIN_H
