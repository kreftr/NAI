#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>


int main( int argc, char** argv ) {
    bool capturing = true;
    int key;
    cv::Mat frame, frameRange;
    cv::VideoCapture cap(0);
     
    int h_min = 0;
    int s_min = 172;
    int v_min = 141;
    int h_max = 255;
    int s_max = 255;
    int v_max = 255;


    if ( !cap.isOpened() ) {
        std::cerr << "error opening frames source" << std::endl;
        return -1;
    }
    std::cout << "Video size: " << cap.get( cv::CAP_PROP_FRAME_WIDTH ) 
    << "x" << cap.get( cv::CAP_PROP_FRAME_HEIGHT ) << std::endl;

    cv::namedWindow("settings", cv::WINDOW_AUTOSIZE );    

    cv::createTrackbar("h_min", "settings", &h_min, 255);
    cv::createTrackbar("s_min", "settings", &s_min, 255);      
    cv::createTrackbar("v_min", "settings", &v_min, 255);
    cv::createTrackbar("h_max", "settings", &h_max, 255);
    cv::createTrackbar("s_max", "settings", &s_max, 255);
    cv::createTrackbar("v_max", "settings", &v_max, 255); 

    do {

        int dilation_size = 5;
        auto structElem = cv::getStructuringElement(cv::MORPH_ELLIPSE,
                                                    cv::Size(2*dilation_size+1, 2*dilation_size+1),
                                                    cv::Point(dilation_size, dilation_size));

        key = cv::waitKey( 1000.0/60.0 )&0x0ff; 

        if ( cap.read( frame ) ) {
            
            cv::flip(frame, frame, 1);
        
            cv::cvtColor(frame, frameRange, cv::COLOR_BGR2HSV);
            cv::inRange(frameRange, cv::Scalar(h_min, s_min, v_min), cv::Scalar(h_max, s_max, v_max), frameRange);
            cv::morphologyEx(frameRange, frameRange, cv::MORPH_CLOSE, structElem);
            cv::morphologyEx(frameRange, frameRange, cv::MORPH_OPEN, structElem);
            
            
            std::vector<std::vector<cv::Point>> contours;
            cv::Point pt, pt2;

            cv::findContours(frameRange, contours, cv::RETR_LIST, cv::CHAIN_APPROX_TC89_KCOS);

            std::sort(contours.begin(), contours.end(),
                [](auto &a, auto &b){
                    return contourArea(a,false) > contourArea(b,false);
                });

            for(int i = 0; i < contours.size(); i++){

                if((i==0||i==1)&&cv::contourArea(contours.at(i), false)>500){

                    cv::approxPolyDP(contours.at(i),contours.at(i),2,true);
                    cv::drawContours(frame,contours,i,{255,0,0,255},3);
                    cv::putText(frame,std::to_string(contours.at(i).size()),contours.at(i).at(0),cv::FONT_HERSHEY_PLAIN,2,{255,0,0,255});

                    cv::Point txtpos = contours.at(i).at(0);
                    txtpos.y += 30;
                    cv::putText(frame,"Area:"+std::to_string(cv::contourArea(contours.at(i), false)),txtpos,cv::FONT_HERSHEY_PLAIN,2,{0,255,0,0});
                    txtpos.y -= 60;
                    cv::putText(frame,"Index:"+std::to_string(i),txtpos,cv::FONT_HERSHEY_PLAIN,2,{255,0,0,0});


                    cv::Point avg;
                    cv::Rect r = cv::boundingRect(contours.at(i));
                    avg.x = r.x+r.width/2;
                    avg.y = r.y+r.height/2;
                    putText(frame,"O",avg,cv::FONT_HERSHEY_PLAIN,2,{0,255,255,255});


                    if(i == 0){
                        pt.x = avg.x;
                        pt.y = avg.y;
                    }
                    else{
                        pt2.x = avg.x;
                        pt2.y = avg.y;
                    }


                    txtpos.y -= 30;
                    putText(frame,"y: "+std::to_string(avg.y),txtpos,cv::FONT_HERSHEY_PLAIN,2,{0,255,255,255});
                    

                    if(pt2.y!=0&&pt2.y<pt.y+20&&pt2.y>pt.y-20)cv::line(frame, pt, pt2, {255, 255, 100, 0}, 2);

                }
            }
   


            cv::imshow("contours",frame);

        } else {
            capturing = false;
        }

        
    if( key == 27 ) capturing = false;
    } while( capturing );

    return 0;
}