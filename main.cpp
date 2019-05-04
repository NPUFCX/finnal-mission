#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int main(int argc,char** argv) {
    std::string video = "1.mp4";
    if(argc != 1)
        video = argv[1];
    std::cout << "Hello, World!" << std::endl;
    cv::VideoCapture capture;
    capture.open(video);
    if(!capture.isOpened())
        std::cout<<"No such a video!"<<std::endl;
    cv::Mat frame;
    cv::namedWindow("test",cv::WINDOW_AUTOSIZE);

    while(1)
    {
        capture >> frame;
        if(frame.empty())
        {
            std::cout<<"end"<<std::endl;
            break;
        }
        cv::imshow("test",frame);
        if(cv::waitKey(33) == 27) break;
    }

    return 0;
}