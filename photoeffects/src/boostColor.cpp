#include "photoeffects.hpp"

using namespace cv;

int boostColor(cv::InputArray src, cv::OutputArray dst, float intensity)
{
    Mat srcImg = src.getMat();

    CV_Assert(srcImg.channels() == 3);
    CV_Assert(intensity >= 0.0f && intensity <= 1.0f);

    int srcImgType = srcImg.type();
    if (srcImgType!= CV_32FC3)
    {
        srcImg.convertTo(srcImg, CV_32FC3);
    }

    int height = srcImg.rows;
    int width = srcImg.cols;

    Mat srcHls(srcImg);
    srcHls /= 255.0f;
    cvtColor(srcHls, srcHls, CV_BGR2HLS);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //increase channel - 3(saturation)
            float saturation = srcHls.at<Vec3f>(i,j)[2];
            saturation += intensity;
            if (saturation > 1.0f)
            {
                saturation = 1.0f;
            }
            srcHls.at<Vec3f>(i,j)[2] = saturation;
        }
    }

    cvtColor(srcHls, dst, CV_HLS2BGR);
    dst.getMat() *= 255.0f;
    dst.getMat().convertTo(dst, srcImgType);
    return 0;
}