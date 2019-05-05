#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include <mutex>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "serialer.h"

#define bigdiff(x) ((x) < 100 &&(x) > -100)
void find(cv::Mat& frame,data& point1,data& point2);

data point1_pre,point2_pre,point1,point2;

int main(int argc,char** argv) {

    string m = "/dev/pts/19";
    if(argc != 1)
        m = argv[1];
    io_service ioreciver;
    serial_port sp(ioreciver,m);
    sp.set_option(serial_port::baud_rate(115200));
    sp.set_option(serial_port::flow_control(serial_port::flow_control::none));
    sp.set_option(serial_port::parity(serial_port::parity::none));
    sp.set_option(serial_port::stop_bits(serial_port::stop_bits::one));
    sp.set_option(serial_port::character_size(8));


    int x(0),y(0),flag(0);
    std::string video = "1.mp4";
    if(argc != 1)
        video = argv[1];
    //std::cout << "Hello, World!" << std::endl;
    cv::VideoCapture capture;
    capture.open(video);
    if(!capture.isOpened())
        std::cout<<"No such a video!"<<std::endl;
    cv::Mat frame;
    //cv::namedWindow("test",cv::WINDOW_AUTOSIZE);
    while(1)
    {
        capture >> frame;
        if(frame.empty())
        {
            std::cout<<"end"<<std::endl;
            break;
        }
        thread tt1([&frame](){
            mutex mt;
            mt.lock();
            find(frame,point1,point2);
            mt.unlock();
        });
        tt1.join();
        if(flag != 0) {
            thread tt2([&sp]() {
                mutex mt;
                mt.lock();
                point1_pre.sanddata(sp);
                point2_pre.sanddata(sp);
                mt.unlock();
            });
            tt2.join();
        }
        flag ++;
        point1_pre = point1;
        point2_pre = point2;
        if(cv::waitKey(25) == 27) break;
        std::cout<<"end"<<std::endl;
    }
    return 0;
}


void find(cv::Mat& frame,data& point1,data& point2) {
    cv::Mat mid, frame_gray, mid_gray;
    cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
    cv::morphologyEx(frame_gray, mid, cv::MORPH_CLOSE, NULL, cv::Point(-1, -1), 1);
    cv::morphologyEx(mid, mid, cv::MORPH_OPEN, NULL, cv::Point(-1, -1), 1);
    cv::Canny(mid, mid_gray, 256, 256);
    cv::Mat rect_gray(mid_gray.rows, mid_gray.cols, CV_8U, cv::Scalar(0));
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mid_gray, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_TC89_KCOS);
    cv::Rect rect[contours.size()];
    for (int i = 0; i < contours.size(); i++) {
        rect[i] = cv::boundingRect(contours[i]);
        cv::rectangle(rect_gray,rect[i],cv::Scalar(255),1,cv::LINE_AA);
        cv::rectangle(frame,rect[i],cv::Scalar(255),1,cv::LINE_AA);
    }
    std::vector<std::vector<cv::Point>> contours1;
    cv::findContours(rect_gray, contours1, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_TC89_KCOS);
    cv::Mat out(mid_gray.rows, mid_gray.cols, CV_8UC3, cv::Scalar(0, 0, 0));
    cv::drawContours(frame, contours1, -1, cv::Scalar(255, 0, 255), 2, cv::LINE_AA);
    std::vector<cv::Moments> dian(contours1.size());
    for (int i = 0; i < contours1.size(); i++)
        dian[i] = cv::moments(contours1[i], 1);
    std::vector<cv::Point2f> mc(contours1.size());
    for (int i = 0; i < contours1.size(); i++)
        mc[i] = cv::Point2f(dian[i].m10 / dian[i].m00, dian[i].m01 / dian[i].m00);
    std::cout<<contours1.size()<<std::endl;
    if(mc.size() == 2) {
        point1.resetdata(1, mc[0].x, mc[0].y);
        point2.resetdata(2, mc[1].x, mc[1].y);
        cv::circle(frame,mc[0],3,cv::Scalar(255,0,255),1,cv::LINE_AA);
        cv::circle(frame,mc[1],3,cv::Scalar(255,0,255),1,cv::LINE_AA);
    }
    cv::imshow("test",frame);
}