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

    // Ścieżka, w której znajdują się obrazki z klatkami z laborek drugich
    // Zapisanie wszystkich obrazów do wektora stringów 
    glob("/Users/msoroka/Desktop/uczelnia/obrazy/lab4/frames_vid/*.jpg", fn, false);

    vector<Mat> images;

    // Ile obrazów udało się wczytać
    int count = fn.size();

    // Przeiterowanie po wszystkich wczytanych obrazkach
    // i zapisanie ich do wektora obiektów Mat
    for (int i = 0; i < count; i++)
        images.push_back(imread(fn[i]));

    // Pobranie wysokości i szerokości obrazków
    int width = images[0].cols;
    int height = images[0].rows;

    // Utworzenie obiektu klasy Size
    Size frameSize(width, height);

    // Zmienna, do której będzie zapisywane wideo
    VideoWriter outputVideo;

    // Ustawienie nazwy wideo, kodeka, ilości klatek na sekundę i rozmiaru (z klasy Size)
    outputVideo.open("out.mov", -1, 30, frameSize, true);

    // Zapisanie do pliku wideo wczytanych zdjęć
    for (int i = 0; i < count; i++) {
        outputVideo.write(images[i]);

        waitKey(10);
    }

    outputVideo.release();

    return 0;
}