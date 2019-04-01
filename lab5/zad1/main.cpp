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
    VideoCapture stream(0);

    // Zmienne do wyświetlania obrazu
    Mat image, canny, gauss, sobelX, sobelY, gradientBinary, gradientColoured;

    // Zmienne pomocnicze
    Mat srcGray, sobelXBin, sobelYBin, gradient, angle;
    float angleVal;

    int thresholdSlider = 20;
    int cannyLowSlider = 50;
    int cannyHighSlider = 150;

    if (stream.isOpened()) {
        cout << "Pomyślnie otworzono obraz z kamery." << endl;
    } else {
        cout << "Błąd podczas otwierania obrazu z kamery." << endl;
    }

//    namedWindow("Camera", WINDOW_AUTOSIZE);
    stream.set(CAP_PROP_FRAME_WIDTH, 320);
    stream.set(CAP_PROP_FRAME_HEIGHT, 240);
    namedWindow("Settings", WINDOW_AUTOSIZE);

    createTrackbar("Threshold", "Settings", &thresholdSlider, 100);
    createTrackbar("Canny Low", "Settings", &cannyLowSlider, 100);
    createTrackbar("Canny High", "Settings", &cannyHighSlider, 200);

    while (stream.read(image)) {
        flip(image, image, +1);

        cvtColor(image, srcGray, COLOR_BGR2GRAY);

        // Filtr Gaussa
        GaussianBlur(srcGray, gauss, Size(3, 3), 2.0);

        // Metoda Cannyego
        Canny(gauss, canny, cannyLowSlider, cannyHighSlider, 3);

        // Krawędź pozioma
        Sobel(gauss, sobelX, CV_32F, 1.0, 0.0, 3);
        threshold(sobelX, sobelXBin, thresholdSlider, 255, THRESH_BINARY);

        // Krawędź pionowa
        Sobel(gauss, sobelY, CV_32F, 0.0, 1.0, 3);
        threshold(sobelY, sobelYBin, thresholdSlider, 255, THRESH_BINARY);


        cartToPolar(sobelX, sobelY, gradient, angle, true);
        threshold(gradient, gradientBinary, thresholdSlider, 255, THRESH_BINARY);

        // Gradient kolorowy
        gradientColoured = gradientBinary.clone();
        cvtColor(gradientColoured, gradientColoured, COLOR_GRAY2BGR);
        gradientColoured.convertTo(gradientColoured, CV_8UC3, 255);

        for (int i = 0; i < gradient.rows; i++) {
            for (int j = 0; j < gradient.cols; j++) {
                angleVal = angle.at<float>(i, j);
                Vec3b *color = &gradientColoured.at<Vec3b>(i, j);

                if (gradientBinary.at<float>(i, j) > 0) {
                    if (angleVal > 45 && angleVal <= 135)
                        *color = Vec3b(255, 255, 255);
                    if (angleVal > 135 && angleVal <= 255)
                        *color = Vec3b(255, 0, 0);
                    if (angleVal > 255 && angleVal <= 315)
                        *color = Vec3b(0, 255, 0);
                    if ((angleVal > 315 && angleVal <= 360) || (angleVal > 0 && angleVal <= 45))
                        *color = Vec3b(0, 0, 255);
                } else {
                    *color = Vec3b(0, 0, 0);
                }
            }
        }

        try {
            imshow("Camera", image);
            imshow("Canny", canny);
            imshow("Gauss", gauss);
            imshow("Sobel X", sobelXBin);
            imshow("Sobel Y", sobelYBin);
            imshow("Gradient binary", gradientBinary);
            imshow("Gradient coloured", gradientColoured);
        } catch (Exception &e) {
            cout << e.what() << endl;

            return 1;
        }

        if (waitKey(30) == ESC) break;
    }

    stream.release();
    image.release();
    canny.release();
    gauss.release();
    sobelX.release();
    sobelXBin.release();
    sobelY.release();
    sobelYBin.release();
    gradient.release();
    gradientBinary.release();
    gradientColoured.release();
    destroyAllWindows();

    return 0;
}
