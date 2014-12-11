#ifndef MAIN_H
#define MAIN_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <stdio.h>
#include <iostream>

#include <greyLevelTransformation.h>
#include <histogrammFunctions.h>
#include <filter.h>

using namespace cv;

//Set the task and the trackbar
void setTask(int& task, int& trackBarValue, void* data, Mat& image, Mat& outputImage);
//Select the right method
void taskSelect(int taskBarValue, void* userdata);

//Main method
int main( int argc, const char** argv );


#endif // MAIN_H
