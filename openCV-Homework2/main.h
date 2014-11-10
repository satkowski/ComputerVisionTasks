#ifndef MAIN_H
#define MAIN_H

#include "opencv2/highgui/highgui.hpp"

#include <stdio.h>

#include <filter.h>

using namespace cv;

//Trackbar method that select wich filter was choosed
static void filterSelect(int windowSize, void* userdata);

//Main method
int main( int argc, const char** argv );

#endif // MAIN_H
