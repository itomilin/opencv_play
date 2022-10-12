#include <opencv2/highgui.hpp>

#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudafilters.hpp>

#include <iostream>

void gpu_median_blur( cv::cuda::GpuMat& src, cv::cuda::GpuMat& dst, const int kernel )
{
    cv::Ptr<cv::cuda::Filter> median_ptr = cv::cuda::createMedianFilter( src.type(), kernel );
    median_ptr->apply( src, dst );
}

void cpu_median_blur( cv::Mat& src, cv::Mat& dst, const int kernel )
{
     cv::medianBlur( src, dst, kernel );
}

int main()
{
    auto constexpr kernel = 31;

#ifdef GPU
    cv::cuda::setDevice( 0 );
    cv::cuda::printCudaDeviceInfo( cv::cuda::getDevice() );
    int cuda_devices_number = cv::cuda::getCudaEnabledDeviceCount();
    std::cout << "CUDA Device(s) Number: " << cuda_devices_number << std::endl;

    cv::cuda::GpuMat dst_cuda,
                     src_cuda;
#endif

    cv::Mat frame,
            frame_grayscale,
            result_frame;

    // Read video
    cv::VideoCapture video( "../../imgs/highway_traffic.mp4" );

    // Exit if video is not opened
    if( !video.isOpened() )
    {
    std::cout << "Could not read video file." << std::endl;
        return EXIT_FAILURE;
    }

    auto cap_fps = video.get( cv::CAP_PROP_FPS );
    std::cout << "CAP_FPS: " << cap_fps << std::endl;
    do
    {
        video >> frame;

    cv::cvtColor( frame, frame_grayscale, cv::COLOR_BGR2GRAY );

        // Start timer
        double start = (double)cv::getTickCount();


#ifdef GPU
    src_cuda.upload( frame_grayscale );
    gpu_median_blur( src_cuda, dst_cuda, kernel );
    dst_cuda.download( result_frame );
#endif

#ifdef CPU
        cpu_median_blur( frame_grayscale, result_frame, kernel );
#endif

        // Calculate Frames per second (FPS)
        float fps = cv::getTickFrequency() / (cv::getTickCount() - start);


    std::cout << "FPS: " << std::to_string( int(fps) ) << std::endl;
    // Display FPS on frame
    //cv::putText( frame, "REAL_FPS : " + std::to_string(int(fps)),
    //cv::Point(100, 30), cv::FONT_HERSHEY_SIMPLEX, 1.1,
    //cv::Scalar( 0, 0, 255), 4 );

    // Display frame.
    //cv::imshow( "Tracking", result_frame );

    } while( !frame.empty() && cv::waitKey( 1000.0 / cap_fps ) != 27 );

    return EXIT_SUCCESS;
}


