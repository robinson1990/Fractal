#include "Replace.h"
#include "C:\OpenCV2.3\include\opencv2\opencv.hpp"

void Replace(cv::Mat& src,cv::Mat& dst,int X,int Y,const int SIZE)
{
	for(int y=0;y<SIZE;y++)
	{
		uchar* s=src.ptr<uchar>(y);
		uchar* d=dst.ptr<uchar>(y+Y);

		for(int x=0;x<SIZE;x++)
		{
			d[3*(X+x)]=s[3*x];
			d[3*(X+x)+1]=s[3*x+1];
			d[3*(X+x)+2]=s[3*x+2];
		}
	}
}
