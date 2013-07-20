#include "getFractalCodeElements.h"
#include "FractalCode.h"
#include "getPSNR.h"
#include "C:\OpenCV2.3\include\opencv2\opencv.hpp"

void getFractalCodeElements(FractalCode* obj,cv::Mat& src,const int ROWS,const int COLS,const int RANGEBLOCKSIZE,const int DOMAINBLOCKSIZE)
{
	const int OBJECTREFERENCE=ROWS/RANGEBLOCKSIZE;

	std::cout<<"getFractalCodeElements function Start"<<std::endl;

	const int ch=src.channels();

	//Range Block Loop Start
	for(int ry=0;ry<ROWS;ry+=RANGEBLOCKSIZE)
	{
		for(int rx=0;rx<COLS;rx+=RANGEBLOCKSIZE)
		{
			double t = (double)cv::getTickCount();

			cv::Point2i rPoint;
			rPoint.x=rx;
			rPoint.y=ry;
			obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].setrCoordinate(rPoint);
			std::cout<<"obj["<<ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE<<"]"<<std::endl;
			std::cout<<"range coordinate="<<obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getrCoordinate()<<std::endl;

			cv::Mat rBlock(RANGEBLOCKSIZE,RANGEBLOCKSIZE,CV_8UC3);

			for(int y=0;y<RANGEBLOCKSIZE;y++)
			{
				uchar* s=src.ptr<uchar>(ry+y);
				uchar* d=rBlock.ptr<uchar>(y);

				for(int x=0;x<RANGEBLOCKSIZE;x++)
				{
					d[3*x]=s[3*(rx+x)];
					d[3*x+1]=s[3*(rx+x)+1];
					d[3*x+2]=s[3*(rx+x)+2];
				}
			}

			//std::ostringstream rBlockName;
			//rBlockName<<"./output/Block/rBlock("<<ry<<","<<rx<<")(obj["<<ry+rx/RANGEBLOCKSIZE<<"])"<<".png";
			//imwrite(rBlockName.str(),rBlock);

			enum MODE{ROTATION=0,REFLECTION};
			double psnr=0;
			int dX;
			int dY;

			//Domain Block Loop start

			for(int dy=0;dy<=ROWS-DOMAINBLOCKSIZE;dy+=RANGEBLOCKSIZE)
			{
				for(int dx=0;dx<=COLS-DOMAINBLOCKSIZE;dx+=RANGEBLOCKSIZE)
				{
					cv::Mat dBlock(DOMAINBLOCKSIZE,DOMAINBLOCKSIZE,CV_8UC3);

					for(int y=0;y<DOMAINBLOCKSIZE;y++)
					{
						uchar* s=src.ptr<uchar>(dy+y);
						uchar* d=dBlock.ptr<uchar>(y);

						for(int x=0;x<DOMAINBLOCKSIZE;x++)
						{
							d[3*x]=s[3*(dx+x)];
							d[3*x+1]=s[3*(dx+x)+1];
							d[3*x+2]=s[3*(dx+x)+2];
						}
					}

					cv::Mat smalldBlock;
					cv::resize(dBlock,smalldBlock,cv::Size(),(float)RANGEBLOCKSIZE/DOMAINBLOCKSIZE,(float)RANGEBLOCKSIZE/DOMAINBLOCKSIZE);

					double tempPSNR=getPSNR(rBlock,smalldBlock);

					if(tempPSNR>psnr)
					{
						psnr=tempPSNR;
						dX=dx;
						dY=dy;
					}
					else{
					;
					}
				}//Domain Block Loop End

			}//Domain Block Loop End

			//Set Fractal Code Elements
			cv::Point2i dPoint;
			dPoint.x=dX;
			dPoint.y=dY;
			obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].setdCoordinate(dPoint);

			std::cout<<"domain coordinate="<<obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getdCoordinate()<<std::endl;
		
			std::cout<<"PSNR="<<psnr<<std::endl;

			std::cout<<"processing time="<<((double)cv::getTickCount() - t)/cv::getTickFrequency()/60<<"min"<<std::endl;

		}//Range Block Loop End
	}//Range Block Loop End

	std::cout<<"getFractalCodeElements function End"<<std::endl;

}