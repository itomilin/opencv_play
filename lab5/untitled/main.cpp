#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

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

void gaussian_filter( cv::Mat image, cv::Size size, std::string text )
{
    cv::Mat image_blurred;
    cv::GaussianBlur( image, image_blurred, size, 0 );

    imshow_with_text( text, image_blurred );
}

int main()
{
    cv::Mat source_img = get_image_source( cv::IMREAD_COLOR, "../../imgs/img1.jpeg" );
    imshow_with_text( "original", source_img );

    source_img = get_image_source( cv::IMREAD_COLOR, "../../imgs/img1.jpeg" );
    gaussian_filter( source_img, cv::Size( 3, 3 ), "Gaussian_filter_3x3_kernel" );

    source_img = get_image_source( cv::IMREAD_COLOR, "../../imgs/img1.jpeg" );
    gaussian_filter( source_img, cv::Size( 5, 5 ), "Gaussian_filter_5x5_kernel" );

    source_img = get_image_source( cv::IMREAD_COLOR, "../../imgs/img1.jpeg" );
    gaussian_filter( source_img, cv::Size( 9, 9 ), "Gaussian_filter_9x9_kernel" );
    press_esc_to_close( 0 );

    press_esc_to_close( 0 );

    return EXIT_SUCCESS;
}
