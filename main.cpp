#include "main.h"
#include "FractalCode.h"
#include "getFractalCodeElements.h"
#include "ImageReconstruction.h"
#include <math.h>
#include "C:\OpenCV2.3\include\opencv2\opencv.hpp"
#include "getPSNR.h"

int main(int argc,char** argv)
{

	cv::Mat idealImage=cv::imread("./input/tokamiyama.png");

	cv::Mat resizeImage;
	//cv::resize(idealImage,resizeImage,cv::Size(100,100));
	cv::resize(idealImage,resizeImage,cv::Size(50,50));

	cv::imwrite("./output/clear.png",resizeImage);

	cv::Mat blurImage;
	cv::GaussianBlur(resizeImage,blurImage,cv::Size(3,3),2,2);

	cv::imwrite("./output/blur.png",blurImage);

	const int ITERATIONNUM=50;  
	const int ROWS=resizeImage.rows;
	const int COLS=resizeImage.cols;
	const int RANGEBLOCKSIZE=10;
	const int DOMAINBLOCKSIZE=2*RANGEBLOCKSIZE;

	//const int ARRAYNUM=static_cast<int>(pow((ROWS/RANGEBLOCKSIZE),2.0));
	//std::cout<<ARRAYNUM<<std::endl;

	FractalCode obj[25];

	getFractalCodeElements(obj,resizeImage,ROWS,COLS,RANGEBLOCKSIZE,DOMAINBLOCKSIZE);

	imageReconstruction(obj,blurImage,ITERATIONNUM,idealImage,ROWS,COLS,RANGEBLOCKSIZE,DOMAINBLOCKSIZE);

	return 0;

}