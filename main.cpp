#include "main.h"
#include "FractalCode.h"
#include "getFractalCode.h"
#include "ImageReconstruction.h"
#include <math.h>
#include "C:\OpenCV2.3\include\opencv2\opencv.hpp"
#include "getPSNR.h"

int main(int argc,char** argv)
{
	//artificial image
	//cv::Mat src=cv::imread("./input/src.png");
	//cv::Mat dst=cv::imread("./input/dst.png");

	//natural image
	cv::Mat dst=cv::imread("./input/128lenna.png");
	//cv::Mat resize;
	//cv::resize(dst,resize,cv::Size(128,128));
	//imwrite("128lenna.png",resize);

	cv::Mat src= cv::Mat::zeros(dst.rows,dst.cols,CV_8UC3);
	//cv::GaussianBlur(dst,src,cv::Size(3,3),2,2);

	const int ITERATIONNUM=10;  
	const int ROWS=dst.rows;
	const int COLS=dst.cols;
	const int RANGEBLOCKSIZE=4;
	const int DOMAINBLOCKSIZE=2*RANGEBLOCKSIZE;

	FractalCode obj[1024];

	double t=(double)cv::getTickCount();

	getFractalCode(obj,dst,RANGEBLOCKSIZE,DOMAINBLOCKSIZE);

	std::cout<<"processing time="<<((double)cv::getTickCount()-t)/cv::getTickFrequency()/60<<"min"<<std::endl;

	ImageReconstruction(obj,src,dst,RANGEBLOCKSIZE,DOMAINBLOCKSIZE,ITERATIONNUM);

	return 0;

}