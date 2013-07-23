#include "C:\OpenCV2.3\include\opencv2\opencv.hpp"
#include "getPSNR.h"
#include "fractalCode.h"
#include "imageReconstruction.h"
#include "Reflection.h"
#include "Trim.h"
#include "Replace.h"
#include "AdjustContrast.h"
#include "AdjustOffset.h"

void ImageReconstruction(FractalCode* obj,cv::Mat& src,cv::Mat& dst,const int RANGEBLOCKSIZE,const int DOMAINBLOCKSIZE,const int ITERATION)
{
	std::cout<<"ImageReconstruction function Start"<<std::endl;

	const int OBJECTREFERENCE=src.rows/RANGEBLOCKSIZE;

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
		for(int ry=0;ry<src.rows-RANGEBLOCKSIZE;ry+=RANGEBLOCKSIZE)
		{
			for(int rx=0;rx<src.cols-RANGEBLOCKSIZE;rx+=RANGEBLOCKSIZE)
			{

				std::cout<<"range coordinate="<<obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getrCoordinate()<<std::endl;
				std::cout<<"domain coordinate="<<obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getdCoordinate()<<std::endl;
				std::cout<<"mode="<<obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getMode()<<" (0 means rotation,1 means reflection)"<<std::endl;
				std::cout<<"angle="<<obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getAngle()<<std::endl;
				std::cout<<"contrast GBR=("
					<<(int)obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getContrast()[0]<<","
					<<obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getContrast()[1]<<","
					<<obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getContrast()[2]<<")"<<std::endl;

				std::cout<<"offset GBR=("
					<<(int)obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getOffset()[0]<<","
					<<obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getOffset()[1]<<","
					<<obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getOffset()[2]<<")"<<std::endl;

				cv::Point2i rPoint=obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getrCoordinate();
				cv::Point2i dPoint=obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getdCoordinate();

				cv::Mat dBlock(DOMAINBLOCKSIZE,DOMAINBLOCKSIZE,CV_8UC3);

				Trim(ref,dBlock,dPoint.x,dPoint.y,DOMAINBLOCKSIZE);

				//size

				cv::Mat smalldBlock;
				cv::resize(dBlock,smalldBlock,cv::Size(),(float)RANGEBLOCKSIZE/DOMAINBLOCKSIZE,(float)RANGEBLOCKSIZE/DOMAINBLOCKSIZE);

				//rotation and reflection

				enum MODE{ROTATION=0,REFLECTION};
				cv::Mat replace(RANGEBLOCKSIZE,RANGEBLOCKSIZE,CV_8UC3);
				cv::Mat Affine;
				cv::Mat cvContrast(RANGEBLOCKSIZE,RANGEBLOCKSIZE,CV_8UC3);

				if(obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getMode()==ROTATION){
					float scale=1.0f;
					cv::Point2i center(RANGEBLOCKSIZE*0.5,RANGEBLOCKSIZE*0.5);
					cv::Mat affine_matrix=cv::getRotationMatrix2D(center,obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getAngle(),scale);
					cv::warpAffine(smalldBlock,Affine,affine_matrix,smalldBlock.size());
				}else if(obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getMode()==REFLECTION){

					Affine=Reflection(smalldBlock,obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getAngle(),RANGEBLOCKSIZE);

				}else{
					std::cout<<"wrong mode"<<std::endl;
				}

				//contrast

				AdjustContrast(Affine,cvContrast,obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getContrast());


				//offset
				
				AdjustOffset(cvContrast,replace,obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getOffset());

				//std::cout<<Affine<<std::endl;
				//imwrite("./output/check/Affine.png",Affine);

				//std::cout<<cvContrast<<std::endl;
				//imwrite("./output/check/cvContrast.png",Affine);

				//std::cout<<replace<<std::endl;
				//imwrite("./output/check/replace.png",replace);

				//replace

				Replace(replace,src,rPoint.x,rPoint.y,RANGEBLOCKSIZE);

				//imshow("ref",ref);
				//imshow("Affine",Affine);
				//imshow("cvContrast",cvContrast);
				//imshow("replace",replace);
				//imshow("srcf",srcf);

				//cv::waitKey();


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




