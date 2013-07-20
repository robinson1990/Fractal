#ifndef INCLUDED_FRACTALCODE_H
#define INCLUDED_FRACTALCODE_H

#include "C:\OpenCV2.3\include\opencv2\opencv.hpp"

class FractalCode
{
public:
	FractalCode();
	FractalCode(const FractalCode& obj);
	~FractalCode();

	void setdCoordinate(cv::Point2i);
	void setrCoordinate(cv::Point2i);
	void setContrast(float num[]);
	void setOffset(int num[]);
	void setMode(int num);
	void setAngle(int num);

	cv::Point2i getdCoordinate();
	cv::Point2i getrCoordinate();
	float* getContrast();
	int* getOffset();
	int getMode();
	int getAngle();

private:
	cv::Point2i dCoordinate;
	cv::Point2i rCoordinate;
	float contrast[3];
	int offset[3];
	int mode;
	int angle;
};

#endif