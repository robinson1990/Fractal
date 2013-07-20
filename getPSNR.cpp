#include "C:\OpenCV2.3\include\opencv2\opencv.hpp"

double getPSNR(cv::Mat& src1,cv::Mat& src2)
{
	double sse=0;
	double mse,psnr;
	double MAX=255;

	cv::Mat gsrc1,gsrc2;
	cvtColor(src1,gsrc1,CV_BGR2GRAY);
	cvtColor(src2,gsrc2,CV_BGR2GRAY);

	int count=0;

	for(int j=2;j<gsrc1.rows-2;j++)
	{
		uchar* s1=gsrc1.ptr(j);
		uchar* s2=gsrc2.ptr(j);

		for(int i=2;i<gsrc1.cols-2;i++)
		{
			sse+=((s2[i]-s1[i])*(s2[i]-s1[i]));
			count++;
		}
	}

	mse=sse/(double)count;
	if(mse==0){
	psnr=50;
	}else{
	psnr=10.0*log10((MAX*MAX)/mse);
	}
	return psnr;

}