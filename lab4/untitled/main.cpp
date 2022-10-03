#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

void imshow_with_text( cv::Mat& img, std::string text )
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
    cv::Mat img = cv::Mat();
    while (img.empty()) {
        img = imread( path, mode );
        if ( img.empty() ) {
            std::cout << "Error reading the image. Try another one.\n";
        }
    }

    return img;
}

void apply_filter( cv::Mat source_img, char anchor )
{
    cv::Mat dst = source_img.clone();

    float kernel[9];
    kernel[0] = -1;
    kernel[1] = -1;
    kernel[2] = -1;
    kernel[3] = -1;
    kernel[4] = anchor;
    kernel[5] = -1;
    kernel[6] = -1;
    kernel[7] = -1;
    kernel[8] = -1;

    cv::Mat kernel_matrix = cv::Mat( 3, 3, CV_32FC1, kernel );
    cv::filter2D( source_img, dst, dst.depth(), kernel_matrix, cv::Point(-1, -1) );

    imshow_with_text( source_img, "original" );
    imshow_with_text( dst, cv::format("anchor_value = %d", anchor) );

    press_esc_to_close( 0 );
}

void apply_border_filter( cv::Mat source_img, char border_size )
{
    cv::Mat dst = source_img.clone();
    cv::Mat dst2 = source_img.clone();

    cv::copyMakeBorder( source_img, dst,
                        border_size, border_size, border_size, border_size,
                        cv::BORDER_CONSTANT, cv::Scalar(250) );
    cv::copyMakeBorder( source_img, dst2,
                        border_size, border_size, border_size, border_size,
                        cv::BORDER_WRAP, cv::Scalar(250) );

    imshow_with_text( source_img, "original" );
    imshow_with_text( dst, cv::format("BORDER_CONSTANT|border_size = %d", border_size) );
    imshow_with_text( dst2, cv::format("BORDER_WRAP|border_size = %d", border_size) );

    press_esc_to_close( 0 );
}

int main( int argc, char** argv )
{
    // -----------------------------------------
    cv::Mat source_img = get_image_source( cv::IMREAD_COLOR, "../../imgs/img3.png" );
    apply_filter( source_img, 8 );

    source_img = get_image_source( cv::IMREAD_COLOR, "../../imgs/img3.png" );
    apply_filter( source_img, 10 );

    // -----------------------------------------

    source_img = get_image_source( cv::IMREAD_COLOR, "../../imgs/img3.png" );
    apply_border_filter( source_img, 10 );

    source_img = get_image_source( cv::IMREAD_COLOR, "../../imgs/img3.png" );
    apply_border_filter( source_img, 50 );

    return EXIT_SUCCESS;
}
