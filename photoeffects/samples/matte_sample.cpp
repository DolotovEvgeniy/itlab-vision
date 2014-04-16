#include "photoeffects.hpp"
#include <iostream>

using namespace cv;
using namespace std;

const string ORIGINAL_IMAGE = "Original image";
const string MATTE_IMAGE = "Matte image";

Point firstPoint, secondPoint;
int numberOfChoosenPoints = 0;
float sigma1 = 0.0f;
float sigma2 = 0.0f;
const char *helper = "matte_sample.exe <img> <sigma1> <sigma2>\n\
\t<img>-file name contained the processed image\n\
\t<sigma1>-float param - power of the blur on Xahis\n\
\t<sigma2>-float param - power of the blur on Yahis";


void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    Mat src=*((Mat*)userdata);
    Mat srcCpy;
    if (event == EVENT_LBUTTONDOWN)
    {
        switch(numberOfChoosenPoints)
        {
            case 0:
            {
                firstPoint = Point(x,y);
                src.copyTo(srcCpy);
                circle(srcCpy, firstPoint, 3, CV_RGB(255, 0, 0), 3);
                imshow(ORIGINAL_IMAGE, srcCpy);
                numberOfChoosenPoints++;
                break;
            }
            case 1:
            {
                secondPoint = Point(x,y);
                src.copyTo(srcCpy);
                circle(srcCpy, firstPoint, 3, CV_RGB(255, 0, 0), 3);
                circle(srcCpy, secondPoint, 3, CV_RGB(255, 0, 0), 3);
                imshow(ORIGINAL_IMAGE, srcCpy);
                numberOfChoosenPoints++;
                Mat dst;
                matte(src, dst, firstPoint, secondPoint, sigma1, sigma2);
                namedWindow(MATTE_IMAGE, CV_WINDOW_AUTOSIZE);
                imshow(MATTE_IMAGE, dst);
                break;
            }
        }
    }
}

int processArguments(int argc, char **argv, Mat &src);

int main(int argc, char** argv)
{
    Mat src;
    if(processArguments(argc, argv, src) != 0)
    {
        cout << helper << endl;
        return 1;
    }
    namedWindow(ORIGINAL_IMAGE, CV_WINDOW_AUTOSIZE);
    imshow(ORIGINAL_IMAGE, src);
    setMouseCallback(ORIGINAL_IMAGE,CallBackFunc, &src);
    cout<<"Press any key"<<endl;
    waitKey(0);
    destroyAllWindows();
    return 0;
}

int processArguments(int argc, char **argv, Mat &src)
{
    if(argc < 4)
    {
        return 1;
    }
    src = imread(argv[1], CV_LOAD_IMAGE_COLOR);
    if(src.empty())
    {
        cout<<"Image file not found!"<<endl;
        return 2;
    }
    sigma1=atof(argv[2]);
    sigma2=atof(argv[3]);
    return 0;
}
