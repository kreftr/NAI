#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
int main( int argc, char** argv ) {
    bool capturing = true;
    // Question for you
    //cv::VideoCapture cap( "test.webm" );
     cv::VideoCapture cap(0);
     cv::VideoCapture cap2(0);
    if ( !cap.isOpened() ) {
        std::cerr << "error opening frames source" << std::endl;
        return -1;
    }
    std::cout << "Video size: " << cap.get( cv::CAP_PROP_FRAME_WIDTH ) 
    << "x" << cap.get( cv::CAP_PROP_FRAME_HEIGHT ) << std::endl;
    do {
        cv::Mat frame, frame2;

        if ( cap.read( frame ) ) {
            cv::imshow( "Not-yet smart windown", frame );
            cv::flip(frame, frame2, 1);
            cv::imshow("Okno2", frame2);
        } else {
            // stream finished
            capturing = false;
        }
        //czekaj na klawisz, sprawdz czy to jest 'esc'
        if( (cv::waitKey( 1000.0/60.0 )&0x0ff) == 27 ) capturing = false;
    } while( capturing );
    return 0;
}