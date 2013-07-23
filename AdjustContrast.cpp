#include "AdjustContrast.h"
#include "C:\OpenCV2.3\include\opencv2\opencv.hpp"

void AdjustContrast(cv::Mat& src,cv::Mat& dst,float contrast[3])
{
	cv::Mat Vector;
	Vector=src.reshape(3,src.rows*src.cols);

	std::vector<cv::Mat> singleVector;
	cv::split(Vector,singleVector);

	int iAve[3];

	for(int i=0;i<src.channels();i++)
	{
		cv::Mat cvAve;

		cv::reduce(singleVector[i],cvAve,0,CV_REDUCE_AVG); 

		iAve[i]=cvAve.at<uchar>(0.0);
	}

	cv::Mat srcf;
	cv::Mat dstf;

	src.convertTo(srcf,CV_32FC3);
	dst.convertTo(dstf,CV_32FC3);

	for(int y=0;y<src.rows;y++)
	{
		float* s=srcf.ptr<float>(y);
		float* d=dstf.ptr<float>(y);

		for(int x=0;x<src.cols;x++)
		{
			for(int i=0;i<src.channels();i++){

				d[3*x+i]=iAve[i]-((iAve[i]-(int)s[3*x+i])*contrast[i]);
				if(d[3*x+i]<0)
				{
					d[3*x+i]=0;
				}else if(d[3*x+i]>255){
					d[3*x+i]=255;
				}else{
					;
				}
			}
		}
	}

	srcf.convertTo(src,CV_8UC3);
	dstf.convertTo(dst,CV_8UC3);

}