#include <main.h>

int main( int argc, const char** argv ) {
    String sourceName, destinationName;
    vector<Point2f> destinationPoints;

    const char* keyMap;
    //Standard image that will be used if dont exist arguments
    keyMap = "{source       |   |../Bilder/flagge-deutschland.jpg }"
             "{destination  |   |../Bilder/62962.jpg }";

    //Reading the Callingarguments
    CommandLineParser parser(argc, argv, keyMap);
    sourceName = parser.get<String>("source");
    destinationName = parser.get<String>("destination");

    //Creating the images and testing if it is empty or not
    SOURCEIMAGE = imread(sourceName);
    DESTINATIONIMAGE = imread(destinationName);

    if(SOURCEIMAGE.empty()) {
        printf("Cannot read the image %s\n", sourceName.c_str());
        return -1;
    }
    if(DESTINATIONIMAGE.empty()) {
        printf("Cannot read the image %s\n", destinationName.c_str());
        return -1;
    }

    //Add the corners of the source image
    SOURCEPOINTS.push_back(Point2f(0,                0));
    SOURCEPOINTS.push_back(Point2f(0,                SOURCEIMAGE.rows));
    SOURCEPOINTS.push_back(Point2f(SOURCEIMAGE.cols, SOURCEIMAGE.rows));
    SOURCEPOINTS.push_back(Point2f(SOURCEIMAGE.cols, 0));

    //Creating windows for the images
    namedWindow("Source Image", 0);
    imshow("Source Image", SOURCEIMAGE);

    namedWindow("Destination Image", 0);
    imshow("Destination Image", DESTINATIONIMAGE);

    //Add the mousecallback
    setMouseCallback("Destination Image", mouseListener, &destinationPoints);


    waitKey();
    return 0;
}

void mouseListener(int event, int x, int y, int, void* userdata) {
    //On left doubleclick the points will be selected
    if(event == EVENT_LBUTTONDOWN) {
        vector<Point2f>* destinationPoints = static_cast<vector<Point2f>*>(userdata);
        destinationPoints->push_back(Point2f(x, y));
        circle(DESTINATIONIMAGE, Point(x, y), 7, Scalar(0, 0, 255), CV_FILLED);
        imshow("Destination Image", DESTINATIONIMAGE);

        if(destinationPoints->size() >= 4) {
            Mat homography, sourceWarped;
            //Deactivate the mouselistener
            setMouseCallback("Destination Image", NULL);

            //Create homography
            homography = findHomography(SOURCEPOINTS, *destinationPoints, 0);

            //Transform the source image
            warpPerspective(SOURCEIMAGE, sourceWarped, homography, DESTINATIONIMAGE.size());
            showFinal(DESTINATIONIMAGE, sourceWarped);
        }
    }
}

void showFinal(Mat destination,Mat source)
{
    Mat gray, grayInv, destinationFinal, sourceFinal, finalImage;

    //Creates an aquivalent gray image of the warded image
    cvtColor(source, gray, CV_BGR2GRAY);
    //Change the gray image to a binary image
    threshold(gray, gray, 0, 255, CV_THRESH_BINARY);
    //Invert the gray image
    bitwise_not(gray, grayInv);

    //Copy only those pixel that got an 1 in the mask
    destination.copyTo(destinationFinal, grayInv);
    source.copyTo(sourceFinal,gray);

    //Combine the two images
    finalImage = destinationFinal + sourceFinal;
    imshow("Destination Image", finalImage);
}
