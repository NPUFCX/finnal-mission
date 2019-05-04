#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int main(int argc,char** argv) {
    std::string video = "2.avi";
    if(argc != 1)
        video = argv[1];
    std::cout << "Hello, World!" << std::endl;
    cv::VideoCapture capture;
    capture.open(video);
    if(!capture.isOpened())
        std::cout<<"No such a video!"<<std::endl;
    cv::Mat frame,gray;
    cv::namedWindow("test",cv::WINDOW_AUTOSIZE);

    while(1)
    {
        capture >> frame;
        if(frame.empty())
        {
            std::cout<<"end"<<std::endl;
            break;
        }
        cv::Canny(frame,gray,256,256);
        cv::Mat out(gray.rows,gray.cols,CV_8UC3,cv::Scalar(0,0,0));
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(gray,contours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_TC89_KCOS);
        cv::drawContours(out,contours,-1,cv::Scalar(255,0,255),2,cv::LINE_AA);
        std::vector<cv::Moments> dian(contours.size());
        for(int i=0;i<contours.size();i++)
            dian[i]=cv::moments(contours[i],1);
        std::vector<cv::Point2f> mc(contours.size());
        for(int i=0;i<contours.size();i++)
            mc[i]=cv::Point2f(dian[i].m10/dian[i].m00,dian[i].m01/dian[i].m00);
        for(int i=0;i<mc.size();i++)
            cv::circle(out,mc[i],3,cv::Scalar(255,0,255),1,cv::LINE_AA);
        cv::imshow("test",out);
        if(cv::waitKey(33) == 27) break;
    }

    return 0;
}