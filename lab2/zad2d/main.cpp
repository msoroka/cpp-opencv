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
    int delay;
    int frames;
    int framesCounter = 0;
    double timestamp = 0;
    clock_t start;
    Mat image;
    VideoCapture cap;

    cout << "Ile klatek?" << endl;
    cin >> frames;

    cout << "Podaj opóźnienie (w ms):" << endl;
    cin >> delay;

    cap.open(0);

    namedWindow("window", WINDOW_AUTOSIZE);

    double dWidth = cap.set(CAP_PROP_FRAME_WIDTH, 640);
    double dHeight = cap.set(CAP_PROP_FRAME_HEIGHT, 480);

    if (!cap.isOpened()) {
        cout << "Błąd podczas otwierania kamery" << endl;
        return -1;
    }

    start = clock();
    while (true) {
        cap >> image;
        timestamp = clock() - start / (CLOCKS_PER_SEC / 1000);

        if (timestamp >= delay) {
            if (framesCounter < frames && !image.empty()) {
                string name = "frame_" + to_string(framesCounter) + ".jpg";
                imwrite("/Users/msoroka/Desktop/uczelnia/obrazy/lab2/zad2d/frames/" + name, image);
                framesCounter++;
            } else {
                break;
            }
        }

        imshow("window", image);

        if (waitKey(30) == ESC) break;
    }


    cap.release();
    destroyAllWindows();

    return 0;
}