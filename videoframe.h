#ifndef VIDEOFRAME_H
#define VIDEOFRAME_H
#include <iostream>
#include <cmath>
#include <exception>
#include <opencv2/opencv.hpp>
#include <QApplication>
#include <QObject>
#include <QDebug>
#include "gui.h"
using namespace cv;
using namespace std;

const int SLIDERMAX = 50;
const int RATIO = 3;
const int APERTURESIZE = 3;
const int THRESHOLD = 255/3;
const int BLUE = 0;
const int GREEN = 1;
const int RED = 2;
const int MINBIAS = -255;
const int MAXBIAS = 255;

const char EMPTY_FRAME_ERROR[] = "Frame could not be read.\t";
const char OPEN_CAMERA_ERROR[] = "Camera could not be opened.\t";

const double MINGAIN = 0.00001;
const double MAXGAIN = 255.0;

const string FEED_WINDOW_NAME = "Edited feed";
const string ORIGINAL_FEED_WINDOW_NAME = "Original feed";
const string KERNEL_TRACKBAR_NAME = "Kernel size: ";
const string GAUSS_TRACKBAR_WINDOW_NAME = "Gaussian Blur";
const string CANNY_TRACKBAR_NAME = "Threshold: ";
const string CANNY_TRACKBAR_WINDOW_NAME = "Edge Detection";

class VideoFrame: public QObject
{
    Q_OBJECT

private:
    Mat frame, originalFrame;
    VideoCapture source;
    bool isBlurred, detectedEdges, isGradient, brightnessAdjusted, contrastAdjusted, isNegative, isGrayscale, isMirroredH, isMirroredV, isRecording;
    int kernelSize, bias, timesRotated, timesResized;
    double gain;

    // Modules to apply all valid operations
    void applyGaussianBlur();
    void applyEdgeDetection();
    void applyGradient();
    void applyBrightness();
    void applyContrast();
    void applyNegative();
    void applyGrayscale();
    void applyResizing();
    void applyRotation();
    void applyHMirror();
    void applyVMirror();

public:
    Mat getFrame();
    bool isEmpty();

    void applyEffects();            // Module that checks which operations should be applied and applies them - necessary since all chosen operations have to be applied to every frame

    void gaussianBlur();            // Modules to set which operations will be performed over the Webcam feed
    void detectEdges();
    void gradient();
    void adjustBrightness();
    void adjustContrast();
    void negative();
    void grayscale();
    void resizeToHalf();
    void rotate90Degrees();
    void horizontalMirror();
    void verticalMirror();

    VideoFrame(int camera);
    VideoFrame(QObject *parent = nullptr) : QObject(parent) {}
    ~VideoFrame();

public slots:
    void updateFeed();      // Displays the live feed from Webcam to user
};

#endif // VIDEOFRAME_H
