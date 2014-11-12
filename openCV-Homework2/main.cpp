#include <main.h>

int main( int argc, const char** argv ) {
    String filename;
    int filter, windowSize = 0;
    Mat image, outputImage;
    Vec<void*, 2>* data;

    const char* keyMap;
    //Standard image that will be used if dont exist arguments
    keyMap = "{path     |   |../Bilder/prep-for-grilling.jpg }"
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

    data->val[0] = &filter;
    data->val[1] = &image;

    //Creating windows for the images
    namedWindow("Filtered Image", 0);
    imshow("Filtered Image", outputImage);

    namedWindow("Original Image", 0);
    imshow("Original Image", image);

    //Adding the area trackbar to the window
    createTrackbar("Window Size", "Filtered Image", &windowSize, 100, filterSelect, data);

    waitKey();
    return 0;
}

static void filterSelect(int windowSize, void* userdata) {
    //Cast all variables
    Vec<void*, 3>* data = static_cast<Vec<void*, 3>*>(userdata);
    int* filter = static_cast<int*>(data->val[0]);
    Mat* image = static_cast<Mat*>(data->val[1]);
    Mat outputImage;
    image->copyTo(outputImage);

    //Decesion which filter should be choosed
    switch(*filter) {
    case 0:
        meanFilter(windowSize, &outputImage);
        break;
    case 1:
        medianFilter(windowSize, &outputImage);
        break;
    default:
        meanFilter(windowSize, &outputImage);
        break;
    }

    //Show the new filtered image
    imshow("Filtered Image", outputImage);
}
