#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>


int main( int argc, char** argv ) {
    bool capturing = true;
    int height;
    int width;
    int h_min = 0;
    int s_min = 0;
    int v_min = 0;
    int h_max = 179;
    int s_max = 255;
    int v_max = 255;
    bool show_hsv = false;
    int key;

     cv::VideoCapture cap(0);
     
    if(argc==3){
              
        height = atoi(argv[1]);
        width = atoi(argv[2]);
    } else {

        height = 320;
        width = 200;
    }

    if ( !cap.isOpened() ) {
        std::cerr << "error opening frames source" << std::endl;
        return -1;
    }
    std::cout << "Video size: " << cap.get( cv::CAP_PROP_FRAME_WIDTH ) 
    << "x" << cap.get( cv::CAP_PROP_FRAME_HEIGHT ) << std::endl;
    do {
        cv::Mat frame, captured_frame, hsv_frame, roi_frame;
        key = cv::waitKey( 1000.0/60.0 )&0x0ff; 
        if ( cap.read( frame ) ) {


            cv::imshow( "Lab2", frame );

            if(show_hsv){ 
                cv::cvtColor(frame, hsv_frame, cv::COLOR_BGR2HSV);
                cv::inRange(frame, cv::Scalar(h_min, s_min, v_min), cv::Scalar(h_max, s_max, v_max), hsv_frame);
                cv::imshow("HSV", hsv_frame); 
            }
        } else {

            capturing = false;
        }

        if( key == 27 ) capturing = false;
        if(key == 104 ){                         // 'h' -> hsv
            show_hsv = true;
            cv::createTrackbar("h_min", "HSV", &h_min, 179);
            cv::createTrackbar("s_min", "HSV", &s_min, 255);      
            cv::createTrackbar("v_min", "HSV", &v_min, 255);
            cv::createTrackbar("h_max", "HSV", &h_max, 179);
            cv::createTrackbar("s_max", "HSV", &s_max, 255);
            cv::createTrackbar("v_max", "HSV", &v_max, 255);                         
        }
        if(key == 32 ){                          // 'SPACE' -> rozmycie Gaussa
            
            captured_frame = frame;
            
            cv::GaussianBlur(captured_frame, captured_frame, cv::Size(0, 0), 10);

            cv::resize(captured_frame, captured_frame, cv::Size(height, width));
            cv::imshow("Captured frame", captured_frame);
            
        }
        if(key == 120 ){                     // 'x' -> roi

            auto roi = selectROI("Lab2", frame);
            roi_frame = frame(roi);
            cv::imshow("ROI", roi_frame);
            cv::imwrite("roi.jpg", roi_frame);
        }


    } while( capturing );

    return 0;
}