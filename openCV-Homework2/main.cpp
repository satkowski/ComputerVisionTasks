#include <main.h>


int main( int argc, const char** argv ) {
    String filename;
    int filter, area = 0;
    Mat image, outputImage;


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

    //Creating windows for the images
    namedWindow("Filtered Image", 0);
    imshow("Filtered Image", outputImage);

    namedWindow("Original Image", 0);
    imshow("Original Image", image);

    //Adding the area trackbar to the window
    createTrackbar("Area Size", "Filtered Image", &area, 100, integralImage, &outputImage);

    waitKey();
    return 0;
}

static void integralImage(int w, void* userdata) {
    int i, j, wArea = (2*w + 1)*(2*w +1);
    Mat temp, output;
    Mat* input;
    input = static_cast<Mat*>(userdata);
    Mat areas(input->rows + 2*w, input->cols + 2*w, CV_64FC3);
    input->convertTo(temp, CV_64FC3);
    
    for(int cY = 0; cY <= input->rows; cY++)
        areas.at<Vec3d>(cY, 0) = 0;
    for(int cX = 0; cX <= input->cols; cX++)
        areas.at<Vec3d>(0, cX) = 0;

    for(int cY = 0; cY < (input->rows + 2*w); cY++) {
        //Calculate the mirror coordinate
        if(cY - w <= 0)                 j = (cY - w) * - 1;
        else if(cY - w >= input->rows)   j = (cY - w) - ((cY - w) - input->rows + 1);
        else                            j = (cY - w);

        for(int cX = 0; cX < (input->cols + 2*w); cX++) {
            //Calculate the mirror coordinate
            if(cX - w <= 0)                 i = (cX - w) * - 1;
            else if(cX - w >= input->cols)   i = (cX - w) - ((cX - w) - input->cols + 1);
            else                            i = (cX - w);

            //Add the color of the actual pixel
            areas.at<Vec3d>(cY, cX) = temp.at<Vec3d>(j, i);

            //Add the sum of the areas before if there are any
            if(cY - 1 >= 0 && cX - 1 >= 0)
                areas.at<Vec3d>(cY, cX) += areas.at<Vec3d>(cY - 1, cX) +
                                           areas.at<Vec3d>(cY, cX - 1) -
                                           areas.at<Vec3d>(cY  - 1, cX - 1);
            else if(cY  - 1 < 0 && cX  - 1 >= 0)
                areas.at<Vec3d>(cY, cX) += areas.at<Vec3d>(cY, cX - 1);
            else if(cY  - 1 >= 0 && cX  - 1 < 0)
                areas.at<Vec3d>(cY, cX) += areas.at<Vec3d>(cY - 1, cX);
        }
    }

    for(int cY = 0; cY < input->rows; cY++) {
        j = cY + w;
        for(int cX = 0; cX < input->cols; cX++) {
            i = cX + w;
            temp.at<Vec3d>(cY, cX) = areas.at<Vec3d>(j + w, i + w);

            if((j - w - 1) >= 0 && (i - w - 1) >= 0)
                temp.at<Vec3d>(cY, cX) = temp.at<Vec3d>(cY, cX) -
                                         areas.at<Vec3d>(j + w, i - w - 1) -
                                         areas.at<Vec3d>(j - w - 1, i + w) +
                                         areas.at<Vec3d>(j - w - 1, i - w - 1);
            else if((j - w - 1) < 0 && (i - w - 1) >= 0)
                temp.at<Vec3d>(cY, cX) -= areas.at<Vec3d>(j + w, i - w - 1);
            else if((j - w - 1) >= 0 && (i - w - 1) < 0)
                temp.at<Vec3d>(cY, cX) -= areas.at<Vec3d>(j - w - 1, i + w);
            temp.at<Vec3d>(cY, cX) /= wArea;
        }
    }

    temp.convertTo(output, CV_8UC3);
    imshow("Filtered Image", output);
}
