#include <filter.h>


Mat sharpening(Mat* input) {
    /*------------- initialization ------------*/
    Mat unsharpMask;
    Mat tempInput, tempOutput, output;
    Vec3d actualValue;
    /*-----------------------------------------*/
    input->convertTo(tempInput, CV_64FC3);
    unsharpMask = (Mat_<double>(3, 3) << -1, -2,  -1,
                                         -2,  12, -2,
                                         -1, -2,  -1);
    tempOutput = Mat(tempInput.rows, tempInput.cols, CV_64FC3);

    for(int cY = 1; cY < tempInput.rows - 1; cY++) {
        for(int cX = 1; cX < tempInput.cols - 1; cX++) {
            actualValue = 0;
            for(int wY = -1; wY <= 1; wY++)
                for(int wX = -1; wX <= 1; wX++)
                    tempOutput.at<Vec3d>(cY - 1, cX - 1) += unsharpMask.at<double>(wY + 1, wX + 1) *
                                                            tempInput.at<Vec3d>(cY + wY, cX + wX);
            tempOutput.at<Vec3d>(cY - 1, cX - 1) /= 4;
        }
    }
    tempOutput.convertTo(output, CV_8UC3);
    std::cout << output << std::endl;
    output += *input;
    return output;
}


Mat medianThreshold(int threshold, Mat* input) {

}
