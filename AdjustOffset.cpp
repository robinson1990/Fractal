#include "AdjustOffset.h"
#include "C:\OpenCV2.3\include\opencv2\opencv.hpp"

void AdjustOffset(cv::Mat& src,cv::Mat& dst,int offset[3])
{
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

				d[3*x+i]=s[3*x+i]-offset[i];

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