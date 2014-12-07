#ifndef MAIN_H
#define MAIN_H

#include "opencv2/highgui/highgui.hpp"

#include <stdio.h>

#include <filter.h>
#include <harrisDetector.h>

using namespace cv;

//Set the filter and the trackbar
void setFilter(int& filter, int& windowSize, void* data, Mat& image, Mat& outputImage);
//Trackbar method that select wich filter was choosed
static void filterSelect(int windowSize, void* userdata);

//Main method
int main( int argc, const char** argv );

#endif // MAIN_H
