#include "get_Min_Max.h"
#include "C:\OpenCV2.3\include\opencv2\opencv.hpp"
void get_Min_Max(cv::Mat& src,int iMin[3],int iMax[3])
{
	cv::Mat Vector;
	Vector=src.reshape(3,src.rows*src.cols);

	std::vector<cv::Mat> singleVector;
	cv::split(Vector,singleVector);

	for(int i=0;i<src.channels();i++)
	{
		cv::Mat cvMin,cvMax;

		cv::reduce(singleVector[i],cvMin,0,CV_REDUCE_MIN); 
		cv::reduce(singleVector[i],cvMax,0,CV_REDUCE_MAX); 

		iMin[i]=cvMin.at<uchar>(0.0);
		iMax[i]=cvMax.at<uchar>(0.0);
	}
}