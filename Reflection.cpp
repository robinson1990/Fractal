#include "C:\OpenCV2.3\include\opencv2\opencv.hpp"
#include "Reflection.h"

cv::Mat Reflection(cv::Mat& src,int angle,const int RANGEBLOCKSIZE)
{

	cv::Mat dst(RANGEBLOCKSIZE,RANGEBLOCKSIZE,CV_8UC3);

	if(angle==0){
		for(int y=0;y<RANGEBLOCKSIZE;y++)
						{
							//std::cout<<"y="<<y<<std::endl;
							for(int x=0;x<RANGEBLOCKSIZE;x++)
							{
							//std::cout<<"x="<<x<<std::endl;
								for(int i=0;i<src.channels();i++)
								{
									dst.at<cv::Vec3b>(RANGEBLOCKSIZE-y-1,x)[i]=src.at<cv::Vec3b>(y,x)[i];
								}
							}
						}

	}else if(angle==45){
		for(int y=0;y<RANGEBLOCKSIZE;y++)
						{
							for(int x=0;x<RANGEBLOCKSIZE;x++)
							{	
								for(int i=0;i<src.channels();i++)
								{
									dst.at<cv::Vec3b>(y,x)[i]=src.at<cv::Vec3b>(RANGEBLOCKSIZE-x-1,RANGEBLOCKSIZE-y-1)[i];
								}
							}
						}
	
	
	}else if(angle==90){
		for(int y=0;y<RANGEBLOCKSIZE;y++)
						{
							for(int x=0;x<RANGEBLOCKSIZE;x++)
							{	
								for(int i=0;i<src.channels();i++)
								{
										dst.at<cv::Vec3b>(y,RANGEBLOCKSIZE-x-1)[i]=src.at<cv::Vec3b>(y,x)[i];
								}
							}
						}
	}else if(angle==135){
		for(int y=0;y<RANGEBLOCKSIZE;y++)
						{
							for(int x=0;x<RANGEBLOCKSIZE;x++)
							{	
								for(int i=0;i<src.channels();i++)
								{		
									dst.at<cv::Vec3b>(y,x)[i]=src.at<cv::Vec3b>(x,y)[i];
								}
							}
						}
	}else{
		std::cout<<"wrong angle"<<std::endl;
	}

		return dst;

}
