#include <main.h>

main( int argc, const char** argv ) {
    String sourceName, destinationName;
    Mat sourceImage, destinationImage;

    const char* keyMap;
    //Standard image that will be used if dont exist arguments
    keyMap = "{source       |   |../ComputerVision/62962.jpg              }"
             "{destination  |   |../ComputerVision/flagge-deutschland.jpf }";

    //Reading the Callingarguments
    CommandLineParser parser(argc, argv, keyMap);
    sourceName = parser.get<String>("source");
    destinationName = parser.get<int>("destination");

    //Creating the images and testing if it is empty or not
    sourceImage = imread(sourceImage);
    destinationImage = imread(destinationName);

    if(sourceImage.empty()) {
        printf("Cannot read the image %s\n", sourceImage.c_str());
        return -1;
    }
    if(destinationImage.empty()) {
        printf("Cannot read the image %s\n", destinationImage.c_str());
        return -1;
    }
}
