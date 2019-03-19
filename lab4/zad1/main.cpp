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
    VideoCapture stream;
    Mat image, firstFrame, secondFrame, thirdFrame, result;
    int alphaSlider = 1;
    int thresholdSlider = 1;
    int choice = 0;

    cout << "1. Kamerka laptopa" << endl;
    cout << "2. Plik wideo bike" << endl;
    cout << "3. Plik wideo robot_no_loop" << endl;
    cout << "Podaj żródło obrazu:" << endl;
    cin >> choice;

    switch (choice) {
        case 1: {
            stream.open(0);
            break;
        }
        case 2: {
            stream.open("/Users/msoroka/Desktop/uczelnia/obrazy/lab4/bike.avi");
            break;
        }
        case 3: {
            stream.open("/Users/msoroka/Desktop/uczelnia/obrazy/lab4/robot_no_loop.avi");
            break;
        }
        default: {
            cout << "Błędny wybór." << endl;
            return -1;
        }
    }


    if (!stream.isOpened()) {
        cout << "Błąd podczas otwierania kamery." << endl;
    } else {
        cout << "Pomyślnie otworzono źródło obrazu." << endl;
    }

    stream >> firstFrame;
    secondFrame = firstFrame;
    cvtColor(firstFrame, firstFrame, COLOR_BGR2GRAY);
    cvtColor(secondFrame, secondFrame, COLOR_BGR2GRAY);

    image = firstFrame;

    namedWindow("Camera", WINDOW_AUTOSIZE);
    namedWindow("Settings", WINDOW_AUTOSIZE);

    createTrackbar("Alpha", "Settings", &alphaSlider, 300);
    createTrackbar("Threshold", "Settings", &thresholdSlider, 300);

    while (true) {
        stream >> secondFrame;
        stream >> thirdFrame;

        if (secondFrame.empty() || thirdFrame.empty()) {
            stream.set(CAP_PROP_POS_FRAMES, 0);
            stream >> secondFrame;
            stream >> thirdFrame;
        }

        cvtColor(secondFrame, secondFrame, COLOR_BGR2GRAY);
        cvtColor(thirdFrame, thirdFrame, COLOR_BGR2GRAY);
        absdiff(secondFrame, thirdFrame, result);
        threshold(result, result, thresholdSlider, 255, THRESH_BINARY);

        double alpha = (double) alphaSlider / 100;
        image = (1 - alpha) * image + alpha * result;

        try {
            imshow("Camera", image);
        } catch (Exception &e) {
            cout << e.what() << endl;

            return 1;
        }

        if (waitKey(30) == ESC) break;
    }

    stream.release();
    image.release();
    firstFrame.release();
    secondFrame.release();
    thirdFrame.release();
    result.release();
    destroyAllWindows();

    return 0;
}