#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/tracking/tracking.hpp"

#include <iostream>

using namespace std;
using namespace cv;

int main( int argc, char **argv )
{
    std::string trackerTypes[8] = {"BOOSTING", "MIL", "KCF", "TLD","MEDIANFLOW", "GOTURN", "MOSSE", "CSRT"};

    // Create a tracker
    std::string trackerType = trackerTypes[std::atoi(argv[1])];

    cv::Ptr<cv::Tracker> tracker;

    if (trackerType == "BOOSTING")
        tracker = TrackerBoosting::create();
    if (trackerType == "MIL")
        tracker = TrackerMIL::create();
    if (trackerType == "KCF")
        tracker = TrackerKCF::create();
    if (trackerType == "TLD")
        tracker = TrackerTLD::create();
    if (trackerType == "MEDIANFLOW")
        tracker = TrackerMedianFlow::create();
    if (trackerType == "GOTURN")
        tracker = TrackerGOTURN::create();
    if (trackerType == "MOSSE")
        tracker = TrackerMOSSE::create();
    if (trackerType == "CSRT")
        tracker = TrackerCSRT::create();

    // Read video
    VideoCapture video( "../../imgs/traffic.mp4" );

    // Exit if video is not opened
    if(!video.isOpened())
    {
        cout << "Could not read video file" << endl;
        return 1;
    }

    // Read first frame
    Mat frame;
    video >> frame;

    // Define initial bounding box
    // y x  w_box h_box
    int w_box = 170,
        h_box = 130;
    Rect2d bbox( 1000, 50, w_box, h_box );

    rectangle(frame, bbox, Scalar( 0, 0, 255 ), 2, 1 );

    tracker->init(frame, bbox);

    vector<Point> contours;

    auto cap_fps = video.get( cv::CAP_PROP_FPS );
    while( !frame.empty() && cv::waitKey( 1000.0 / cap_fps ) != 27 )
    {
        // Start timer
        double start = (double)getTickCount();

        if ( tracker->update(frame, bbox) ) {
            rectangle( frame, bbox, Scalar( 0, 0, 255 ), 2, 1 );

            // Collect points for the trace.
            if (std::find(contours.begin(), contours.end(), Point(bbox.x, bbox.y)) == contours.end()) {
                contours.emplace_back( Point(bbox.x + (w_box * 0.5), bbox.y + (h_box * 0.5)) );
            }

            // Draw a trace.
            for ( auto& contour : contours )
            {
                cv::drawMarker(frame, contour,  cv::Scalar(0, 0, 255), MARKER_CROSS, 10, 1);
            }
        }
        else
            cv::putText(frame, "Tracking failure detected", Point(100,80), cv::FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);

        // Calculate Frames per second (FPS)
        float fps = getTickFrequency() / (getTickCount() - start);

        // Display tracker type on frame
        cv::putText(frame, trackerType + " Tracker", cv::Point(100, 150), cv::FONT_HERSHEY_SIMPLEX, 2.0, Scalar(0, 0, 255), 6);

        // Display FPS on frame
        cv::putText(frame, "REAL_FPS : " + std::to_string(int(fps)), cv::Point(100, 30), cv::FONT_HERSHEY_SIMPLEX, 1.1, Scalar( 0, 0, 255), 4);

        // Display frame.
        cv::imshow( "Tracking", frame );

        video >> frame;
    }
}
