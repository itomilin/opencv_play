#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
cv::iplIplImage* image = 0;
IplImage* dst = 0;
int main(int argc, char* argv[])
{
    // имя картинки задаётся первым параметром

    char* filename = argc == 2 ? argv[1] : "Image3.jpg";
    // получаем картинку
    image = cvLoadImage(filename, 1);
    printf("[i] image: %s\n", filename);
    assert(image != 0);
    // клонируем картинку
    dst = cvCloneImage(image);
    // окно для отображения картинки
    cvNamedWindow("original", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("cvFilter2D", CV_WINDOW_AUTOSIZE);
    float kernel[9];
    kernel[0] = -1;
    kernel[1] = -1;
    kernel[2] = -1;
    kernel[3] = -1;
    kernel[4] = 9;
    kernel[5] = -1;
    kernel[6] = -1;
    kernel[7] = -1;
    kernel[8] = -1;
    // матрица
    CvMat kernel_matrix = cvMat(3, 3, CV_32FC1, kernel);
    // накладываем фильтр
    cvFilter2D(image, dst, &kernel_matrix, cvPoint(-1, -1));
    // показываем картинку
    cvShowImage("original", image);
    cvShowImage("cvFilter2D", dst);
    // ждём нажатия клавиши
    cvWaitKey(0);
    // освобождаем ресурсы
    cvReleaseImage(&image);
    cvReleaseImage(&dst);
    // удаляем окна
    cvDestroyAllWindows();
    return 0;
}
