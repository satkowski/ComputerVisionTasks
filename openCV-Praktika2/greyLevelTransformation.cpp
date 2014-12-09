#include <greyLevelTransformation.h>

Mat logTrans(int trackBarValue, Mat *input) {
    /*------------- initialization ------------*/
    Mat tempInput, output;
    double c;
    /*-----------------------------------------*/
    cvtColor(*input, tempInput, CV_BGR2GRAY);
    output = Mat(tempInput.rows, tempInput.cols, CV_8U);
    c = 255 / (log10(255 * exp(trackBarValue - 254)));

    for(int cY = 0; cY < tempInput.rows; cY++)
        for(int cX = 0; cX < tempInput.cols; cX++)
            output.at<uchar>(cY, cX) = c * log10(1 + (exp(trackBarValue) - 1) *
                                                      static_cast<double>(tempInput.at<uchar>(cY, cX)));

    return output;
}


Mat powerLawTrans(int trackBarValue, Mat* input) {
    /*------------- initialization ------------*/
    Mat tempInput, output;
    double c;
    /*-----------------------------------------*/
    cvtColor(*input, tempInput, CV_BGR2GRAY);
    output = Mat(tempInput.rows, tempInput.cols, CV_8U);

    for(int cY = 0; cY < tempInput.rows; cY++)
        for(int cX = 0; cX < tempInput.cols; cX++)
            output.at<uchar>(cY, cX) = c * log10(pow(static_cast<double>(tempInput.at<uchar>(cY, cX)), trackBarValue));

    return output;
}
