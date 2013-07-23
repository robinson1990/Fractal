#include "Trim.h"
#include "C:\OpenCV2.3\include\opencv2\opencv.hpp"

void Trim(cv::Mat& src,cv::Mat& dst,int X,int Y,const int SIZE)
{

	for(int j=0;j<SIZE;j++)
	{
		uchar* s=src.ptr<uchar>(Y+j);
		uchar* d=dst.ptr<uchar>(j);

		for(int i=0;i<SIZE;i++)
		{
			d[3*i]=s[3*(X+i)];
			d[3*i+1]=s[3*(X+i)+1];
			d[3*i+2]=s[3*(X+i)+2];
		}
	}
}
