#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

#include <iostream>

#define ESC 27

using namespace cv;
using namespace std;

int main() {
    VideoCapture stream;
    Mat original, foreground, background;
    Ptr<BackgroundSubtractorMOG2> pMOG2 = createBackgroundSubtractorMOG2();

    int substractor_history = 100;
    int substractor_mixtures = 20;
    int shadows = 0;
    int choice = 0;

    namedWindow("Background", 1);
    namedWindow("Motion", 1);
    namedWindow("Settings", 1);

    createTrackbar("History", "Settings", &substractor_history, 200);
    createTrackbar("nMixtures", "Settings", &substractor_mixtures, 100);
    createTrackbar("Shadows", "Settings", &shadows, 1);

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

    while (true) {
        stream >> original;
        if (original.empty()) {
            stream.set(CAP_PROP_POS_FRAMES, 0);
            stream >> original;
        }

        pMOG2->apply(original, foreground);
        pMOG2->setHistory(substractor_history);
        pMOG2->setNMixtures(substractor_mixtures);
        pMOG2->setDetectShadows(static_cast<bool>(shadows));
        pMOG2->getBackgroundImage(background);

        vector<vector<Point> > contours;
        findContours(foreground, contours, RETR_LIST, CHAIN_APPROX_NONE);

        for(int i=0; i<contours.size(); i++)
            drawContours(original, contours, i, Scalar(100, 100, 100), 1);

        imshow("Motion", original);
        imshow("Background", background);

        if (waitKey(30) == ESC) break;
    }

    stream.release();
    original.release();
    foreground.release();
    background.release();

    return 0;
}