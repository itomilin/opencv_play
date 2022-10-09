#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>

void imshow_with_text( std::string text, cv::Mat& img )
{
    int font_face = cv::FONT_HERSHEY_DUPLEX;
    int thickness = 1;
    double font_scale = 0.75;
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

int main( int argc, char** argv )
{
    int MATRIX_SIZE = std::atoi( argv[1] );
    const char* IMG_PATH { "../../imgs/morph_img.png" };
    cv::Mat erosion,
            dilation,
            opening,
            closing,
            gradient;
    int kernel[MATRIX_SIZE];
    std::fill_n( kernel, MATRIX_SIZE, 1 );


    cv::Mat img_original = get_image_source( cv::IMREAD_COLOR, IMG_PATH );
    imshow_with_text( "original", img_original );

    cv::Mat kernel_matrix = cv::Mat( std::sqrt( MATRIX_SIZE ),
                                     std::sqrt( MATRIX_SIZE ),
                                     CV_8U,
                                     kernel );

    img_original = get_image_source( cv::IMREAD_COLOR, IMG_PATH );
    cv::erode( img_original, erosion, kernel_matrix );
    imshow_with_text( "erode", erosion );

    // It is just opposite of erosion. Here, a pixel element is '1' if at least one pixel under the kernel is '1'. So it increases the white region in the image or size
    //of foreground object increases. Normally, in cases like noise removal, erosion is followed by dilation. Because, erosion removes white noises, but it also shrinks
    // our object. So we dilate it. Since noise is gone, they won't come back, but our object area increases. It is also useful in joining broken parts of an object.
    img_original = get_image_source( cv::IMREAD_COLOR, IMG_PATH );
    cv::dilate( img_original, dilation, kernel_matrix );
    imshow_with_text( "dilation", dilation );

    //Opening is just another name of erosion followed by dilation. It is useful in removing noise, as we explained above. Here we use the function,
    img_original = get_image_source( cv::IMREAD_COLOR, IMG_PATH );
    cv::morphologyEx( img_original, opening, cv::MORPH_OPEN, kernel_matrix );
    imshow_with_text( "opening", opening );

    // Closing is reverse of Opening, Dilation followed by Erosion. It is useful in closing small holes inside the foreground objects,
    // or small black points on the object.
    img_original = get_image_source( cv::IMREAD_COLOR, IMG_PATH );
    cv::morphologyEx( img_original, closing, cv::MORPH_CLOSE, kernel_matrix );
    imshow_with_text( "closing", closing );

    // It is the difference between dilation and erosion of an image.
    img_original = get_image_source( cv::IMREAD_COLOR, IMG_PATH );
    cv::morphologyEx( img_original, gradient, cv::MORPH_GRADIENT, kernel_matrix );
    imshow_with_text( "gradient", gradient );

    press_esc_to_close( 0 );

    return EXIT_SUCCESS;
}
