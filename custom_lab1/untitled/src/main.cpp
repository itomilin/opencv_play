#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>

#include "blob.h"
#include "player.h"

cv::VideoCapture cap( "../../imgs/highway_traffic720.mp4" );

// Обработчик уровня бинаризации
void trackbar_handler_binary( int current_position, void (*ptr) ) { }

void trackbar_handler_video_pos( int current_position, void (*ptr) )
{
    cap.set( cv::CAP_PROP_POS_FRAMES, current_position );
}

void trackbar_handler_area( int current_position, void (*ptr) ) { }

double dbl_aspect_ratio_upper = 3.2;
void trackbar_handler_dbl_aspect_ratio_upper( int current_position, void (*ptr) )
{
    std::cout << current_position / 10.0 << std::endl;
    dbl_aspect_ratio_upper = current_position / 10.0;
}

void trackbar_handler_dbl_diagonal_size( int current_position, void (*ptr) ) { }

void trackbar_handler_bounding_rect_wh( int current_position, void (*ptr) ) { }

void trackbar_handler_bounding_rect_area( int current_position, void (*ptr) ) { }


int main( int argc, char **argv )
{
    int binarization_val         = 30,
        bounding_rect_area       = 11,
        bounding_rect_wh         = 5,
        dbl_diagonal_size        = 5,
        dbl_aspect_ratio_upper_t = 50;

    cv::Mat imgFrame1;
    cv::Mat imgFrame2;

    if ( !cap.isOpened() )
    {
        std::cout << "Could not open the file." << std::endl << std::endl;
        return EXIT_FAILURE;
    }

    cap.read( imgFrame1 );
    cap.read( imgFrame2 );

    create_window( "binary", 480, 320, 50, 50 );
    create_window( "detection", 480, 320, 550, 50 );
    create_window( "original", 480, 320, 550, 450 );

    trackbar_handler_binary( binarization_val, &cap );
    trackbar_handler_video_pos( 0, &cap );

    cv::createTrackbar( "binarization_ratio",
                        "binary",
                        &binarization_val,
                        255,
                        trackbar_handler_binary );

    cv::createTrackbar( "bounding_rect_area",
                        "binary",
                        &bounding_rect_area,
                        255,
                        trackbar_handler_bounding_rect_area );

    cv::createTrackbar( "set_size_particles (effective)",
                        "binary",
                        &bounding_rect_wh,
                        255,
                        trackbar_handler_bounding_rect_wh );

    cv::createTrackbar( "dbl_diagonal_size (effective)",
                        "binary",
                        &dbl_diagonal_size,
                        255,
                        trackbar_handler_dbl_diagonal_size );

    cv::createTrackbar( "dbl_aspect_ratio_upper (effective)",
                        "binary",
                        &dbl_aspect_ratio_upper_t,
                        255,
                        trackbar_handler_dbl_aspect_ratio_upper );

    cv::createTrackbar( "pos",
                        "original",
                        0,
                        cap.get( cv::CAP_PROP_FRAME_COUNT ),
                        trackbar_handler_video_pos );

    cv::Mat imgDifference,
            imgThresh,
            imgFrame1Copy,
            imgFrame2Copy,
            imgThreshCopy;
    std::vector< Blob > blobs;
    std::vector< std::vector< cv::Point > > contours;

    int fps = (int) cap.get( cv::CAP_PROP_FPS );
    while ( !imgFrame2.empty() && cv::waitKey( 1000.0 / fps ) != 27 )
    {
        imgFrame1Copy = imgFrame1.clone();
        imgFrame2Copy = imgFrame2.clone();

        cv::cvtColor( imgFrame1Copy, imgFrame1Copy, cv::COLOR_BGR2GRAY );
        cv::cvtColor( imgFrame2Copy, imgFrame2Copy, cv::COLOR_BGR2GRAY );
        cv::imshow( "original", imgFrame2 );
        cv::absdiff( imgFrame1Copy, imgFrame2Copy, imgDifference );

        cv::threshold( imgDifference,
                       imgThresh,
                       binarization_val,
                       255,
                       cv::THRESH_BINARY );

        imgThreshCopy = imgThresh.clone();
        cv::imshow( "binary", imgThreshCopy );
        cv::findContours( imgThreshCopy,
                          contours,
                          cv::RETR_EXTERNAL,
                          cv::CHAIN_APPROX_SIMPLE );

        for( auto &contours : contours )
        {
            Blob possibleBlob( contours );
            // Параметры объекта, для его захвата, нужно подгонять значения
            if ( possibleBlob._boundingRect.area() >  bounding_rect_area     &&
                 possibleBlob._dblAspectRatio      >= 0.0                    &&
                 possibleBlob._dblAspectRatio      <= dbl_aspect_ratio_upper && // important param
                 possibleBlob._boundingRect.width  >  bounding_rect_wh       && // размер объекта
                 possibleBlob._boundingRect.height >  bounding_rect_wh       && // размер объектар
                 possibleBlob._dblDiagonalSize     >  dbl_diagonal_size )
            {
                blobs.push_back( possibleBlob );
            }
        }

        imgFrame2Copy = imgFrame2.clone();
        for ( auto &blob : blobs )
            cv::rectangle( imgFrame2Copy,
                           blob._boundingRect,
                           cv::Scalar( 0, 255, 0 ),
                           2 );
        cv::imshow( "detection", imgFrame2Copy );
        imgFrame1 = imgFrame2.clone();

//         Cause lags.
//        cv::setTrackbarPos( "pos",
//                            "original",
//                            cap.get( cv::CAP_PROP_POS_FRAMES ) );

        blobs.clear();
        contours.clear();

        cap.read( imgFrame2 );
    }

    return EXIT_SUCCESS;
}
