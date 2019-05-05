#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

#define MAX_COUNT 500
#define ESC 27

int main() {
    VideoCapture stream;
    Mat frame, frameGrey, frameRGB, frameGreyPrev;
    vector<Point2f> points, pointsPrev;
    vector<uchar> status;
    vector<float> err;

    int choice = 0;

    TermCriteria termcrit(TermCriteria::COUNT | TermCriteria::EPS, 20, 0.03);

    cout << "1. Kamerka laptopa" << endl;
    cout << "2. Plik wideo bike" << endl;
    cout << "Podaj żródło obrazu:" << endl;
    cin >> choice;

    switch (choice) {
        case 1: {
            stream.open(0);
            break;
        }
        case 2: {
            stream.open("/Users/msoroka/Desktop/uczelnia/obrazy/lab8/bike.avi");
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

    namedWindow("Original", WINDOW_AUTOSIZE);
    namedWindow("Motion", WINDOW_AUTOSIZE);

    while (true) {
        stream >> frame;
        if (frame.empty()) {
            stream.set(CAP_PROP_POS_FRAMES, 0);
            stream >> frame;
        }
        frame.copyTo(frameRGB);
        cvtColor(frameRGB, frameGrey, COLOR_BGR2GRAY);

        if (pointsPrev.size() < 10) {
            goodFeaturesToTrack(frameGrey, points, MAX_COUNT, 0.001, 5, Mat(), 3, 0, false, 0.04);
        } else if (!pointsPrev.empty()) {
            calcOpticalFlowPyrLK(frameGreyPrev, frameGrey, pointsPrev, points, status, err, Size(31, 31), 3, termcrit, 0, 0.001);
        }

        for (int i = 0; i < pointsPrev.size(); i++) {
            line(frameRGB, points[i], pointsPrev[i], Scalar(255, 255, 255), 2, 1, 0);
            circle(frameRGB, points[i], 2, Scalar(255, 255, 255), 2, 1, 0);
        }

        imshow("Original", frame);
        imshow("Motion", frameRGB);

        swap(pointsPrev, points);
        points.clear();
        frameGrey.copyTo(frameGreyPrev);

        if (waitKey(30) == ESC) break;
    }

    frame.release();
    frameRGB.release();
    stream.release();
}
