#include <main.h>

int main( int argc, const char** argv ) {
    String sourceName, destinationName;
    Mat sourceImage, destinationImage;

    const char* keyMap;
    //Standard image that will be used if dont exist arguments
    keyMap = "{source       |   |../Bilder/62962.jpg              }"
             "{destination  |   |../Bilder/flagge-deutschland.jpg }";

    //Reading the Callingarguments
    CommandLineParser parser(argc, argv, keyMap);
    sourceName = parser.get<String>("source");
    destinationName = parser.get<String>("destination");

    //Creating the images and testing if it is empty or not
    sourceImage = imread(sourceName);
    destinationImage = imread(destinationName);

    if(sourceImage.empty()) {
        printf("Cannot read the image %s\n", sourceName.c_str());
        return -1;
    }
    if(destinationImage.empty()) {
        printf("Cannot read the image %s\n", destinationName.c_str());
        return -1;
    }

    //Creating windows for the images
    namedWindow("Source Image", 0);
    imshow("Source Image", sourceImage);

    namedWindow("Destination Image", 0);
    imshow("Destination Image", destinationImage);

    waitKey();
    return 0;
}
