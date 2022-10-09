#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

const int GREEN_MIN = 21;
const int GREEN_MAX = 110;

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
//            std::cout << H << "|" << S << "|" << V << std::endl;
            }
        else if (H < 34) {
            color = cYELLOW;
//            std::cout << H << "|" << S << "|" << V << std::endl;
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

int main(int argc, char** argv) {
    Mat src = imread("../../imgs/img5.jpeg"); //Исходное изображение

    imshow("input", src);
//    imwrite("src.jpg", src);
    //Переводим в формат HSV
    Mat hsv = Mat(src.cols, src.rows, 8, 3); //
    std::vector<Mat> splitedHsv = std::vector<Mat>();
    cvtColor(src, hsv, COLOR_BGR2HSV);
    split(hsv, splitedHsv);

    //Удаляем фон

    for (int y = 0; y < hsv.cols; y++) {
        for (int x = 0; x < hsv.rows; x++) {

            // получаем HSV-компоненты пикселя
            int H = static_cast<int>(splitedHsv[0].at<uchar>(x, y));        // Тон
            int S = static_cast<int>(splitedHsv[1].at<uchar>(x, y));        // Интенсивность
            int V = static_cast<int>(splitedHsv[2].at<uchar>(x, y));        // Яркость

            if ( cGREEN == getPixelColorType(H, S, V) ) {
                src.at<Vec3b>(x, y)[0] = 0;
                src.at<Vec3b>(x, y)[1] = 255;
                src.at<Vec3b>(x, y)[2] = 0;
            } else if ( cBLUE == getPixelColorType(H, S, V) ) {
                src.at<Vec3b>(x, y)[0] = 255;
                src.at<Vec3b>(x, y)[1] = 0;
                src.at<Vec3b>(x, y)[2] = 0;
            } else if ( cBLACK == getPixelColorType(H, S, V) ) {
                src.at<Vec3b>(x, y)[0] = 0;
                src.at<Vec3b>(x, y)[1] = 0;
                src.at<Vec3b>(x, y)[2] = 0;
            } else if ( cWHITE == getPixelColorType(H, S, V) ) {
                src.at<Vec3b>(x, y)[0] = 255;
                src.at<Vec3b>(x, y)[1] = 255;
                src.at<Vec3b>(x, y)[2] = 255;
            } else if ( cRED == getPixelColorType(H, S, V) ) {
                src.at<Vec3b>(x, y)[0] = 0;
                src.at<Vec3b>(x, y)[1] = 0;
                src.at<Vec3b>(x, y)[2] = 255;
            } else if ( cORANGE == getPixelColorType(H, S, V) ) {
                src.at<Vec3b>(x, y)[0] = 0;
                src.at<Vec3b>(x, y)[1] = 130;
                src.at<Vec3b>(x, y)[2] = 255;
            } else if ( cYELLOW == getPixelColorType(H, S, V) ) {
            src.at<Vec3b>(x, y)[0] = 30;
            src.at<Vec3b>(x, y)[1] = 237;
            src.at<Vec3b>(x, y)[2] = 255;
            } else if ( cGREY == getPixelColorType(H, S, V) ) {
                src.at<Vec3b>(x, y)[0] = 162;
                src.at<Vec3b>(x, y)[1] = 162;
                src.at<Vec3b>(x, y)[2] = 162;
            } else if ( cWHITE == getPixelColorType(H, S, V) ) {
                src.at<Vec3b>(x, y)[0] = 0;
                src.at<Vec3b>(x, y)[1] = 0;
                src.at<Vec3b>(x, y)[2] = 0;
            }
        }
    }
    Mat tmp;

    //Морфологическое замыкание для удаления остаточных шумов.
    int an = 5;
    Mat element = getStructuringElement(MORPH_ELLIPSE, Size(an * 2 + 1, an * 2 + 1), Point(an, an));
    dilate(src, tmp, element);
    erode(tmp, tmp, element);

    //Переводим изображение в чернобелый формат
    Mat grayscaleMat;
    cvtColor(tmp, grayscaleMat, COLOR_BGR2GRAY);

//    //Делаем бинарную маску
    Mat mask(grayscaleMat.size(), grayscaleMat.type());
    Mat out(src.size(), src.type());
    threshold(grayscaleMat, mask, 200, 255, THRESH_BINARY_INV);

//    //Финальное изображение редварительно красим в белый цвет
    out = Scalar::all(255);
//    //Копируем зашумленное изображение через маску
    src.copyTo(out, mask);

    imshow("До замыкания", src);
    imshow("Результат замыкания", tmp);
//    imshow("Результат", out);

    press_esc_to_close( 0 );

    return EXIT_SUCCESS;
}
