#include "FractalCode.h"
#include "C:\OpenCV2.3\include\opencv2\opencv.hpp"

//set
void FractalCode::setdCoordinate(cv::Point2i Point)
{
	dCoordinate.x=Point.x;
	dCoordinate.y=Point.y;
}
void FractalCode::setrCoordinate(cv::Point2i Point)
{
	rCoordinate.x=Point.x;
	rCoordinate.y=Point.y;
}
void FractalCode::setContrast(float num[])
{
	contrast[0]=num[0];
	contrast[1]=num[1];
	contrast[2]=num[2];
}
void FractalCode::setOffset(int num[])
{
	offset[0]=num[0];
	offset[1]=num[1];
	offset[2]=num[2];
}
void FractalCode::setMode(int num)
{
	mode=num;
}
void FractalCode::setAngle(int num)
{
	angle=num;
}

//get
cv::Point2i FractalCode::getdCoordinate()
{
return dCoordinate;
}
cv::Point2i FractalCode::getrCoordinate()
{
return rCoordinate;
}
float* FractalCode::getContrast()
{
	return contrast;
}
int* FractalCode::getOffset()
{
	return offset;
}
int FractalCode::getMode()
{
	return mode;
}
int FractalCode::getAngle()
{
	return angle;
}

// constructor
FractalCode::FractalCode()
{
	static int count=0;
	std::cout << "Constructor Called" << std::endl;
	std::cout<<count<<std::endl;
	count++;
}

//copy constructor
FractalCode::FractalCode(const FractalCode& obj)
{
	std::cout << "Copy Constructor Called" << std::endl;
}

// destructor
FractalCode::~FractalCode()
{
	std::cout << "Destructor Called" << std::endl;
}