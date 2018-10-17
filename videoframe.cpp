#include "videoframe.h"

Mat VideoFrame::getFrame()
{
    return frame;
}

bool VideoFrame::isEmpty()
{
    if (frame.empty())
        return true;
    else return false;
}


// Displays the live feed from the Webcam
void VideoFrame::updateFeed()
{
    bool success{true};

    source.retrieve(frame);
    success = source.read(frame);
    if (!success)
        qDebug() << EMPTY_FRAME_ERROR;
    else
    {
        originalFrame = frame;
        applyEffects();
        imshow(FEED_WINDOW_NAME, frame);
        imshow(ORIGINAL_FEED_WINDOW_NAME, originalFrame);
        if (isRecording)
        {
            output.write(frame);
            setWindowTitle(FEED_WINDOW_NAME, FEED_WINDOW_NAME +  "- Recording");
        }
        else
            setWindowTitle(FEED_WINDOW_NAME, FEED_WINDOW_NAME);
    }
}


// Modules to set which operations will be performed over the Webcam feed
void VideoFrame::gaussianBlur()
{
    imshow(GAUSS_TRACKBAR_WINDOW_NAME, 1);
    resizeWindow(GAUSS_TRACKBAR_WINDOW_NAME, 400, 50);
    createTrackbar(KERNEL_TRACKBAR_NAME, GAUSS_TRACKBAR_WINDOW_NAME, &kernelSize, min(SLIDERMAX, min(frame.rows, frame.cols)));
    waitKey();
    destroyWindow(GAUSS_TRACKBAR_WINDOW_NAME);

    if (kernelSize > 0)
        isBlurred = true;
    else
        isBlurred = false;
}

void VideoFrame::detectEdges()
{
    if (detectedEdges)
        detectedEdges = false;
    else detectedEdges = true;
}

void VideoFrame::gradient()
{
    if (isGradient)
        isGradient = false;
    else isGradient = true;
}

void VideoFrame::adjustBrightness()
{
    QString newBrightnessValue;
    Dialog inputDialog;
    newBrightnessValue = inputDialog.readUserInput();

    if (newBrightnessValue == nullptr)
        return;

    brightnessAdjusted = true;
    bias += newBrightnessValue.toInt();
    bias = max(MINBIAS, bias);
    bias = min(MAXBIAS, bias);
}

void VideoFrame::adjustContrast()
{
    QString newContrastValue;
    Dialog inputDialog;
    newContrastValue = inputDialog.readUserInput();

    if (newContrastValue == nullptr)
        return;

    contrastAdjusted = true;
    gain = newContrastValue.toDouble();
    gain = max(MINGAIN, gain);
    gain = min(MAXGAIN, gain);
}

void VideoFrame::negative()
{
    if (isNegative)
        isNegative = false;
    else isNegative = true;
}

void VideoFrame::grayscale()
{
    if (isGrayscale)
        isGrayscale = false;
    else isGrayscale = true;
}

void VideoFrame::resizeToHalf()
{
    timesResized++;
}

void VideoFrame::rotate90Degrees()
{
    if (timesRotated == 3)
        timesRotated = 0;
    else
        timesRotated++;
}

void VideoFrame::horizontalMirror()
{
    if (isMirroredH)
        isMirroredH = false;
    else isMirroredH = true;
}

void VideoFrame::verticalMirror()
{
    if (isMirroredV)
        isMirroredV = false;
    else isMirroredV = true;
}

void VideoFrame::saveVideo()
{
    Dialog *popup = new Dialog();
    if (timesRotated > 0 || timesResized > 0)
    {
        popup->showMessage("Rotation and resizing not supported during recording of videos.");
        timesRotated = 0;
        timesResized = 0;
    }

    if (!isRecording)
    {
        QString fileName = popup->readUserInput();
        fileName += ".avi";
        output.open(fileName.toStdString(), CV_FOURCC('M', 'J', 'P', 'G'), 30, Size(frame.cols, frame.rows));
        isRecording = true;
    }
    else
    {
        output.release();
        isRecording = false;
    }
}

// Modules to apply all valid operations
void VideoFrame::applyEffects()
{
    if (isBlurred)
        applyGaussianBlur();
    if (detectedEdges)
        applyEdgeDetection();
    if (isGradient)
        applyGradient();
    if (brightnessAdjusted)
        applyBrightness();
    if (contrastAdjusted)
        applyContrast();
    if (isNegative)
        applyNegative();
    if (isGrayscale)
        applyGrayscale();
    for (int i = 0; i < timesResized; i++)
        applyResizing();
    for (int i = 0; i < timesRotated; i++)
        applyRotation();
    if (isMirroredH)
        applyHMirror();
    if (isMirroredV)
        applyVMirror();
}

void VideoFrame::applyGaussianBlur()
{
    Mat blurredImage;
    if (kernelSize%2 == 0)      // kernelSize value must be odd
        kernelSize--;
    GaussianBlur(frame, blurredImage, Size(kernelSize, kernelSize), 0, 0);
    frame = blurredImage;
}

void VideoFrame::applyEdgeDetection()
{
    Mat edges, gray, masked;

    cvtColor(frame, gray, CV_BGR2GRAY);
    blur(gray, edges, Size(3,3));
    Canny(edges, edges, THRESHOLD, THRESHOLD*RATIO, APERTURESIZE);

    masked = Scalar::all(0);
    frame.copyTo(masked, edges);
    frame = masked;
}

void VideoFrame::applyGradient()
{
    Mat gradients;
    Sobel(frame, gradients, CV_8U, 1, 1, 3);
    gradients.convertTo(gradients, -1, 1, 127);
    frame = gradients;
}

void VideoFrame::applyBrightness()
{
    Mat brightened;
    frame.convertTo(brightened, -1, 1, bias);   // Adds bias value to every pixel
    frame = brightened;
}

void VideoFrame::applyContrast()
{
    Mat contrasted;
    frame.convertTo(contrasted, -1, gain, 0);       // Multiplies every pixel by the gain value
    frame = contrasted;
}

void VideoFrame::applyNegative()
{
    Mat negative;
    frame.convertTo(negative, -1, -1, 255);
    frame = negative;
}

void VideoFrame::applyGrayscale()
{
    Mat grayscale;
    cvtColor(frame, grayscale, CV_BGR2GRAY);
    frame = grayscale;
}

void VideoFrame::applyResizing()
{
    Mat resized;
    resize(frame, resized, Size(static_cast<int>(round(0.5*frame.cols)), static_cast<int>(round(0.5*frame.rows))), 0, 0, INTER_CUBIC);
    frame = resized;
}

void VideoFrame::applyRotation()
{
    Mat rotated;
    rotate(frame, rotated, ROTATE_90_CLOCKWISE);
    frame = rotated;
}

void VideoFrame::applyHMirror()
{
    Mat flipped;
    flip(frame, flipped, 1);
    frame = flipped;
}

void VideoFrame::applyVMirror()
{
    Mat flipped;
    flip(frame, flipped, 0);
    frame = flipped;
}


VideoFrame::VideoFrame(int camera)
{
    source.open(camera);
    if (!source.isOpened())
        qDebug() << OPEN_CAMERA_ERROR;

    isBlurred = false;
    detectedEdges = false;
    isGradient = false;
    brightnessAdjusted = false;
    contrastAdjusted = false;
    isNegative = false;
    isGrayscale = false;
    isMirroredH = false;
    isMirroredV = false;
    isRecording = false;
    timesRotated = 0;
    timesResized = 0;
}

VideoFrame::~VideoFrame()
{
    source.release();
}
