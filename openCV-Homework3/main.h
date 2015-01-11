#ifndef MAIN_H
#define MAIN_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <stdio.h>
#include <iostream>

using namespace cv;

//Method for the task
Mat task(Mat& input);
//Used to sort a vector properly
bool sortVector(float left, float right);
//Combine two images
Mat combineImages(Mat firstImage, Mat secondImage);

//Main method
int main( int argc, const char** argv );

#endif // MAIN_H
