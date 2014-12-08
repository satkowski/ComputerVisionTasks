#include <main.h>

int main( int argc, const char** argv ) {
    /*------------- initialization ------------*/
    String filename;
    int filter, windowSize = 0, gaussianFilter = 1;
    Mat image, outputImage;
    Vec<void*, 4>* data;
    /*-----------------------------------------*/

    const char* keyMap;
    //Standard image that will be used if dont exist arguments
    keyMap = "{path     |   |../Bilder/haus.jpg }"
             "{filter   |1  |0                               }";

    //Reading the Callingarguments
    CommandLineParser parser(argc, argv, keyMap);
    filename = parser.get<String>("path");
    filter = parser.get<int>("filter");

    //Creating the image and testing if it is empty or not
    image = imread(filename);
    if(image.empty()) {
        printf("Cannot read the image %s\n", filename.c_str());
        return -1;
    }
    image.copyTo(outputImage);

    //Filling the data pointer
    data->val[0] = &filter;
    data->val[1] = &image;
    data->val[2] = &windowSize;
    data->val[3] = &gaussianFilter;

    //Creating windows for the images
    namedWindow("Original Image", 0);
    imshow("Original Image", image);
    namedWindow("Filtered Image", 0);

    setFilter(filter, windowSize, gaussianFilter, data, image, outputImage);

    waitKey();
    return 0;
}

void setFilter(int& filter, int& windowSize, int& gaussianFilter, void* data, Mat& image, Mat& outputImage) {
    switch(filter) {
    case 4:
        //Adding a gaussian range trackbar to the window
        createTrackbar("Gaussian range", "Filtered Image", &gaussianFilter, (image.cols < image.rows) ? image.cols / 2 - 1 : image.rows / 2 - 1, filterSelect, data);
        //Adding a trackbar to the window
        createTrackbar("1 / value", "Filtered Image", &windowSize, 10000, filterSelect, data);
        break;
    case 5:
        outputImage = sobelFilter(&image);
        break;
    case 3:
        //Adding a gaussian range trackbar to the window
        createTrackbar("Gaussian range", "Filtered Image", &gaussianFilter, (image.cols < image.rows) ? image.cols / 2 - 1 : image.rows / 2 - 1, filterSelect, data);
    default:
        //Adding the area trackbar to the window
        createTrackbar("Window Size", "Filtered Image", &windowSize, (image.cols < image.rows) ? image.cols / 2 - 1 : image.rows / 2 - 1, filterSelect, data);
        break;
    }

    //Show the image
    imshow("Filtered Image", outputImage);
}

static void filterSelect(int, void* userdata) {
    /*------------- initialization ------------*/
    Vec<void*, 4>* data;
    int* filter;
    int* windowSize;
    int* gaussianFilter;
    Mat* image;
    Mat outputImage;
    /*-----------------------------------------*/
    //Cast all variables
    data = static_cast<Vec<void*, 4>*>(userdata);
    filter = static_cast<int*>(data->val[0]);
    image = static_cast<Mat*>(data->val[1]);
    windowSize = static_cast<int*>(data->val[2]);
    gaussianFilter = static_cast<int*>(data->val[3]);

    //Decesion which filter should be choosed
    switch(*filter) {
    case 0:
        outputImage = meanFilter(*windowSize, image);
        break;
    case 1:
        outputImage = medianFilterTwoSets(*windowSize, image);
        break;
    case 2:
        outputImage = medianFilterOneSet(*windowSize, image);
        break;
    case 3:
        outputImage = harrisCornerDetector(*windowSize, *gaussianFilter, 0, image);
        break;
    case 4:
        outputImage = harrisCornerDetector(*windowSize, *gaussianFilter, 1, image);
        break;
    }

    //Show the new filtered image
    imshow("Filtered Image", outputImage);
}
