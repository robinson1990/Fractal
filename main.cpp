#include "main.h"
#include "FractalCode.h"
#include "getFractalCodeElements.h"
#include "ImageReconstruction.h"
#include <math.h>
#include "C:\OpenCV2.3\include\opencv2\opencv.hpp"
#include "getPSNR.h"

int main(int argc,char** argv)
{
	//artificial image
	cv::Mat src=cv::imread("./input/src.png");
	cv::Mat dst=cv::imread("./input/dst.png");

	//natural image
	//cv::Mat input=cv::imread("./input/tokamiyama.png");
	//cv::Mat dst;
	//cv::resize(input,dst,cv::Size(100,100));
	//cv::Mat src;
	//cv::GaussianBlur(dst,src,cv::Size(3,3),2,2);

	const int ITERATIONNUM=2;  
	const int ROWS=dst.rows;
	const int COLS=dst.cols;
	const int RANGEBLOCKSIZE=10;
	const int DOMAINBLOCKSIZE=2*RANGEBLOCKSIZE;

	//const int ARRAYNUM=static_cast<int>(pow((ROWS/RANGEBLOCKSIZE),2.0));
	//std::cout<<ARRAYNUM<<std::endl;

	FractalCode obj[100];

	getFractalCodeElements(obj,dst,ROWS,COLS,RANGEBLOCKSIZE,DOMAINBLOCKSIZE);

	imageReconstruction(obj,src,ITERATIONNUM,dst,ROWS,COLS,RANGEBLOCKSIZE,DOMAINBLOCKSIZE);

	return 0;

}