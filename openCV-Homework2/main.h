#ifndef MAIN_H
#define MAIN_H

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <set>
#include <stdio.h>

using namespace cv;

typedef std::multiset<Vec3f> rgbMap;

static void integralImage(int w, void* userdata);

//Main method
int main( int argc, const char** argv );

#endif // MAIN_H
