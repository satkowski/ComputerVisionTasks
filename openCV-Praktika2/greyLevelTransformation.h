#ifndef GREYLEVELTRANSFORMATION_H
#define GREYLEVELTRANSFORMATION_H

#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>

using namespace cv;

//Method that calculate the log transformation of a given input
Mat logTrans(int trackBarValue, Mat* input);
//Method that calculate the power law transformation of a given input
Mat powerLawTrans(int trackBarValue, Mat* input);
//Method that calculate an own transformation
Mat myChoiceTrans(int trackBarValue, Mat* input);

#endif // GREYLEVELTRANSFORMATION_H
