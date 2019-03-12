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
    Mat camera;
    Mat image = imread("/Users/msoroka/Desktop/uczelnia/obrazy/lab3/logo.bmp", 0);
    VideoCapture cap;
    int choice;

    cout << "1. Kamerka laptopa" << endl;
    cout << "2. Plik wideo" << endl;
    cout << "Podaj żródło obrazu:" << endl;
    cin >> choice;

    switch (choice) {
        case 1: {
            cap.open(0);
            break;
        }
        case 2: {
            cap.open("/Users/msoroka/Desktop/uczelnia/obrazy/lab3/robot_no_loop.mov");
            break;
        }
        default: {
            cout << "Błędny wybór" << endl;
            return -1;
        }
    }

    if (!cap.isOpened()) {
        cerr << "Błąd podczas otwierania pliku lub kamerki wideo." << endl;
        return -1;
    } else {
        cout << "Pomyślnie otwarto plik wideo." << endl;
    }

    int sizeOfImage = 0;

    cout << "Podaj rozmiar obrazka" << endl;
    cin >> sizeOfImage;

    resize(image, image, Size(sizeOfImage, sizeOfImage));

    namedWindow("window", WINDOW_AUTOSIZE);

    while (true) {
        cap >> camera;

        if (camera.empty()) {
            break;
        }

        try {
            cvtColor(camera, camera, COLOR_BGR2GRAY);
            Rect react(camera.cols - sizeOfImage, camera.rows - sizeOfImage, sizeOfImage, sizeOfImage);
            camera(react) = image + camera(react);

            imshow("window", camera);
        } catch (Exception &e) {
            cout << e.what() << endl;

            return 1;
        }

        if (waitKey(30) == ESC) break;
    }

    cap.release();
    destroyAllWindows();

    return 0;
}