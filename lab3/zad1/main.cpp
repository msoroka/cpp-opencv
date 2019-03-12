#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <iostream>

#define ESC 27

using namespace std;
using namespace cv;

int main() {
    VideoCapture stream("/Users/msoroka/Desktop/uczelnia/obrazy/lab3/robot_no_loop.mov");

    Mat firstFrame, secondFrame, result;
    Mat firstFrame2, secondFrame2, result2;

    stream.read(firstFrame2);
    cvtColor(firstFrame2, firstFrame2, COLOR_RGB2GRAY);

    if (!stream.isOpened()) {
        cout << "Błąd podczas przetwarzania wideo." << endl;
    } else {
        cout << "Pomyślnie otwarto plik wideo." << endl;
    }

    while (true) {
        stream >> firstFrame;
        stream >> secondFrame;

        if (firstFrame.empty() || secondFrame.empty()) {
            break;
        }

        imshow("cam1", secondFrame);

        stream.read(secondFrame2);
        cvtColor(secondFrame2, secondFrame2, COLOR_RGB2GRAY);

        cvtColor(firstFrame, firstFrame, COLOR_RGB2GRAY);
        cvtColor(secondFrame, secondFrame, COLOR_RGB2GRAY);

        absdiff(firstFrame, secondFrame, result);
        absdiff(firstFrame2, secondFrame2, result2);

        threshold(result, result, 100, 255, THRESH_TRUNC);
        threshold(result2, result2, 100, 255, THRESH_TRUNC);

        imshow("cam2", result);
        imshow("cam3", result2);

        if (waitKey(30) == ESC) break;
    }

    return 0;
}