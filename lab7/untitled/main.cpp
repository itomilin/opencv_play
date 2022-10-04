#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
// For cvui
#define CVUI_IMPLEMENTATION
#include "cvui.h"
#define MODIFIED_NAME "Modified image"
#define SETTINGS_NAME "Settings"

void replacePixels( const cv::Mat& source,
                    cv::Mat& dest,
                    const uint8_t(&lowerBound)[3],
const uint8_t(&upperBound)[3] )
{
    uint8_t* pixelPtr = dest.data; // Получить массив пикселей из cv::Mat
    int destCn = dest.channels();
    int sourceCn = source.channels();
    for (int i = 0; i < dest.rows; i++) {
        for (int j = 0; j < dest.cols; j++) {
            // Получение отдельной составляющей
            uint8_t b = pixelPtr[i*dest.cols*destCn + j * destCn + 0];
            uint8_t g = pixelPtr[i*dest.cols*destCn + j * destCn + 1];
            uint8_t r = pixelPtr[i*dest.cols*destCn + j * destCn + 2];
            if (b >= lowerBound[0] && b <= upperBound[0]
                    && g >= lowerBound[1] && g <= upperBound[1]
                    && r >= lowerBound[2] && r <= upperBound[2]) {
                // Сама замена пикселей
                for (int k = 0; k < 3; k++) {
                    pixelPtr[i*dest.cols*destCn + j * destCn + k] =
                            source.data[i*source.cols*sourceCn + j * sourceCn + k];
                }
            }
        }
    }
}

int main()
{
    //    cv::VideoCapture cameraVideoStream(0);// Camera ID 0
    cv::VideoCapture cap( "../../imgs/highway_traffic720.mp4" );
    if ( !cap.isOpened() )
    {
        std::cout << "Could`n open source file." << std::endl;
        exit( EXIT_FAILURE );
    }

    cv::Mat cameraFrame; // Frame
    cv::Mat replacer;
    /***************Image window init*******************/
    cvui::init(MODIFIED_NAME);
    /*******************************************************/
    /***************Settings window init*******************/
    cv::Mat settingsFrame;
    settingsFrame.create(475, 200, 0);
    cvui::init(SETTINGS_NAME);

    bool useReplace = true;

    bool green_black_replace = false;
    bool white_replace      = false;

    // lowe 0 0 50 green |dark || 180 90 70 - white
    // upper 80 100 215 green|dark || 255 255 255 - white
    uint8_t lowerBound[3]{ 0, 0, 50 };
    uint8_t upperBound[3]{ 80, 100, 215 };

    uint8_t lower_bound_white[3]{ 160, 170, 0 };
    uint8_t upper_bound_white[3]{ 255, 255, 255 };

    uint8_t lower_bound_black_green[3]{ 0, 0, 50 };
    uint8_t upper_bound_black_green[3]{ 80, 100, 215 };

//    yellow_lower = np.array([20, 100, 100])
//       yellow_upper = np.array([30, 255, 255])
    /*******************************************************/
    while (true)
    {
        cameraFrame.release();
        cap.read(cameraFrame);
        /***************Settings window draw*******************/
        cvui::window(settingsFrame, 0, 0, 200, 475, "Pixel Replacing");

        cvui::checkbox( settingsFrame, 0, 25, "Use Replace", &useReplace );
        cvui::checkbox( settingsFrame, 0, 25 * 2, "white_color_replace", &white_replace );
        cvui::checkbox( settingsFrame, 0, 25 * 3, "dark_color_replace", &green_black_replace );
        if ( !replacer.data ) {
            cvui::text( settingsFrame, 100, 27, "Needs picture", 0.4, 0xFFCEFF );
            useReplace = false;
        }
        // Button for open file dialog

        std::string fileName = "../../imgs/red_wall.jpeg";
        // This realization works only with pictures, not video
        replacer = cv::imread(fileName);
        if (!replacer.data) {
            std::cerr << "Can't open source file";
            exit(1);
        }
        cv::resize(replacer, replacer, cv::Size(cameraFrame.cols,
                                                cameraFrame.rows), 0, 0, cv::INTER_CUBIC);
        //}
        // Trackbar start position
        int trackBarsY = 95;
        cvui::window(settingsFrame, 0, trackBarsY, 200, 190, "Lower bound");
        cvui::text(settingsFrame, 10, trackBarsY + 50, "b");
        cvui::trackbar<uint8_t>(settingsFrame, 20, trackBarsY + 35, 165, &lowerBound[0], 0, 255);
        cvui::text(settingsFrame, 10, trackBarsY + 100, "g");
        cvui::trackbar<uint8_t>(settingsFrame, 20, trackBarsY + 85, 165, &lowerBound[1], 0, 255);
        cvui::text(settingsFrame, 10, trackBarsY + 150, "r");
        cvui::trackbar<uint8_t>(settingsFrame, 20, trackBarsY + 135, 165, &lowerBound[2], 0, 255);
        //
        cvui::window(settingsFrame, 0, trackBarsY + 190, 200, 190, "Upper bound");
        cvui::text(settingsFrame, 10, trackBarsY + 240, "b");
        cvui::trackbar<uint8_t>(settingsFrame, 20, trackBarsY + 225, 165, &upperBound[0], 0, 255);
        cvui::text(settingsFrame, 10, trackBarsY + 290, "g");
        cvui::trackbar<uint8_t>(settingsFrame, 20, trackBarsY + 275, 165, &upperBound[1], 0, 255);
        cvui::text(settingsFrame, 10, trackBarsY + 340, "r");
        cvui::trackbar<uint8_t>(settingsFrame, 20, trackBarsY + 325, 165, &upperBound[2], 0, 255);
        //
        if ( white_replace ) {
            replacePixels( replacer, cameraFrame, lower_bound_white, upper_bound_white);
            cvui::trackbar<uint8_t>(settingsFrame, 20, trackBarsY + 35, 165, &lower_bound_white[0], 0, 255);
            cvui::trackbar<uint8_t>(settingsFrame, 20, trackBarsY + 85, 165, &lower_bound_white[1], 0, 255);
            cvui::trackbar<uint8_t>(settingsFrame, 20, trackBarsY + 135, 165, &lower_bound_white[2], 0, 255);
            cvui::trackbar<uint8_t>(settingsFrame, 20, trackBarsY + 225, 165, &upper_bound_white[0], 0, 255);
            cvui::trackbar<uint8_t>(settingsFrame, 20, trackBarsY + 275, 165, &upper_bound_white[1], 0, 255);
            cvui::trackbar<uint8_t>(settingsFrame, 20, trackBarsY + 325, 165, &upper_bound_white[2], 0, 255);
        }

        if ( green_black_replace ) {
            replacePixels( replacer, cameraFrame, lower_bound_black_green, upper_bound_black_green);
            cvui::trackbar<uint8_t>(settingsFrame, 20, trackBarsY + 35, 165, &lower_bound_black_green[0], 0, 255);
            cvui::trackbar<uint8_t>(settingsFrame, 20, trackBarsY + 85, 165, &lower_bound_black_green[1], 0, 255);
            cvui::trackbar<uint8_t>(settingsFrame, 20, trackBarsY + 135, 165, &lower_bound_black_green[2], 0, 255);
            cvui::trackbar<uint8_t>(settingsFrame, 20, trackBarsY + 225, 165, &upper_bound_black_green[0], 0, 255);
            cvui::trackbar<uint8_t>(settingsFrame, 20, trackBarsY + 275, 165, &upper_bound_black_green[1], 0, 255);
            cvui::trackbar<uint8_t>(settingsFrame, 20, trackBarsY + 325, 165, &upper_bound_black_green[2], 0, 255);
        }
        //
        cvui::update();
        cv::imshow(SETTINGS_NAME, settingsFrame);
        /*******************************************************/
        if (useReplace) {
            replacePixels( replacer, cameraFrame, lowerBound, upperBound );
        }
        cv::imshow(MODIFIED_NAME, cameraFrame);

        if ( cv::waitKey( 16 ) == 27 )
        {
            cv::destroyAllWindows();
            break;
        }
    }
    cameraFrame.release();
    replacer.release();
    return 0;
}
