#include <histogrammFunctions.h>

vector<Mat> calcCDFandPDF(Mat *input, bool convert) {
    /*------------- initialization ------------*/
    Mat tempInput;
    Mat pdf, pdfImage;
    int histSize, histHeight, histWidth, binWidth;
    Mat cdf, cdfImage;
    float range[2] = { 0, 255 };
    const float* ranges[1];
    /*-----------------------------------------*/
    //Is it necessary to convert the input
    if(convert)
        cvtColor(*input, tempInput, CV_BGR2GRAY);
    else
       tempInput = *input;

    //Set values
    histSize = 256;
    *ranges = { range };
    histWidth = 512;
    histHeight = 400;
    binWidth = cvRound((double) histWidth/histSize);

    pdfImage = Mat(histHeight, histWidth, CV_8U, Scalar(0, 0, 0));
    cdfImage = Mat(histHeight, histWidth, CV_8U, Scalar(0, 0, 0));

    //Calculate pdf
    calcHist(&tempInput, 1, 0, Mat(), pdf, 1, &histSize, ranges, true, false);
    //Plot the pdf
    normalize(pdf, pdf, 0, pdfImage.rows, NORM_MINMAX, -1, Mat());
    //Draw every line between the points
    for(int c = 1; c < histSize; c++)
      line(pdfImage, Point(binWidth * (c - 1), histHeight - cvRound(pdf.at<float>(c - 1))),
                     Point(binWidth * c,       histHeight - cvRound(pdf.at<float>(c))),
                     Scalar(255, 0, 0),
                     2, 8, 0);

    cdf = Mat(pdf.rows, pdf.cols, CV_32F);

    //Calculate cdf
    cdf.at<float>(0) = pdf.at<float>(0);
    for(int cY = 1; cY < cdf.rows; cY++)
        cdf.at<float>(cY) = cdf.at<float>(cY - 1) + pdf.at<float>(cY);

    //Plot the cdf
    normalize(cdf, cdf, 0, cdfImage.rows, NORM_MINMAX, -1, Mat());
    //Draw every line between the points
    for(int c = 1; c < histSize; c++)
        line(cdfImage, Point(binWidth * (c - 1), histHeight - cvRound(cdf.at<float>(c - 1))),
                       Point(binWidth * c,       histHeight - cvRound(cdf.at<float>(c))),
                       Scalar(255, 0, 0),
                       2, 8, 0);

    //Create the vector and return it
    return vector<Mat> { pdf, cdf, pdfImage, cdfImage };
}

vector<Mat> histogrammEqualizationOneChannel(Mat *input) {
    /*------------- initialization ------------*/
    vector<Mat> image1, image2;
    Mat firstImage, pdfOutputImage, cdfOutputImage;
    Mat cdf1, cdf2, lut;
    String filename = "../openCV-Praktika2/taskImages/spine.jpg";
    /*-----------------------------------------*/

    //Creating the image and testing if it is empty or not
    firstImage = imread(filename);
    while(firstImage.empty()) {
        printf("Cannot read the image %s\n", filename.c_str());
    }

    //Calculate the cdf, pdf and the responding graphs
    image1 = calcCDFandPDF(&firstImage, true);
    image2 = calcCDFandPDF(input, true);
    cdf1 = image1.at(1);
    cdf2 = image2.at(1);
    lut = Mat(cdf1.rows, 1 , CV_8U);
    cdfOutputImage = Mat(firstImage.rows, firstImage.cols, CV_8U);

    //Histogram matching of both cdf
    for(int c1 = 0; c1 < cdf1.rows; c1++) {
        for(int c2 = 0; c2 < cdf2.rows; c2++) {
            //A match if the cdf1 value is bigger than the cdf2 value or its the last value in cdf2
            if(cdf1.at<float>(c1) > cdf2.at<float>(c2) && (c2 + 1) != cdf2.rows) continue;
            lut.at<uchar>(c1) = c2;
            break;
        }
    }

    //Calculate the pdf image
    cvtColor(firstImage, firstImage, CV_BGR2GRAY);
    equalizeHist(firstImage, pdfOutputImage);

    //Calculate the cdf image
    for(int cY = 0; cY < cdfOutputImage.rows; cY++)
        for(int cX = 0; cX < cdfOutputImage.cols; cX++)
            cdfOutputImage.at<uchar>(cY, cX) = lut.at<uchar>(firstImage.at<uchar>(cY, cX));

    //Create the vector and return it
    return vector<Mat> { image1.at(3), image2.at(3), firstImage, pdfOutputImage, cdfOutputImage };
}


vector<Mat> histogrammEqualizationThreeChannel(Mat *input) {
    /*------------- initialization ------------*/
    String filename = "../openCV-Praktika2/taskImages/rocks.jpg";
    vector<Mat> image1Split, image2Split;
    vector<Mat> image1[3], image2[3];
    Mat firstImage, cdfOutputImage;
    Mat cdf1[3], cdf2[3], lut[3];
    bool lutSet[3];
    /*-----------------------------------------*/

    //Creating the image and testing if it is empty or not
    firstImage = imread(filename);
    while(firstImage.empty()) {
        printf("Cannot read the image %s\n", filename.c_str());
    }

    //Split the different color channel
    split(firstImage, image1Split);
    split(*input, image2Split);

    //Calculate the cdf, pdf and the responding graphs for all channel
    for(int c = 0; c < 3; c++) {
        image1[c] = calcCDFandPDF(&image1Split[c], false);
        image2[c] = calcCDFandPDF(&image2Split[c], false);
        cdf1[c] = image1[c].at(1);
        cdf2[c] = image2[c].at(1);
        lut[c] = Mat(cdf1[c].rows, 1 , CV_8U);
    }
    cdfOutputImage = Mat(firstImage.rows, firstImage.cols, CV_8UC3);

    //Histogram matching of both cdf for all channel
    for(int c1 = 0; c1 < cdf1[0].rows; c1++) {
        lutSet[0] = false;
        lutSet[1] = false;
        lutSet[2] = false;
        for(int c2 = 0; c2 < cdf2[0].rows; c2++) {
            for(int channel = 0; channel < 3; channel++) {
                //A match if the cdf1 value is bigger than the cdf2 value or its the last value in cdf2
                if((cdf1[channel].at<float>(c1) <= cdf2[channel].at<float>(c2) || (c2 + 1) == cdf2[channel].rows)
                    && !lutSet[channel]) {
                    lut[channel].at<uchar>(c1) = c2;
                    lutSet[channel] = true;
                }
            }
            if(lutSet[0] && lutSet[1] && lutSet[2]) break;
        }
    }
    //Calculate the cdf image
    for(int cY = 0; cY < cdfOutputImage.rows; cY++)
        for(int cX = 0; cX < cdfOutputImage.cols; cX++)
            cdfOutputImage.at<Vec3b>(cY, cX) = Vec3b { lut[0].at<uchar>(firstImage.at<Vec3b>(cY, cX).val[0]),
                                                       lut[1].at<uchar>(firstImage.at<Vec3b>(cY, cX).val[1]),
                                                       lut[2].at<uchar>(firstImage.at<Vec3b>(cY, cX).val[2]) };

    //Create the vector and return it
    return vector<Mat> { image1[0].at(3), image2[0].at(3),
                         image1[1].at(3), image2[1].at(3),
                         image1[2].at(3), image2[2].at(3),
                         firstImage, cdfOutputImage };
}
