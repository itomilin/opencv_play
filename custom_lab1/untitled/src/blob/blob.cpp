#include "blob.h"

Blob::Blob( std::vector<cv::Point> contour )
{
    _contour = contour;
    _boundingRect = cv::boundingRect( _contour );

    _centerPosition.x = ( _boundingRect.x + _boundingRect.x +
                          _boundingRect.width ) / 2;

    _centerPosition.y = ( _boundingRect.y + _boundingRect.y +
                         _boundingRect.height ) / 2;

    _dblDiagonalSize = sqrt( pow( _boundingRect.width, 2 ) +
                             pow( _boundingRect.height, 2 ) );

    _dblAspectRatio = (float)_boundingRect.width / (float)_boundingRect.height;
}

