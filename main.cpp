#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define bigdiff(x) ((x) < 50 &&(x) > -50)

int main(int argc,char** argv) {
    int x(0),y(0);
    std::string video = "1.mp4";
    if(argc != 1)
        video = argv[1];
    //std::cout << "Hello, World!" << std::endl;
    cv::VideoCapture capture;
    capture.open(video);
    if(!capture.isOpened())
        std::cout<<"No such a video!"<<std::endl;
    cv::Mat frame,mid_gray;
    cv::namedWindow("test",cv::WINDOW_AUTOSIZE);

    while(1)
    {
        capture >> frame;
        if(frame.empty())
        {
            std::cout<<"end"<<std::endl;
            break;
        }
        cv::GaussianBlur(frame,frame,cv::Size(13,21),0,0);
        cv::Canny(frame,mid_gray,256,256);
        cv::Mat gray(mid_gray.rows,mid_gray.cols,CV_8U,cv::Scalar(0));
        //cv::imshow("test",gray);
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(mid_gray,contours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_TC89_KCOS);
        for(int i=0;i<contours.size();i++)
        {
            cv::Rect rect;
            rect = cv::boundingRect(contours[i]);
            cv::rectangle(gray,rect,cv::Scalar(255),1,cv::LINE_AA);
        }
        //std::cout<<"end"<<std::endl;
        std::vector<std::vector<cv::Point>> contours1;
        cv::findContours(gray,contours1,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_TC89_KCOS);
        cv::Mat out(gray.rows,gray.cols,CV_8UC3,cv::Scalar(0,0,0));
        cv::drawContours(out,contours1,-1,cv::Scalar(255,0,255),2,cv::LINE_AA);
        std::vector<cv::Moments> dian(contours1.size());
        for(int i=0;i<contours1.size();i++)
            dian[i]=cv::moments(contours1[i],1);
        std::vector<cv::Point2f> mc(contours1.size());
        for(int i=0;i<contours1.size();i++)
            mc[i]=cv::Point2f(dian[i].m10/dian[i].m00,dian[i].m01/dian[i].m00);
        for(int i=0;i<mc.size();i++)
            cv::circle(out,mc[i],3,cv::Scalar(255,0,255),1,cv::LINE_AA);

        int mx(0),my(0);
        int l = contours1.size();
        int c(0);
        if(l % 2 == 0)
            for(int i=0;i<l;i++)
            {
                c++;
                mx += mc[i].x;
                my += mc[i].y;
            }
        else
        {
            for(int i=0;i<l;i+=2)
            {
                c++;
                mx += mc[i].x;
                my += mc[i].y;
            }
        }
        int mxc,myc;
        if(c != 0)
        {
            mxc = mx/c;
            myc = my/c;
        }
        else
        {
            mxc = x;
            myc = y;
        }
        //std::cout<<x-mxc<<" "<<bigdiff(x-mxc)<<" "<<y-myc<<" "<<bigdiff(y-myc)<<std::endl;
        if(x == 0 && y == 0)
        {
            x = mxc;
            y = myc;
        }
        else if((bigdiff(x-mxc) && bigdiff(y-myc))) {
            x = mxc;
            y = myc;
        }
        else
        {
            x = x;
            y = y;
        }

        /*std::vector<cv::Point> chatater;
        cv::goodFeaturesToTrack(gray,chatater,4,0.05,5);
        for(int i=0;i<chatater.size();i++)
            cv::circle(out,chatater[i],3.0,cv::Scalar(0,255,0),3,cv::LINE_AA);
        int mx(0),my(0);
        for(int i=0;i<chatater.size();i++)
        {
            mx+=chatater[i].x;
            my+=chatater[i].y;
        }
        mx/=chatater.size();
        my/=chatater.size();
        if(flag == 0)
        {
            x=mx;
            y=my;
            flag =1;
        }
        else
        {
            std::cout<<x-mx<<std::endl;
            std::cout<<y-my<<std::endl;
            std::cout<<std::endl;
            cv::waitKey();
            if(bigdiff(x,mx))
                x = x;
            else
                x = mx;
            if(bigdiff(y,my))
                y = y;
            else
                y = my;
        }*/
        cv::circle(frame,cv::Point(x,y),3.0,cv::Scalar(0,0,255),3,cv::LINE_AA);
        cv::imshow("test",frame);
        //cv::waitKey();
        if(cv::waitKey(33) == 27) break;
    }

    return 0;
}