#include "getOffset.h"
#include "C:\OpenCV2.3\include\opencv2\opencv.hpp"
void getOffset(cv::Mat& src1,cv::Mat& src2,int offset[3])
{
	int ch=src1.channels();

	cv::Mat Vector1,Vector2;
	Vector1=src1.reshape(3,src1.rows*src1.cols);
	Vector2=src2.reshape(3,src2.rows*src2.cols);
	std::vector<cv::Mat> singleVector1,singleVector2;
	cv::split(Vector1,singleVector1);
	cv::split(Vector2,singleVector2);

	int iAve1[3],iAve2[3];

	for(int i=0;i<ch;i++)
	{
		cv::Mat cvAve1,cvAve2;
		cv::reduce(singleVector1[i],cvAve1,0,CV_REDUCE_AVG); 
		cv::reduce(singleVector2[i],cvAve2,0,CV_REDUCE_AVG); 

		iAve1[i]=cvAve1.at<uchar>(0.0);
		iAve2[i]=cvAve2.at<uchar>(0.0);
		offset[i]=iAve1[i]-iAve2[i];
	}
}