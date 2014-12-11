#ifndef HISTOGRAMMFUNCTIONS_H
#define HISTOGRAMMFUNCTIONS_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>

using namespace cv;

vector<Mat> histogrammEqualizationOneChannel(Mat *input);
vector<Mat> calcCDFandPDF(Mat* input, bool convert);
vector<Mat> histogrammEqualizationThreeChannel(Mat *input, bool useAverage);


#endif // HISTOGRAMMFUNCTIONS_H
