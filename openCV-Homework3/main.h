#ifndef MAIN_H
#define MAIN_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <stdio.h>
#include <iostream>

using namespace cv;

//
Mat fourier(Mat& input);

//Main method
int main( int argc, const char** argv );

#endif // MAIN_H