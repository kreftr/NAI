#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

// g++ `pkg-config --cflags opencv4` cv.cpp `pkg-config --libs opencv4`

using namespace std;
using namespace cv;

double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1 * dx2 + dy1 * dy2) / sqrt((dx1 * dx1 + dy1 * dy1) * (dx2 * dx2 + dy2 * dy2) + 1e-10);
}


int main( int argc, char** argv ) {
    
    int canny_a = 90, canny_b = 190;
    int scan_counter = 0;
    //a = 110 b = 210
    //a = 122 b = 255

    int h_min = 0;
    int s_min = 170;//106
    int v_min = 130;//137
    int h_max = 255;
    int s_max = 255;
    int v_max = 255;

    namedWindow("ustawienia", cv::WINDOW_AUTOSIZE);
    createTrackbar("A", "ustawienia", &canny_a, 255);
    createTrackbar("B", "ustawienia", &canny_b, 255);


    cv::namedWindow("orange", cv::WINDOW_AUTOSIZE );    

    cv::createTrackbar("h_min", "orange", &h_min, 255);
    cv::createTrackbar("s_min", "orange", &s_min, 255);      
    cv::createTrackbar("v_min", "orange", &v_min, 255);
    cv::createTrackbar("h_max", "orange", &h_max, 255);
    cv::createTrackbar("s_max", "orange", &s_max, 255);
    cv::createTrackbar("v_max", "orange", &v_max, 255); 

    cv::VideoCapture cap(2);

    while(waitKey(1) != 27){

        Mat frame, gray, canny, result, orange;
        cap >> frame;
        flip(frame,frame,1);
        result = frame.clone();
        // imshow("Window",frame);


        int dilation_size = 5;
        auto structElem = cv::getStructuringElement(cv::MORPH_ELLIPSE,
                            cv::Size(2*dilation_size+1, 2*dilation_size+1),
                            cv::Point(dilation_size, dilation_size)); 
        cv::cvtColor(frame, orange, cv::COLOR_BGR2HSV);
        cv::inRange(orange, cv::Scalar(h_min, s_min, v_min), cv::Scalar(h_max, s_max, v_max), orange);
        cv::morphologyEx(orange, orange, cv::MORPH_CLOSE, structElem);
        cv::morphologyEx(orange, orange, cv::MORPH_OPEN, structElem);
        imshow("Orange window", orange);


        cvtColor(frame,gray,COLOR_BGR2GRAY);
        equalizeHist(gray, gray);
        // imshow("Gray", gray);

        Canny(gray, canny, 16, 32, 3);
        //imshow("Canny", canny);
        static auto ellipse = getStructuringElement(MORPH_ELLIPSE, Size(14,14));
        
        morphologyEx(gray, gray, MORPH_CLOSE, ellipse);
        morphologyEx(gray, gray, MORPH_OPEN, ellipse);
        
        Canny(gray, canny, canny_a, canny_b, 3);
        imshow("Canny_fixed", canny);
        morphologyEx(gray, gray, MORPH_DILATE, ellipse);

        vector<vector<Point>> contours;
        vector<vector<Point>> orange_contours;
        vector<vector<Point>> rectangles;
        findContours(canny, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
        findContours(orange, orange_contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

        sort(orange_contours.begin(), orange_contours.end(),
			[](auto &a, auto &b) {
			return contourArea(a, false) > contourArea(b, false);
		});

        for(int i = 0; i < contours.size(); i++){
            approxPolyDP(contours[i], contours[i], 10, true);
            if(contours[i].size() == 4 && contourArea(contours.at(i),false)>25000){
                double amax = 0;
                for(int j = 0; j < 4; j++){
                    double a = angle(contours[i][j], contours[i][(j+1)%4], contours[i][(j+1)%4]);
                    amax = max(a,amax);
                }
                if(amax<0.4) rectangles.push_back(contours[i]);
                
                sort(rectangles.begin(), rectangles.end(),
				    [](auto &a, auto &b) {
					    return contourArea(a, true) > contourArea(b, true);
			        });
            }
        }

        cv::Point2f orange_center;

        if(orange_contours.size() > 0)
        {
            if(contourArea(orange_contours.at(0)) > 500)
            {
                approxPolyDP(orange_contours[0], orange_contours[0], 10, true);
                cv::Rect r = cv::boundingRect(orange_contours.at(0));
                orange_center.x = r.x+r.width/2;
                orange_center.y = r.y+r.height/2;
                putText(frame,"X",orange_center,cv::FONT_HERSHEY_PLAIN,2,{0,0,255,0});
                drawContours(frame, orange_contours, 0, Scalar(0,0,255));
            }
        }

        if(rectangles.size() > 0){

            cv::Point rectangle_center;
            cv::Rect r = cv::boundingRect(rectangles.at(0));
            rectangle_center.x = r.x+r.width/2;
            rectangle_center.y = r.y+r.height/2;
            putText(frame,"X",rectangle_center,cv::FONT_HERSHEY_PLAIN,2,{0,255,0,0});
            drawContours(frame, rectangles, 0, Scalar(255,255,255), 3);


            if(pointPolygonTest(rectangles[0], orange_center, false) == 1)
            {
                Mat dstMat(Size(400,600),CV_8UC3);
                vector<Point2f> src = {{0,0},{dstMat.cols,0},{dstMat.cols,dstMat.rows},{0,dstMat.rows}};
                vector<Point2f> dst;
                for(auto p: rectangles[0]) dst.push_back(Point2f(p.x,p.y));
                auto warp_mtx = getPerspectiveTransform(dst,src);
                warpPerspective(result, dstMat, warp_mtx, Size(dstMat.cols, dstMat.rows));


                if(orange_center.y<rectangle_center.y){
                    rotate(dstMat,dstMat,ROTATE_90_CLOCKWISE);
                    rotate(dstMat,dstMat,ROTATE_90_CLOCKWISE);
                    imwrite("scan_"+to_string(scan_counter)+".jpg", dstMat);
                    scan_counter++;
                }
                else
                {
                    imwrite("scan_"+to_string(scan_counter)+".jpg", dstMat);
                    scan_counter++;   
                }
                if(scan_counter == 5) return 0;
                imshow("RESULT", dstMat);
            }
             
            
        }
        flip(frame,frame,1);
        imshow("contours",frame);
    }

    return 0;
}