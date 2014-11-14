#include <filter.h>

void meanFilter(int w, Mat* input) {
    int i, j, wArea = (2*w + 1)*(2*w +1);
    Mat temp, output;
    Mat areas(input->rows + 2*w, input->cols + 2*w, CV_64FC3);

    //Convert the image
    input->convertTo(temp, CV_64FC3);

    //Fill the first row and column with 0
    for(int cY = 0; cY <= input->rows; cY++)
        areas.at<Vec3d>(cY, 0) = 0;
    for(int cX = 0; cX <= input->cols; cX++)
        areas.at<Vec3d>(0, cX) = 0;

    //Calculate the "integral" of the window from (0,0) to the pixel
    for(int cY = 0; cY < (input->rows + 2*w); cY++) {
        //Calculate the mirror coordinate
        if(cY - w <= 0)                 j = (cY - w) * - 1;
        else if(cY - w >= input->rows)  j = (cY - w) - ((cY - w) - input->rows + 1);
        else                            j = (cY - w);

        for(int cX = 0; cX < (input->cols + 2*w); cX++) {
            //Calculate the mirror coordinate
            if(cX - w <= 0)                 i = (cX - w) * - 1;
            else if(cX - w >= input->cols)  i = (cX - w) - ((cX - w) - input->cols + 1);
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

    //Calculate the mean for every pixel with the window size
    for(int cY = 0; cY < input->rows; cY++) {
        //Calculate the center of the window
        j = cY + w;
        for(int cX = 0; cX < input->cols; cX++) {
            //Calculate the center of the window
            i = cX + w;

            //Calculate the sum of the window
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
            //Calculate the mean of the window
            temp.at<Vec3d>(cY, cX) /= wArea;
        }
    }

    //Convert the image back
    temp.convertTo(output, CV_8UC3);
    *input = output;
}

void medianFilter(int w, Mat* input) {
    int i, j;
    Mat temp, output;
    Mat pixelSet = Mat_<Vec3dSet>(input->rows, input->cols);

    //Convert the image
    input->convertTo(temp, CV_64FC3);

    for(int cY = 0; cY < input->rows; cY++) {
        //Calculate the mirror coordinate
        if(cY - w <= 0)                 j = (cY - w) * - 1;
        else if(cY - w >= input->rows)  j = (cY - w) - ((cY - w) - input->rows + 1);
        else                            j = (cY - w);

        for(int cX = 0; cX < input->cols; cX++) {
            //Calculate the mirror coordinate
            if(cX - w <= 0)                 i = (cX - w) * - 1;
            else if(cX - w >= input->cols)  i = (cX - w) - ((cX - w) - input->cols + 1);
            else                            i = (cX - w);


        }
    }
}
