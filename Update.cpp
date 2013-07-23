#include "Update.h"
#include "C:\OpenCV2.3\include\opencv2\opencv.hpp"
#include "FractalCode.h"

void Update(FractalCode* obj,cv::Mat& src,int rx,int ry,int dx,int dy,int mode,int angle,float contrast[3],int offset[3],const int RANGEBLOCKSIZE)
{

	const int OBJECTREFERENCE=src.rows/RANGEBLOCKSIZE;

	//std::cout<<"/////Update/////"<<std::endl;
	//std::cout<<"obj["<<ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE<<"]"<<std::endl;

	cv::Point2i rPoint,dPoint;
	rPoint.x=rx;
	rPoint.y=ry;
	dPoint.x=dx;
	dPoint.y=dy;

	obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].setrCoordinate(rPoint);
	obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].setdCoordinate(dPoint);
	obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].setMode(mode);
	obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].setAngle(angle);
	obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].setContrast(contrast);
	obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].setOffset(offset);

	//std::cout<<"range coordinate="<<obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getrCoordinate()<<std::endl;
	//std::cout<<"domain coordinate="<<obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getdCoordinate()<<std::endl;
	//std::cout<<"mode="<<obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getMode()<<" (0 means rotation,1 means reflection)"<<std::endl;
	//std::cout<<"angle="<<obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getAngle()<<std::endl;
	//std::cout<<"contrast GBR=("
	//	<<(int)obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getContrast()[0]<<","
	//	<<obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getContrast()[1]<<","
	//	<<obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getContrast()[2]<<")"<<std::endl;

	//std::cout<<"offset GBR=("
	//	<<(int)obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getOffset()[0]<<","
	//	<<obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getOffset()[1]<<","
	//	<<obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getOffset()[2]<<")"<<std::endl;

}