#include <histogrammFunctions.h>

vector<Mat> calcCDFandPDF(Mat *input) {
    /*------------- initialization ------------*/
    Mat tempInput;
    vector<Mat> output;
    Mat pdf, pdfImage;
    int histSize, histHeight, histWidth, binWidth;
    Mat cdf, cdfImage;
    float range[2] = { 0, 255 };
    const float* ranges[1];
    /*-----------------------------------------*/
    cvtColor(*input, tempInput, CV_BGR2GRAY);

    histSize = 256;
    *ranges = { range };

    histWidth = 512;
    histHeight = 400;
    binWidth = cvRound((double) histWidth/histSize);

    pdfImage = Mat(histHeight, histWidth, CV_8U, Scalar(0, 0, 0));
    cdfImage = Mat(histHeight, histWidth, CV_8U, Scalar(0, 0, 0));

    //Calculate histogram
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

    output.push_back(pdf);
    output.push_back(cdf);
    output.push_back(pdfImage);
    output.push_back(cdfImage);

    return output;
}

vector<Mat> histogrammEqualizationOneChannel(Mat *input) {
    /*------------- initialization ------------*/
    vector<Mat> image1, image2, output;
    Mat firstImage, pdfOutputImage, cdfOutputImage;
    Mat cdf1, cdf2;
    vector<Vec2b> cdfCompare;
    String filename = "../openCV-Praktika2/taskImages/spine.jpg";
    Vec2b actualValue;
    /*-----------------------------------------*/

    //Creating the image and testing if it is empty or not
    firstImage = imread(filename);
    while(firstImage.empty()) {
        printf("Cannot read the image %s\n", filename.c_str());
    }

    image1 = calcCDFandPDF(&firstImage);
    image2 = calcCDFandPDF(input);
    cdf1 = image1.at(1);
    cdf2 = image2.at(1);
    cdfOutputImage = Mat(firstImage.rows, firstImage.cols, CV_8U);

    for(int c1 = 0; c1 < cdf1.rows; c1++) {
        for(int c2 = 0; c2 < cdf2.rows; c2++) {
            if(cdf1.at<float>(c1) > cdf2.at<float>(c2) && (c2 - 1) != cdf2.rows) continue;
            cdfCompare.push_back(Vec2b { c1, c2 });
            break;
        }
    }

    cvtColor(firstImage, firstImage, CV_BGR2GRAY);
    equalizeHist(firstImage, pdfOutputImage);

    for(int cY = 0; cY < cdfOutputImage.rows; cY++) {
        for(int cX = 0; cX < cdfOutputImage.cols; cX++) {
            for(int c = 0; c < cdfCompare.size(); c++) {
                actualValue = cdfCompare.at(c);
                if(firstImage.at<uchar>(cY, cX) == actualValue.val[0])
                    cdfOutputImage.at<uchar>(cY, cX) = actualValue.val[1];
            }
        }
    }

    return vector<Mat> { image1.at(3), image2.at(3), firstImage, pdfOutputImage, cdfOutputImage };
}
