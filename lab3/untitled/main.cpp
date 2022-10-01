#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <unistd.h>
using namespace cv;
void demo1();
void demo2();
void task1();
void task2();
void task3();
void task4();
Mat getImageFromHistogram(Mat hist, float scaleX = 1, float scaleY = 1, Scalar
                          fillColor = Scalar(255, 255, 255)) {
    double maxVal = 0;
    minMaxLoc(hist, 0, &maxVal, 0, 0);
    int rows = 64; //default height size
    int cols = hist.rows; //get the width size from the histogram
    Mat histImg = Mat::zeros(rows*scaleX, cols*scaleY, CV_8UC3);
    //for each bin
    for (int i = 0; i<cols - 1; i++) {
        float histValue = hist.at<float>(i, 0);
        float nextValue = hist.at<float>(i + 1, 0);
        Point pt1 = Point(i*scaleX, rows*scaleY);
        Point pt2 = Point(i*scaleX + scaleX, rows*scaleY);
        Point pt3 = Point(i*scaleX + scaleX, (rows - nextValue * rows /
                                              maxVal)*scaleY);
        Point pt4 = Point(i*scaleX, (rows - nextValue * rows / maxVal)*scaleY);
        int numPts = 5;
        Point pts[] = { pt1, pt2, pt3, pt4, pt1 };
        fillConvexPoly(histImg, pts, numPts, fillColor);
    }
    return histImg;
}

//
Mat get_image_source( ImreadModes mode, std::string path );
void show_CLAHE(Mat br, std::vector<Mat> channels, int clip_limit, int grid_size );
void press_esc_to_close( int delay );
void imshow_with_text( Mat img, std::string text );

int main( int argc, char** argv ) {
    //for ( ;; )
    //{
        std::cout << "OPENCV HISTOGRAM\n\n"
                  << "Demos:\n"
                  << "1. Histogram of a RGB image.\n"
                  << "2. Histogram equalization of a grayscale image.\n\n"
                  << "Tasks:\n"
                  << "3. Histogram of intensity.\n"
                  << "4. Histogram of a video.\n"
                  << "5. Histogram equalization of an RGB image.\n"
                  << "6. Adaptive histogram equalization.\n"
                  << "7. Two dimentional histograms.\n\n";
        std::cout << "Which program would you like to run?: ";

        switch ( std::atoi( argv[1] ) ) {
        case 1:
            demo1();
            break;
        case 2:
            demo2();
            break;
        case 3:
            task1();
            break;
        case 4:
            task2();
            break;
        case 5:
            task3();
            break;
        case 6:
            task4();
            break;
        }
//        std::cin.get();
    //}

    return EXIT_SUCCESS;
}
void demo1() {
    Mat img = Mat();
    while (img.empty()) {
        std::cout << "Enter path to an image: ";
        std::string path;
        std::cin >> path;
        img = imread(path, IMREAD_COLOR);
        if (img.empty()) {
            std::cout << "Error reading the image. Try another one.\n";
        }
    }
    MatND hist;
    Mat histImg;
    int nbins = 256; // lets hold 256 levels
    int hsize[] = { nbins }; // just one dimension
    float range[] = { 0, 255 };
    const float *ranges[] = { range };
    int chnls[] = { 0 };
    std::vector<Mat> channels;
    split(img, channels);
    calcHist(&channels[0], 1, chnls, Mat(), hist, 1, hsize, ranges);
    histImg = getImageFromHistogram(hist, 3, 3, Scalar(255, 0, 0));
    imshow("Blue", histImg);
    calcHist(&channels[1], 1, chnls, Mat(), hist, 1, hsize, ranges);
    histImg = getImageFromHistogram(hist, 3, 3, Scalar(0, 255, 0));
    imshow("Green", histImg);

    calcHist(&channels[2], 1, chnls, Mat(), hist, 1, hsize, ranges);
    histImg = getImageFromHistogram(hist, 3, 3, Scalar(0, 0, 255));
    imshow("Red", histImg);
    imshow("Image", img);
    waitKey(0);
}
void demo2() {
    Mat img = Mat();
    while (img.empty()) {
        std::cout << "Enter path to an image: ";
        std::string path;
        std::cin >> path;
        img = imread(path, IMREAD_COLOR);
        if (img.empty()) {
            std::cout << "Error reading the image. Try another one.\n";
        }
    }
    cvtColor(img, img, COLOR_BGR2GRAY);
    Mat image_equalized;
    equalizeHist(img, image_equalized);
    imshow("Original image", img);
    imshow("Equalized image", image_equalized);
    waitKey(0);
}
void task1() {
    // Получаем изображение.
    Mat source_img = get_image_source( IMREAD_COLOR, "../imgs/img3.png" );
    // Хранение чб изображения.
    Mat gray_img;
    // Хранение YCC формата изображения.
    Mat ycc_img;

    // Для сравнения гистрограммы яркости чб и ycc.
    // Конвертируем изображение в серый.
    cvtColor( source_img, gray_img, COLOR_BGR2GRAY );
    // Ковертируем изображение в YCC.
    cvtColor( source_img, ycc_img, COLOR_BGR2YCrCb );

    MatND hist;
    Mat hist_img;
    int nbins = 256; // lets hold 256 levels
    int hsize[] = { nbins }; // just one dimension
    float range[] = { 0, 255 };
    const float *ranges[] = { range };
    int chnls[] = { 0 };
    calcHist( &gray_img, 1, chnls, Mat(), hist, 1, hsize, ranges );
    hist_img = getImageFromHistogram( hist, 3, 3, Scalar(255, 255, 255) );
    imshow_with_text( source_img, "source_img" );
    imshow_with_text( gray_img, "gray_img" );
    imshow_with_text( hist_img, "hist_br_gr_sc" );
    std::vector<Mat> channels;
    split( ycc_img, channels );
    calcHist( &channels[0], 1, chnls, Mat(), hist, 1, hsize, ranges );
    hist_img = getImageFromHistogram( hist, 3, 3, Scalar(255, 255, 255) );
    imshow_with_text( hist_img, "hist_br_conv" );
    press_esc_to_close( 0 );
}

void task2() {
    Mat frame;
    Mat gray_img;
    MatND hist;
    Mat hist_img;
    int nbins = 256; // lets hold 256 levels
    int hsize[] = { nbins }; // just one dimension
    float range[] = { 0, 255 };
    const float *ranges[] = { range };
    int chnls[] = { 0 };
    VideoCapture cap( "../imgs/highway_traffic720.mp4" );

    if ( !cap.isOpened() )  // isOpened() returns true if capturing has been initialized.
    {
        std::cout << "Cannot open the video file. \n";
        exit( EXIT_FAILURE );
    }

//    double fps = cap.get( 60 );
    namedWindow( "capture_video", WINDOW_AUTOSIZE );
    for (;;)
    {

        if ( !cap.read(frame) ) // if not success, break loop
            // read() decodes and captures the next frame.
        {
            std::cout<<"\n Cannot read the video file. \n";
            break;
        }
        cvtColor( frame, gray_img, COLOR_BGR2GRAY );
        std::vector<Mat> channels;
        calcHist( &gray_img, 1, chnls, Mat(), hist, 1, hsize, ranges );
        hist_img = getImageFromHistogram( hist, 3, 3, Scalar(255, 255, 255) );
        imshow( "capture_video", frame );
        imshow( "gray_scale_image", gray_img );
        imshow( "gray_conversion", hist_img );

        //
        split( frame, channels );
        calcHist( &channels[0], 1, chnls, Mat(), hist, 1, hsize, ranges );
        hist_img = getImageFromHistogram( hist, 3, 3, Scalar(255, 0, 0) );
        imshow( "blue", hist_img );

        //
        calcHist( &channels[1], 1, chnls, Mat(), hist, 1, hsize, ranges );
        hist_img = getImageFromHistogram( hist, 3, 3, Scalar(0, 255, 0) );
        imshow( "green", hist_img );

        //
        calcHist( &channels[2], 1, chnls, Mat(), hist, 1, hsize, ranges );
        hist_img = getImageFromHistogram( hist, 3, 3, Scalar(0, 0, 255) );
        imshow( "red", hist_img );

        if ( waitKey( 16 ) == 27 )
        {
            destroyAllWindows();
            break;
        }
    }
}
void task3() {
    Mat img = get_image_source( IMREAD_COLOR, "../imgs/img2.jpeg" );
    Mat ycc;
    cvtColor( img, ycc, COLOR_BGR2YCrCb );
    MatND hist;
    Mat hist_img;
    int nbins = 256; // lets hold 256 levels
    int hsize[] = { nbins }; // just one dimension
    float range[] = { 0, 255 };
    const float *ranges[] = { range };
    int chnls[] = { 0 };
    imshow_with_text( img, "source_image" );
    std::vector<Mat> channels;
    split( ycc, channels );

    //
    calcHist( &channels[0], 1, chnls, Mat(), hist, 1, hsize, ranges );
    hist_img = getImageFromHistogram( hist, 3, 3, Scalar(255, 255, 255) );
    imshow_with_text( hist_img, "source_hist" );
    equalizeHist( channels[0], channels[0] );
    merge( channels, ycc );
    calcHist( &channels[0], 1, chnls, Mat(), hist, 1, hsize, ranges );
    hist_img = getImageFromHistogram( hist, 3, 3, Scalar(255, 255, 255) );
    cvtColor( ycc, img, COLOR_YCrCb2BGR );
    imshow_with_text( img, "image_after_eqlz" );
    imshow_with_text( hist_img, "hist_after_eqlz" );
    press_esc_to_close( 0 );
}
void task4() {
    Mat img = get_image_source( IMREAD_COLOR, "../imgs/cosmic.jpeg" );
    Mat ycc;
    cvtColor( img, ycc, COLOR_BGR2YCrCb );
    MatND hist;
    Mat hist_img;
    int nbins = 256; // lets hold 256 levels
    int hsize[] = { nbins }; // just one dimension
    float range[] = { 0, 255 };
    const float *ranges[] = { range };
    int chnls[] = { 0 };
    imshow_with_text( img, "original_img" );

    std::vector<Mat> channels;
    split( ycc, channels );
    Mat br = channels[0].clone();
    calcHist( &channels[0], 1, chnls, Mat(), hist, 1, hsize, ranges );
    hist_img = getImageFromHistogram( hist, 3, 3, Scalar(255, 255, 255) );
    imshow_with_text( hist_img, "original_img_hist" );
    press_esc_to_close( 0 );

    int clip_limit = 40;
    int grid_size = 8;
    show_CLAHE( br, channels, clip_limit, grid_size );

    //
    clip_limit = 16;
    show_CLAHE( br, channels, clip_limit, grid_size );

    //
    clip_limit = 8;
    show_CLAHE( br, channels, clip_limit, grid_size );

    //
    clip_limit = 4;
    show_CLAHE( br, channels, clip_limit, grid_size );
}

void show_CLAHE(Mat br, std::vector<Mat> channels, int clip_limit, int grid_size )
{
    MatND hist;
    Mat hist_img;
    Mat ycc;
    int nbins = 256; // lets hold 256 levels
    int hsize[] = { nbins }; // just one dimension
    float range[] = { 0, 255 };
    const float *ranges[] = { range };
    int chnls[] = { 0 };

    Ptr<CLAHE> clahe = createCLAHE();
    clahe->setTilesGridSize( Size(grid_size, grid_size) );
    clahe->setClipLimit( clip_limit );
    clahe->apply( br, channels[0] );

    merge( channels, ycc );
    calcHist( &channels[0], 1, chnls, Mat(), hist, 1, hsize, ranges );
    hist_img = getImageFromHistogram( hist, 3, 3, Scalar( 255, 255, 255) );
    cvtColor( ycc, ycc, COLOR_YCrCb2BGR );
    imshow_with_text( ycc, format("after_adopt_eqlz_clip_limit=%d", clip_limit) );
    hist_img = getImageFromHistogram( hist, 3, 3, Scalar(255, 255, 255) );

    calcHist( &channels[0], 1, chnls, Mat(), hist, 1, hsize, ranges );
    hist_img = getImageFromHistogram( hist, 3, 3, Scalar(255, 255, 255) );
    imshow_with_text( hist_img, format("hist_clip_limit=%d", clip_limit) );

    press_esc_to_close( 0 );
}

void imshow_with_text( Mat img, std::string text )
{
    putText( img, //target image
             text, //text
             cv::Point( img.cols / 2 - 50, img.rows - 20 ), //top-left position
             cv::FONT_HERSHEY_DUPLEX,
             1.0,
             CV_RGB( 255, 0, 0 ), //font color
             2 );
    imshow( text, img );
}

void press_esc_to_close( int delay )
{
    for (;;)
    {
        if ( waitKey( delay ) == 27 )
        {
            destroyAllWindows();
            break;
        }
    }
}


Mat get_image_source( ImreadModes mode, std::string path )
{
    Mat img = Mat();
    while (img.empty()) {
        img = imread( path, mode );
        if ( img.empty() ) {
            std::cout << "Error reading the image. Try another one.\n";
        }
    }

    return img;
}
