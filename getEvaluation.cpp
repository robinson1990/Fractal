#include "C:\OpenCV2.3\include\opencv2\opencv.hpp"
#include "getEvaluation.h"

double getEvaluation(cv::Mat& src1,cv::Mat& src2,int offset[3])
{
	double evaluation=0;
	int count=0;
	int goffset=(offset[0]+offset[1]+offset[2])/3;

	cv::Mat gsrc1,gsrc2;
	cvtColor(src1,gsrc1,CV_BGR2GRAY);
	cvtColor(src2,gsrc2,CV_BGR2GRAY);

	for(int j=0;j<gsrc1.rows;j++)
	{
		uchar* s1=gsrc1.ptr(j);
		uchar* s2=gsrc2.ptr(j);

		for(int i=0;i<gsrc1.cols;i++)
		{
			evaluation+=((s2[i]-s1[i]+goffset)*(s2[i]-s1[i]+goffset));
			count++;
		}
	}

	evaluation/=(double)count;

	return evaluation;


}