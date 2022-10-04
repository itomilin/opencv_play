#include "player.h"

int create_window( std::string window_name,
                   int h_size,
                   int v_size,
                   int h_pos,
                   int v_pos )
{
    // Create the window
    cv::namedWindow( window_name, cv::WINDOW_NORMAL );
    //cv::resizeWindow( window_name, 600, 480 );
    cv::resizeWindow( window_name, h_size, v_size );
    //cv::moveWindow( window_name, 420, 210 );
    cv::moveWindow( window_name, h_pos, v_pos );
    return EXIT_SUCCESS;
}
