#include <multiset.h>

bool compareRGB::operator()(const Vec3d& left,const Vec3d& right) const{
    /*------------- initialization ------------*/
    float leftSum, rightSum;
    /*-----------------------------------------*/

    //Calculate the distance of the vectors
    leftSum = sqrt(left.val[0] * left.val[0] +
                   left.val[1] * left.val[1] +
                   left.val[2] * left.val[2]);
    rightSum = sqrt(right.val[0] * right.val[0] +
                    right.val[1] * right.val[1] +
                    right.val[2] * right.val[2]);

    //Return if the left vector is less the right or not
    return leftSum < rightSum;
}

bool compareHarris::operator ()(const Vec3d& left, const Vec3d& right) const {
    return left.val[2] > right.val[2];
}
