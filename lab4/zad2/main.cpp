#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <string>
#include <vector>
#include <iostream>

#define ESC 27

using namespace std;
using namespace cv;

int main() {
    VideoCapture cap;
    Mat image;
    vector<cv::String> fn;

    glob("/Users/msoroka/Desktop/uczelnia/obrazy/lab4/frames_vid/*.jpg", fn, false);

    vector<Mat> images;
    int count = fn.size();

    for (int i = 0; i < count; i++)
        images.push_back(imread(fn[i]));

    int width = images[0].cols;
    int height = images[0].rows;

    Size frameSize(width, height);

    VideoWriter outputVideo;
    outputVideo.open("out.mov", -1, 30, frameSize, true);

    for (int i = 0; i < count; i++) {
        outputVideo.write(images[i]);

        waitKey(10);
    }

    outputVideo.release();

    return 0;
}