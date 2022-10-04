#ifndef __MY_BLOB__
#define __MY_BLOB__

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class Blob
{
public:
    std::vector< cv::Point > _contour;
    cv::Rect  _boundingRect;
    cv::Point _centerPosition;
    double _dblDiagonalSize;
    double _dblAspectRatio;

    Blob( std::vector<cv::Point> _contour );
};
#endif // __MY_BLOB__

