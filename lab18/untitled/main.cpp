#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core_c.h"

#include <stdlib.h>
#include <stdio.h>

#include <algorithm>
#include <vector>
#include <iostream>

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace cv;
using namespace std;



#define RECT_COLORS_SIZE 10
// получение пикселя изображения (по типу картинки и координатам)
#define CV_PIXEL( type,img,x,y ) ( ( (type*)((img).data +(y) * (img).step) ) + (x) * (img).channels() )
enum { cBLACK = 0,
       cWHITE,
       cGREY,
       cRED,
       cORANGE,
       cYELLOW,
       cGREEN,
       cAQUA,
       cBLUE,
       cPURPLE,
       NUM_COLOR_TYPES };
char* sCTypes[NUM_COLOR_TYPES] = { "Black",
                                   "White",
                                   "Grey",
                                   "Red",
                                   "Orange",
                                   "Yellow",
                                   "Green",
                                   "Aqua",
                                   "Blue",
                                   "Purple" };

uchar cCTHue[NUM_COLOR_TYPES] = { 0, 0, 0,
                                  0, 20, 30,
                                  60, 85, 120, 138 };
uchar cCTSat[NUM_COLOR_TYPES] = { 0, 0, 0,
                                  255, 255, 255,
                                  255, 255, 255, 255 };
uchar cCTVal[NUM_COLOR_TYPES] = { 0, 255, 120,
                                  255, 255, 255,
                                  255, 255, 255, 255 };
typedef unsigned int uint;
// число пикселей данного цвета на изображении
uint colorCount[NUM_COLOR_TYPES] = { 0, 0, 0,
                                     0, 0, 0,
                                     0, 0, 0 };

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


int getPixelColorType(int H, int S, int V)
{
    int color = cBLACK;
#if 1
//    if ( S < 25 )
//    {
        if (V < 75){

            color = cBLACK;
//              std::cout << H << "|" << S << "|" << V << std::endl;
        }
        else if ( V > 190 && S < 27 )
            color = cWHITE;
        else if (S < 53 && V < 185) {
            color = cGREY;
        }
//    }
    else
#endif
    {
//        if (S == 0 && H == 0)
//        {
//            color = cBLACK;
//            std::cout << H << "|" << S << "|" << V << std::endl;
//        }
//        else if ( H > 170 && V > 100 && S > 100)
//            color = cBLACK;
        /*else*/ if (H < 7) {
//            std::cout << H << "|" << S << "|" << V << std::endl;
            color = cRED;
        }
        else if (H < 25) {
            color = cORANGE;
            std::cout << H << "|" << S << "|" << V << std::endl;
            }
        else if (H < 34) {
            color = cYELLOW;
            std::cout << H << "|" << S << "|" << V << std::endl;
            }
        else if (H < 73)
            color = cGREEN;
        else if (H < 102)
            color = cAQUA;
        else if (H < 140)
            color = cBLUE;
        else if (H < 170) {
            color = cPURPLE;
            std::cout << H << "|" << S << "|" << V << std::endl;
            }
        else
            color = cRED;
    }
    return color;
}
// сортировка цветов по количеству
bool colors_sort(std::pair< int, uint > a, std::pair< int, uint > b)
{
    return (a.second > b.second);
}
int main(int argc, char* argv[])
{
    // для хранения изображения
    Mat image,
            hsv,
            dst,
            color_indexes;

    // получаем картинку
    image = get_image_source( cv::IMREAD_COLOR, "../../imgs/green_wall.png" );

    //    cvShowImage("image", image);
    // преобразуем изображение в HSV
    ///hsv = Mat( image, CV_8UC3 );
    cvtColor( image, hsv, COLOR_BGR2HSV );
    // картинки для хранения результатов
    dst = image.clone();
    color_indexes = image.clone();

    // для хранения RGB-х цветов
    Scalar rgb_colors[NUM_COLOR_TYPES];
    int i = 0, j = 0, x = 0, y = 0;

    // обнуляем цвета
    for (i = 0; i<NUM_COLOR_TYPES; i++) {
        rgb_colors[i] = cv::Scalar::all(0);
    }
    std::cout << "All pixels: " << hsv.cols * hsv.rows << std::endl;

    for (y = 0; y < hsv.cols; y++) {
        for (x = 0; x< hsv.rows; x++) {
            // получаем HSV-компоненты пикселя
            uchar H = CV_PIXEL(uchar, hsv, x, y)[0];
            uchar S = CV_PIXEL(uchar, hsv, x, y)[1];
            uchar V = CV_PIXEL(uchar, hsv, x, y)[2];
            // Hue
            // Saturation
            // Value
            // определяем к какому цвету можно отнести данные значения
            int ctype = getPixelColorType(H, S, V);

            // подсчитываем
            colorCount[ctype]++;
        }
    }

    //    // усреднение RGB-составляющих
    //    for (i = 0; i<NUM_COLOR_TYPES; ++i ) {
    //        rgb_colors[i].val[0] /= colorCount[i];
    //        rgb_colors[i].val[1] /= colorCount[i];
    //        rgb_colors[i].val[2] /= colorCount[i];
    //    }

    // теперь загоним массив в вектор и отсортируем
    std::vector< std::pair< int, uint > > colors;
    colors.reserve(NUM_COLOR_TYPES);

    for (i = 0; i<NUM_COLOR_TYPES; i++) {
        std::pair< int, uint > color;
        color.first = i;
        color.second = colorCount[i];
        colors.push_back(color);
    }
    // сортируем
    std::sort(colors.begin(), colors.end(), colors_sort);

    // для отладки - выводим коды, названия цветов и их количество
    int sum = 0;
    for (i = 0; i < colors.size(); i++) {
        printf("[i] color %d (%s) - %d\n", colors[i].first,
               sCTypes[colors[i].first], colors[i].second);
        sum += colors[i].second;
    }
    std::cout << "Count of detected pixels: " << sum << std::endl;
    // выдаём код первых цветов
    printf("[i] color code: \n");
    for (i = 0; i<NUM_COLOR_TYPES; i++)
        printf("%02d ", colors[i].first);

    printf("\n[i] color names: \n");

    for (i = 0; i<NUM_COLOR_TYPES; i++)
        printf("%s ", sCTypes[colors[i].first]);
    printf("\n");

    // конвертируем отладочную картинку обратно в RGB
    //    cvtColor(dst, dst, COLOR_HSV2BGR);

    // показываем результат
    imshow_with_text( "color", dst );

    // ждём нажатия клавиши
    press_esc_to_close( 0 );

    return EXIT_SUCCESS;
}
