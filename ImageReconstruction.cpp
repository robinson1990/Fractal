#include "C:\OpenCV2.3\include\opencv2\opencv.hpp"
#include "getPSNR.h"
#include "fractalCode.h"
#include "imageReconstruction.h"

void imageReconstruction(FractalCode* obj,cv::Mat& src,const int ITERATION,cv::Mat& dst,const int ROWS,const int COLS,const int RANGEBLOCKSIZE,const int DOMAINBLOCKSIZE)
{
	std::cout<<"ImageReconstruction function Start"<<std::endl;

	const int OBJECTREFERENCE=ROWS/RANGEBLOCKSIZE;

	int ch=src.channels();

	cv::Mat ref;
	src.copyTo(ref);

	cv::Mat b4;
	src.copyTo(b4);

	char beforename[64];
	sprintf(beforename,"./output/Reconstruction/%02d.png",0);
	imwrite(beforename,b4);

	std::cout<<"Iteration:"<<0<<",PSNR="<<getPSNR(src,dst)<<std::endl;

	for(int i=0;i<ITERATION;i++)
	{
		for(int ry=0;ry<ROWS;ry+=RANGEBLOCKSIZE)
		{
			for(int rx=0;rx<COLS;rx+=RANGEBLOCKSIZE)
			{
				cv::Point2i rPoint=obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getrCoordinate();
				cv::Point2i dPoint=obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getdCoordinate();

				cv::Mat dBlock(DOMAINBLOCKSIZE,DOMAINBLOCKSIZE,CV_8UC3);

				for(int y=0;y<DOMAINBLOCKSIZE;y++)
				{
					uchar* s=ref.ptr<uchar>(dPoint.y+y);
					uchar* d=dBlock.ptr<uchar>(y);

					for(int x=0;x<DOMAINBLOCKSIZE;x++)
					{
						d[3*x]=s[3*(dPoint.x+x)];
						d[3*x+1]=s[3*(dPoint.x+x)+1];
						d[3*x+2]=s[3*(dPoint.x+x)+2];
					}
				}

				//////////adjust size//////////

				cv::Mat smalldBlock;
				cv::resize(dBlock,smalldBlock,cv::Size(),(float)RANGEBLOCKSIZE/DOMAINBLOCKSIZE,(float)RANGEBLOCKSIZE/DOMAINBLOCKSIZE);

				//////////replace range by domain//////////

				for(int y=0;y<RANGEBLOCKSIZE;y++)
				{
					uchar* s=smalldBlock.ptr<uchar>(y);
					uchar* d=src.ptr<uchar>(y+rPoint.y);

					for(int x=0;x<RANGEBLOCKSIZE;x++)
					{
						d[3*(rPoint.x+x)]=s[3*x];
						d[3*(rPoint.x+x)+1]=s[3*x+1];
						d[3*(rPoint.x+x)+2]=s[3*x+2];
					}
				}

			}//range loop end
		}//range loop end


		ref=src;//update reference image

		char name[64];
		sprintf(name,"./output/Reconstruction/%02d.png",i+1);
		imwrite(name,src);

		std::cout<<"Iteration:"<<i+1<<",PSNR="<<getPSNR(src,dst)<<std::endl;

	}//iteration end

	std::cout<<"ImageReconstruction function End"<<std::endl;

}




