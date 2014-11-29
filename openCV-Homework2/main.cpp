#include <main.h>

int main( int argc, const char** argv ) {
    /*------------- initialization ------------*/
    String filename;
    int filter, windowSize = 0;
    Mat image, outputImage;
    Vec<void*, 2>* data;
    /*-----------------------------------------*/

    const char* keyMap;
    //Standard image that will be used if dont exist arguments
    keyMap = "{path     |   |../Bilder/prep-for-grilling.jpg }"
             "{filter   |1  |2                               }";

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

    data->val[0] = &filter;
    data->val[1] = &image;

    //Creating windows for the images
    namedWindow("Filtered Image", 0);
    imshow("Filtered Image", outputImage);

//    namedWindow("Original Image", 0);
//    imshow("Original Image", image);

    //Adding the area trackbar to the window
    createTrackbar("Window Size", "Filtered Image", &windowSize, (image.cols < image.rows) ? image.cols / 2 - 1 : image.rows / 2 - 1, filterSelect, data);

    waitKey();
    return 0;
}

static void filterSelect(int windowSize, void* userdata) {
    /*------------- initialization ------------*/
    Vec<void*, 3>* data;
    int* filter;
    Mat* image;
    Mat outputImage;
    /*-----------------------------------------*/
    //Cast all variables
    data = static_cast<Vec<void*, 3>*>(userdata);
    filter = static_cast<int*>(data->val[0]);
    image = static_cast<Mat*>(data->val[1]);

    //Decesion which filter should be choosed
    switch(*filter) {
    case 0:
        outputImage = meanFilter(windowSize, image);
        break;
    case 1:
        outputImage = medianFilter(windowSize, image);
        break;
    case 2:
        outputImage = sobelFilter(image);
        break;
    default:
        outputImage = meanFilter(windowSize, image);
        break;
    }

    //Show the new filtered image
    imshow("Filtered Image", outputImage);
}
