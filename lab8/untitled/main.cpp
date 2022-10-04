#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;
int thresholdValue = 0;
int thresholdType = 3;

int const maxValue = 255;
int const maxType = 4;
int const maxBinaryValue = 255;
Mat src, srcGray;
Mat thresholdImage;
Mat adaptiveThresholdImage;
std::string adaptiveThresholdingWindow = "Adaptive thresholding";
std::string thresholdingWindow = "Thresholding";


void imshow_with_text( std::string text, cv::Mat& img )
{
    int font_face = cv::FONT_HERSHEY_DUPLEX;
    int thickness = 2;
    double font_scale = 1.0;
    int* base_line = 0;
    cv::Size text_size = cv::getTextSize( text, font_face, font_scale, thickness, base_line );
    // Calculate center text position.
    cv::Point position = cv::Point( img.cols / 2 - (text_size.width / 2),
                                    img.rows - (text_size.height / 2) );
    cv::putText( img, //target image
                 text, //text
                 position,
                 font_face,
                 font_scale,
                 CV_RGB( 255, 0, 0 ), //font color
                 thickness );

    cv::imshow( text, img );
}

void press_esc_to_close( int delay )
{
    for (;;)
    {
        if ( cv::waitKey( delay ) == 27 )
        {
            cv::destroyAllWindows();
            break;
        }
    }
}

cv::Mat get_image_source( cv::ImreadModes mode, std::string path )
{
    cv::Mat img = imread( path, mode );
    if ( img.empty() ) {
        std::cout << "Error reading the image. Try another one.\n";
        exit( EXIT_FAILURE );
    }

    return img;
}

//void onThresholdChanged(int, void*) { }

//void showTresholding(const cv::Mat &image){
//    src = image.clone();
//    cvtColor(src, srcGray, COLOR_BGR2GRAY);
//    namedWindow(thresholdingWindow, WINDOW_AUTOSIZE);
//    createTrackbar("Threshold type", thresholdingWindow,
//                   &thresholdType, maxType, onThresholdChanged);
//    createTrackbar("Threshold", thresholdingWindow, &thresholdValue,
//                   maxValue, onThresholdChanged);
//    onThresholdChanged(0, nullptr);
//}
int adaptiveMethod = 0;
const int maxAdaptiveMethod = 1;
int blockSize = 3;
int C = 1;
void onAdaptiveThresholdChanged(int, void *) {
    if (blockSize % 2 == 0){
        blockSize++;
    }
    if (blockSize <= 3) {
        blockSize = 3;
    }
    // Добавить adaptive tresholding
}
//void showAdaptiveTresholding(const cv::Mat &image){
//    src = image;
//    if (src.rows > 1600 || src.cols > 2560) {
//        resize(src, src, Size(src.cols/5, src.rows/5));
//    }
//    cvtColor(src, srcGray, COLOR_BGR2GRAY);
//    namedWindow(adaptiveThresholdingWindow, WINDOW_AUTOSIZE );
//    createTrackbar("Adaptive method", adaptiveThresholdingWindow,
//                   &adaptiveMethod, maxAdaptiveMethod, onAdaptiveThresholdChanged);

//    createTrackbar("Block size", adaptiveThresholdingWindow,
//                   &blockSize, maxValue, onAdaptiveThresholdChanged);

//    createTrackbar("Const", adaptiveThresholdingWindow, &C, maxValue,
//                   onAdaptiveThresholdChanged);
//    onAdaptiveThresholdChanged(0, nullptr);
//}

void drawCont( const Mat &image, const String windowToShow )
{
    std::vector<std::vector<cv::Point> > contours;
    cv::Mat contourOutput = image.clone();
    cv::findContours(contourOutput,
                     contours,
                     RETR_LIST,
                     CHAIN_APPROX_SIMPLE );
    // Отрисовка контуров
    cv::Mat contourImage(image.size(), CV_8UC3, cv::Scalar(0,0,0));
//    cv::Scalar colors[3];
//    colors[0] = cv::Scalar(255, 0, 0);
//    colors[1] = cv::Scalar(0, 255, 0);
//    colors[2] = cv::Scalar(0, 0, 255);

//    for (size_t idx = 0; idx < contours.size(); idx++) {
//        cv::drawContours(contourImage, contours, idx, colors[idx % 3]);
//    }

    cv::drawContours(contourImage, contours, -1, cv::Scalar(0, 0, 255) );
    imshow_with_text( windowToShow, contourImage );
}


Mat img_original,
    img_corrected,
    img_gray,
    img_threshold;

int contrast_val              = 10,
    brightness_val            = 0,
    binarization_val          = 20,
    adaptive_binarization_val = 0;

void contrast_handler( int cur_pos, void* )
{
    std::cout << "Contrast: " << cur_pos / 10.0 << std::endl;
    img_original.convertTo( img_corrected, -1, cur_pos / 10.0, 0 );
    imshow( "main_window", img_corrected );
}

void brightness_handler( int cur_pos, void* )
{
    std::cout << "Brightness: " << cur_pos << std::endl;
    img_corrected = img_original + Scalar( cur_pos, cur_pos, cur_pos );
    imshow( "main_window", img_corrected );
}

void adaptive_binarization_handler( int cur_pos, void* )
{
    std::cout << "Adaptive binarization: " << cur_pos << std::endl;
    blockSize = cur_pos;
    if (blockSize % 2 == 0){
        blockSize++;
    }
    if (blockSize <= 3) {
        blockSize = 3;
    }
    cv::adaptiveThreshold( img_gray,
                           img_threshold,
                           255,
                           cv::ADAPTIVE_THRESH_GAUSSIAN_C,
                           cv::THRESH_BINARY,
                           blockSize,
                           3);

    drawCont( img_threshold, "adaptive_binarizations_contours" );
    imshow_with_text( "binarization_adaptive", img_threshold );
}

void binarization_handler( int cur_pos, void* )
{
    std::cout << "Binarization: " << cur_pos << std::endl;
    binarization_val = cur_pos;

    cv::threshold( img_gray,
                   img_threshold,
                   binarization_val,
                   255,
                   cv::THRESH_BINARY );

    drawCont( img_threshold, "binarizations_contours" );
    imshow_with_text( "binarization", img_threshold );
}


int main( int argc, char** argv )
{
    img_original = get_image_source( IMREAD_COLOR, "../../imgs/cars.jpeg" );
    cv::cvtColor( img_original, img_gray, cv::COLOR_BGR2GRAY );

    img_corrected = img_original.clone();
    imshow( "main_window", img_original );

    //    contrast_handler( 1, nullptr );
    //    brightness_handler( 0, nullptr );
    binarization_handler( binarization_val, nullptr );
    adaptive_binarization_handler( binarization_val, nullptr );

    createTrackbar( "Contrast", "main_window", &contrast_val, 50, contrast_handler );
    createTrackbar( "Brightness", "main_window", &brightness_val, 200, brightness_handler );

    createTrackbar( "Apadtive binarization", "main_window", &adaptive_binarization_val, 200, adaptive_binarization_handler );
    createTrackbar( "Binarization", "main_window", &binarization_val, 200, binarization_handler );

    press_esc_to_close( 0 );

    return 0;
}
