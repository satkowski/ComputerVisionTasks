#include <main.h>


int main( int argc, const char** argv ) {
    String filename;
    int filter;
    Mat image, outputImage;

    const char* keyMap;
    //Standard image that will be used if dont exist arguments
    keyMap = "{path     |   |../openCV-Homework2/prep-for-grilling.jpg }"
             "{filter   |1  |0                             }";

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
    image.copyTo(image);
    outputImage = integralImage(image);

    //Creating window for the outputimage
    namedWindow("Filtered Image", 0);
    imshow("Filtered Image", outputImage);

    waitKey();
    return 0;
}

Mat integralImage(Mat input) {
    int i, j, iMinusOne, jMinusOne, w = 2, wArea = (2*w + 1)*(2*w +1);
    Mat output;
    Mat areas(input.rows + 2*w, input.cols + 2*w, CV_32S);
    input.copyTo(output);
    
    for(int cY = 0; cY <= input.rows; cY++)
        areas.at<Vec3i>(cY, 0) = 0;
    for(int cX = 0; cX <= input.cols; cX++)
        areas.at<Vec3i>(0, cX) = 0;

//    for(int cY = -w; cY < (input.rows + w); cY++) {
//        if(cY < 0)                  { j = cY * -1;                  jMinusOne = j + 1; }
//        else if(cY >= input.rows)   { j = cY - input.rows - 1; jMinusOne = j - 1; }
//        else                        { j = cY;                       jMinusOne = cY - 1; }

//        for(int cX = -w; cX < (input.cols + w); cX++) {
//            if(cX <= 0)                  { i = cX * -1;                  iMinusOne = i + 1; }
//            else if(cX >= input.rows)   { i = cX - input.cols - 1; iMinusOne = i - 1; }
//            else                        { i = cX;                       iMinusOne = cX - 1; }

//            areas.at<Vec3i>(cY + w, cX + w) = areas.at<Vec3i>(jMinusOne, i) +
//                                              areas.at<Vec3i>(j, iMinusOne) -
//                                              areas.at<Vec3i>(jMinusOne, iMinusOne) +
//                                              static_cast<Vec3i>(input.at<Vec3b>(j, i));
//        }
//    }

    for(int cY = 0; cY < (input.rows + 2*w); cY++) {
        if(cY - w < 0)                  j = (cY - w) * -1;
        else if(cY - w >= input.rows)   j = (cY - w) - input.rows - 1;
        else                            j = (cY - w);

        for(int cX = 0; cX < (input.cols + 2*w); cX++) {
            if(cX - w<= 0)                  i = (cX - w) * -1;
            else if(cX - w >= input.rows)   i = (cX - w) - input.cols - 1;
            else                            i = (cX - w);

            areas.at<Vec3i>(cY, cX) = static_cast<Vec3i>(input.at<Vec3b>(j, i));
            if(cY - 1 >= 0 && cX - 1 >= 0)
                areas.at<Vec3i>(cY, cX) += areas.at<Vec3i>(cY - 1, cX) +
                                                   areas.at<Vec3i>(cY, cX - 1) -
                                                   areas.at<Vec3i>(cX  - 1, cX - 1);
            else if(cY  - 1 < 0 && cX  - 1 >= 0)
                areas.at<Vec3i>(cY, cX) += areas.at<Vec3i>(cY, cX - 1);
            else if(cY  - 1 >= 0 && cX  - 1 < 0)
                areas.at<Vec3i>(cY, cX) += areas.at<Vec3i>(cY- 1, cX);
        }
    }

//    for(int cY = w; cY < (input.rows + w); cY++) {
//        for(int cX = w; cX < (input.cols + w); cX++) {
//            output.at<Vec3b>(cY - w, cX - w) = areas.at<Vec3i>(cY, cX);
//            if(cX - w - 1 >= 0 && cY - w - 1 >= 0)
//                output.at<Vec3b>(cY - w, cX - w) += -areas.at<Vec3i>(cY, cX - w - 1) -
//                                                    areas.at<Vec3i>(cY - w - 1, cX) +
//                                                    areas.at<Vec3i>(cY - w -1, cX - w - 1);
//            else if(cX - w - 1 < 0 && cY - w - 1 >= 0)
//                output.at<Vec3b>(cY - w, cX -w) -= areas.at<Vec3i>(cY - w - 1, cX);
//            else if(cX - w - 1 >= 0 && cY - w - 1 < 0)
//                output.at<Vec3b>(cY - w, cX -w) -= areas.at<Vec3i>(cY, cX - w - 1);
//            output.at<Vec3b>(cY - w, cX - w) *= 1 / wArea;
//        }
//    }

    for(int cY = w; cY < (input.rows + w); cY++) {
        for(int cX = w; cX < (input.cols + w); cX++) {
            output.at<Vec3b>(cY - w, cX - w) = areas.at<Vec3i>(cY, cX);
            if(cX - w - 1 >= 0 && cY - w - 1 >= 0)
                output.at<Vec3b>(cY - w, cX - w) += -areas.at<Vec3i>(cY, cX - w - 1) -
                                                    areas.at<Vec3i>(cY - w - 1, cX) +
                                                    areas.at<Vec3i>(cY - w -1, cX - w - 1);
            else if(cX - w - 1 < 0 && cY - w - 1 >= 0)
                output.at<Vec3b>(cY - w, cX -w) -= areas.at<Vec3i>(cY - w - 1, cX);
            else if(cX - w - 1 >= 0 && cY - w - 1 < 0)
                output.at<Vec3b>(cY - w, cX -w) -= areas.at<Vec3i>(cY, cX - w - 1);
            output.at<Vec3b>(cY - w, cX - w) *= 1 / wArea;
        }
    }

    return output;
}
