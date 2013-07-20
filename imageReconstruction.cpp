#include "C:\OpenCV2.3\include\opencv2\opencv.hpp"
#include "getPSNR.h"
#include "fractalCode.h"
#include "imageReconstruction.h"
#include "getReflection.h"

void imageReconstruction(FractalCode* obj,cv::Mat& src,const int ITERATION,cv::Mat& dst,const int ROWS,const int COLS,const int RANGEBLOCKSIZE,const int DOMAINBLOCKSIZE)
{
	
	const int OBJECTREFERENCE=ROWS/RANGEBLOCKSIZE;

	int ch=src.channels();

	cv::Mat ref;
	src.copyTo(ref);

	char beforename[64];
	sprintf(beforename,"%02d:%.2fdB",0,getPSNR(src,dst));
	cv::putText(src,beforename,cv::Point(0,20), cv::FONT_HERSHEY_DUPLEX,0.3,CV_RGB(255,255,255),1);
	sprintf(beforename,"./output/Reconstruction/%02d.png",0);
	imwrite(beforename,src);

	std::cout<<"Iteration:"<<0<<"PSNR="<<getPSNR(src,dst)<<std::endl;

	for(int i=0;i<ITERATION;i++)
	{
		//std::cout<<"iteration="<<i+1<<std::endl;

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

				//////////adjust contrast//////////

				cv::Mat dVector;
				dVector=smalldBlock.reshape(3,RANGEBLOCKSIZE*RANGEBLOCKSIZE);

				std::vector<cv::Mat> singleDVector;
				cv::split(dVector,singleDVector);

				int iAve[3];

				for(int i=0;i<ch;i++)
				{
					cv::Mat cvAve;
					cv::reduce(singleDVector[i], cvAve, 0, CV_REDUCE_AVG); 
					iAve[i]=cvAve.at<uchar>(0.0);
				}

				cv::Mat ContrastVector(RANGEBLOCKSIZE*RANGEBLOCKSIZE,1,CV_8UC3);

				std::vector<cv::Mat> singleContrastVector;

				cv::split(ContrastVector,singleContrastVector);

				for(int y=0;y<RANGEBLOCKSIZE*RANGEBLOCKSIZE;y++)
				{
					for(int i=0;i<ch;i++)
					{
						uchar* s=singleDVector[i].ptr<uchar>(y);
						uchar* d=singleContrastVector[i].ptr<uchar>(y);

						d[i]=iAve[i]-((iAve[i]-(int)s[i])*obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getContrast()[i]);
					}
				}

				//////////adjust offset//////////

				cv::Mat OffsetVector(RANGEBLOCKSIZE*RANGEBLOCKSIZE,1,CV_8UC3);

				//split channels
				std::vector<cv::Mat> singleOffsetVector;

				cv::split(OffsetVector,singleOffsetVector);

				for(int y=0;y<RANGEBLOCKSIZE*RANGEBLOCKSIZE;y++)
				{
					for(int i=0;i<ch;i++)
					{
						uchar* s=singleContrastVector[i].ptr<uchar>(y);
						uchar* d=singleOffsetVector[i].ptr<uchar>(y);

						d[i]=s[i]+obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getOffset()[i];
					}
				}

				cv::merge(singleOffsetVector,OffsetVector);

				cv::Mat Offset;
				Offset=OffsetVector.reshape(3,RANGEBLOCKSIZE);

				//////////adjust rotation of reflection//////////

				enum MODE{ROTATION=0,REFLECTION};
				MODE mode;
				cv::Mat replace;

				if(obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getMode()==ROTATION)
				{

					float scale= 1.0f;
					cv::Point2i center(RANGEBLOCKSIZE*0.5,RANGEBLOCKSIZE*0.5);

					cv::Mat affine_matrix=cv::getRotationMatrix2D(center,obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getAngle(),scale);
					cv::warpAffine(Offset,replace,affine_matrix,Offset.size());

				}else if(obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getMode()==REFLECTION){

					replace=getReflection(Offset,obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getAngle(),RANGEBLOCKSIZE);

				}else{
					std::cout<<"haven't got that mode"<<std::endl;
				}

				//////////replace range by domain//////////

				for(int y=0;y<RANGEBLOCKSIZE;y++)
				{
					uchar* s=replace.ptr<uchar>(y);
					uchar* d=src.ptr<uchar>(y+rPoint.y);

					for(int x=0;x<RANGEBLOCKSIZE;x++)
					{
						d[3*(rPoint.x+x)]=s[3*x];
						d[3*(rPoint.x+x)+1]=s[3*x+1];
						d[3*(rPoint.x+x)+1]=s[3*x+2];
					}
				}

			}//range loop end
		}//range loop end


		ref=src;//update reference image

		char name[64];
		sprintf(name,"%02d:%.2fdB",i+1,getPSNR(src,dst));
		cv::putText(src,name,cv::Point(0,20),cv::FONT_HERSHEY_DUPLEX,0.3,CV_RGB(255,255,255),1);
		sprintf(name,"./output/Reconstruction/%02d.png",i+1);
		imwrite(name,src);

		std::cout<<"Iteration:"<<i+1<<"PSNR="<<getPSNR(src,dst)<<std::endl;

	}//iteration end

	std::cout<<"ƒ_ƒƒ|"<<std::endl;

}




