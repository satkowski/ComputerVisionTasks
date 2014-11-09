#include <main.h>

int main( int argc, const char** argv ) {
    String filename;
    const char* keyMap;
    Vec3b intensity;

    //Standard image that will be used if dont exist arguments
    keyMap = "{1||../Bilder/62962.jpg}";

    //Reading the Callingarguments
    CommandLineParser parser(argc, argv, keyMap);
    filename = parser.get<String>("1");

    //Creating the image and testing if it is empty or not
    image = imread(filename);
    if(image.empty()) {
        printf("Cannot read the image %s\n", filename.c_str());
        return -1;
    }
    image.copyTo(output);

    //Creating window for the outputimage
    namedWindow("ColourCompare", 0);
    imshow("ColourCompare", output);

    //Creating window for the original image
    namedWindow("OriginalImage", 0);
    imshow("OriginalImage", image);

    //Adding the maxdistance trackbar to the window
    createTrackbar("Maximal Distance", "ColourCompare", &distanceThreshhold, 442, onDistanceTrackbar, &intensity);

    //Adding mous listener to the window
    setMouseCallback("OriginalImage", mouseListener, &intensity);

    waitKey();
    return 0;
}

static void onDistanceTrackbar(int, void* userdata) {
    Vec3b* intensity = static_cast<Vec3b*>(userdata);
    Vec3b newIntensity;
    double distance;
    //Loops to iterate over all pixels of the image
    for(int cY = 0; cY < image.rows; cY++) {
        for(int cX = 0; cX < image.cols; cX++) {
            newIntensity = image.at<Vec3b>(cY, cX);
            //Distance calculation
            distance = sqrt(
                        (intensity->val[0] - newIntensity.val[0]) * (intensity->val[0] - newIntensity.val[0]) +
                        (intensity->val[1] - newIntensity.val[1]) * (intensity->val[1] - newIntensity.val[1]) +
                        (intensity->val[2] - newIntensity.val[2]) * (intensity->val[2] - newIntensity.val[2]));
            //Condition if the field will be black or white
            if(distance <= distanceThreshhold)
                output.at<Vec3b>(cY, cX) = WHITE;
            else
                output.at<Vec3b>(cY, cX) = BLACK;
        }
    }
    findLargestArea();

    //Show the output image
    imshow("ColourCompare", output);
}

void mouseListener(int event, int x, int y, int, void* userdata) {
    //On left doubleclick the colour will be selected
    if(event == EVENT_LBUTTONDBLCLK) {
        Vec3b* intensity = static_cast<Vec3b*>(userdata);
        //Save the new colour
        *intensity = image.at<Vec3b>(y, x);
        userdata = intensity;
        //Activate the distance process
        onDistanceTrackbar(0, userdata);
    }
}

void findLargestArea() {
    int largestArea = 0, largestContourIndex = 0, childContourIndex;
    //Vector for storing contour
    vector<vector<Point> > contours;
    //Vector that save the image topology
    vector<Vec4i> hierarchy;
    Vec4i childValue;

    //Convert the output image in a b/w image
    Mat grayImage;
    cvtColor(output,grayImage,CV_BGR2GRAY);
    //Find the contours in the b/w image
    findContours(grayImage, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

    //Iterate through each contour
    for(uint contourIndex = 0; contourIndex< contours.size(); contourIndex++) {
        //Find the area of contour
        //Contour that are not for a WHITE area are negative
        double area = contourArea(contours[contourIndex],true) * -1;
        //Search only for contour that are larger than the actual largestArea
        if(area <= largestArea)
            continue;

        //Subract the child areas
        childValue = hierarchy.at(static_cast<unsigned>(contourIndex));
        if(childValue.val[2] != -1) {
            //Iterate through all child of the actual contour
            for(childContourIndex = childValue.val[2]; childContourIndex != -1; childContourIndex = hierarchy.at(static_cast<unsigned>(childContourIndex)).val[0])
                area -= contourArea(contours[childContourIndex], true);
        }
        if(area > largestArea) {
            largestArea = area;
            //Store the index of largest contour
            largestContourIndex = contourIndex;
        }
    }
    //Draw the largest contour using previously stored index.
    drawContours(output, contours, largestContourIndex, Scalar(0, 0, 255), CV_FILLED, 8, hierarchy);
}
