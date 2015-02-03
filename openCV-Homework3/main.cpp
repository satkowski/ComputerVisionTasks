#include <main.h>


int main( int argc, const char** argv ) {
    /*------------- initialization ------------*/
    String filename;
    Mat image, outputImage;
    /*-----------------------------------------*/

    const char* keyMap;
    //Standard image that will be used if dont exist arguments
    keyMap = "{path     |   |../Bilder/txt.png }";

    //Reading the Callingarguments
    CommandLineParser parser(argc, argv, keyMap);
    filename = parser.get<String>("path");

    //Creating the image and testing if it is empty or not
    image = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
    if(image.empty()) {
        printf("Cannot read the image %s\n", filename.c_str());
        return -1;
    }
    image.copyTo(outputImage);

    //Creating windows for the images
    namedWindow("Original Image", 0);
    imshow("Original Image", image);
    namedWindow("Fourier Image", 0);
    imshow("Fourier Image", task(image));

    waitKey();
    return 0;
}

Mat task(Mat& input) {
    /*------------- initialization ------------*/
   Mat tempInput, complexDFTInput, tempOutput, output;
    Vector<float>* intensities = new Vector<float>[2];
    float maxValue = 0;
    float maxIntensity = 0;
    float intensityDiff = std::numeric_limits<float>::max();
    int intensityCounter = 0;
    Mat thresholdMat;
    Point maxPoint(0,0);
    /*-----------------------------------------*/

    //Convert the Mat values to floats and make a fourier transformation
    input.convertTo(tempInput, CV_32F);
    dft(tempInput, complexDFTInput, DFT_COMPLEX_OUTPUT);

    intensities[0] = Vector<float>();
    intensities[1] = Vector<float>();
    //Add all values from the fourier transformation to a vector as complex-float numbers
    for(int cRow = 0; cRow < complexDFTInput.rows; cRow++) {
        for(int cCol = 0; cCol < complexDFTInput.cols; cCol++) {
            intensities[0].push_back(complexDFTInput.at<std::complex<float> >(cRow, cCol).imag());
            intensities[1].push_back(complexDFTInput.at<std::complex<float> >(cRow, cCol).real());
        }
    }
    //Search the maxima in the whole vector to sort it afterwards
    for(unsigned int c = 0; c < intensities[1].size(); c++) {
        maxIntensity = maxIntensity >= intensities[1][c] ? maxIntensity : intensities[1][c];
    }
//    std::sort(intensities.begin(), intensities.end(), sortVector);
    //Count the intensities that are greate than the max * 0.045
//    for(intensityCounter = 0; intensities[intensityCounter] > maxIntensity * 0.045; intensityCounter++);
//    intensityCounter--;
    for(unsigned int c = 0; c < intensities[1].size(); c++) {
        if(intensityDiff > intensities[1][c] - maxIntensity * 0.045 &&
           (intensities[1][c] - maxIntensity * 0.045) * -1 < 0) {
            intensityDiff = intensities[1][c] - maxIntensity * 0.045;
            intensityCounter = c;
        }
    }

    //Fill the thresholdMat with 255 and the size and type of the complexDFTInput
    complexDFTInput.copyTo(thresholdMat);
    thresholdMat.setTo(255);

    for (int cRow = 0; cRow < complexDFTInput.rows; cRow++) {
        for (int cCol = 0; cCol < complexDFTInput.cols; cCol++) {
            //Set those of the thresholdMat 0 that are below the intensity in imaginary and real
            if (complexDFTInput.at<std::complex<float> >(cRow, cCol).imag() < intensities[0][intensityCounter] &&
                complexDFTInput.at<std::complex<float> >(cRow, cCol).real() < intensities[1][intensityCounter])
                thresholdMat.at<std::complex<float> >(cRow, cCol) = 0;
            //Set those of the trehsholdMat the same value as the complexDFTInput value if that are great in imaginary or real
            else
                thresholdMat.at<std::complex<float> >(cRow, cCol) = complexDFTInput.at<std::complex<float> >(cRow, cCol);

            //Search for the maximum value in complexDFTInput and save its point
            if (complexDFTInput.at<float>(cRow, cCol) > maxValue) {
                maxValue = complexDFTInput.at<float>(cRow, cCol);
                maxPoint = Point(cRow, cCol);
            }
        }
    }

    //Set the maxPoint also 0
    thresholdMat.at<std::complex<float> >(maxPoint) = 0;

    //Make a inverse fourier transformation and convert it back
    idft(thresholdMat, tempOutput, DFT_REAL_OUTPUT|DFT_SCALE);
    tempOutput.convertTo(tempOutput, CV_8U);
    output = combineImages(input, tempOutput);

    return output;
}

bool sortVector(float left, float right) {
    return left >= right;
}

Mat combineImages(Mat firstImage, Mat secondImage) {
    /*------------- initialization ------------*/
    Mat combinedImage;
    int sumSecI = 0, maxSecI = 0, percSecI = 0;
    /*-----------------------------------------*/
    combinedImage = Mat::zeros(firstImage.size(), CV_8UC3);

    for(int cRow = 0; cRow < firstImage.rows; cRow++) {
        for(int cCol = 0; cCol < firstImage.cols; cCol++) {
            sumSecI += static_cast<int>(secondImage.at<uchar>(cRow, cCol));
            //Save the highest value
            if(static_cast<int>(secondImage.at<uchar>(cRow, cCol)) > maxSecI)
                maxSecI = static_cast<int>(secondImage.at<uchar>(cRow, cCol));
        }
    }
    percSecI = static_cast<int>(maxSecI * 0.075);
    for(int cRow = 0; cRow < firstImage.rows; cRow++) {
        for(int cCol = 0; cCol < firstImage.cols; cCol++) {
            //Make the pixel red
            if(secondImage.at<unsigned char>(cRow, cCol) > percSecI)
                combinedImage.at<Vec3b>(cRow,cCol) = Vec3b(10,10,255);
            //Use the color of the first image
            else
                combinedImage.at<Vec3b>(cRow,cCol) = Vec3b(firstImage.at<char>(cRow, cCol),
                                                           firstImage.at<char>(cRow, cCol),
                                                           firstImage.at<char>(cRow, cCol));
        }
    }
    return combinedImage;
}
