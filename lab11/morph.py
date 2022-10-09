#!/usr/bin/env python3

import cv2
import numpy as np

def imshow_with_text( text, img ):
    font_face = cv2.FONT_HERSHEY_SIMPLEX
    thickness = 1
    font_scale = 0.75
    color = (255, 0, 0)
    text_size = cv2.getTextSize( text, font_face, font_scale, thickness )
    test = img.shape[1]
    x = int( (img.shape[0] / 2 ) - (text_size[0][0] / 2) )
    y = int( img.shape[1]  + (text_size[0][1] / 2) )
    position = ( x, y )
    print( test )
    cv2.putText( img,
                 text,
                 position,
                 font_face,
                 font_scale,
                 color,
                 thickness,
                 cv2.LINE_AA )

    cv2.imshow( text, img )


img_orig = cv2.imread( '../imgs/morph_img.png', 0 )

cv2.imshow( "orig", img_orig )


kernel = np.ones( (5, 5), np.uint8 )

erosion = cv2.erode( img_orig, kernel, iterations = 1 )
cv2.imshow( "erosion", erosion )

dilation = cv2.dilate( img_orig, kernel, iterations = 1 )
cv2.imshow( "dilation", dilation )

opening = cv2.morphologyEx( img_orig, cv2.MORPH_OPEN, kernel )
cv2.imshow( "opening", opening )

closing = cv2.morphologyEx( img_orig, cv2.MORPH_CLOSE, kernel )
cv2.imshow( "closing", closing )

while True:
    if cv2.waitKey( 0 ) == 27:
       cv2.destroyAllWindows()
       break
