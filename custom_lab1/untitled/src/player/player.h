#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <string>

int create_window( std::string window_name,
                   int         h_size,
                   int         v_size,
                   int         h_pos,
                   int         v_pos );

#endif // __PLAYER_H__

