#include "getFractalCode.h"
#include "FractalCode.h"
#include "getPSNR.h"
#include "C:\OpenCV2.3\include\opencv2\opencv.hpp"
#include "Reflection.h"
#include "get_Min_Max.h"
#include "getOffset.h"
#include "AdjustContrast.h"
#include "Trim.h"
#include "AdjustContrast.h"
#include "getEvaluation.h"
#include "Update.h"

void getFractalCode(FractalCode* obj,cv::Mat& src,const int RANGEBLOCKSIZE,const int DOMAINBLOCKSIZE)
{
	const int OBJECTREFERENCE=src.rows/RANGEBLOCKSIZE;

	std::cout<<"getFractalCodeElements function Start"<<std::endl;

	const int ch=src.channels();

	//Range Block Loop Start
	for(int ry=0;ry<src.rows-RANGEBLOCKSIZE;ry+=RANGEBLOCKSIZE)
	{
		for(int rx=0;rx<src.cols-RANGEBLOCKSIZE;rx+=RANGEBLOCKSIZE)
		{
			
			std::cout<<"obj["<<ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE<<"]"<<std::endl;

			cv::Mat rBlock(RANGEBLOCKSIZE,RANGEBLOCKSIZE,CV_8UC3);

			Trim(src,rBlock,rx,ry,RANGEBLOCKSIZE);

			int rmin[3],rmax[3];//BGR
			get_Min_Max(rBlock,rmin,rmax);

			double evaluation=INT_MAX;
			enum MODE{ROTATION=0,REFLECTION};
			MODE mode;

			bool searchFlag=true;

			//Domain Block Loop start

			for(int dy=0;dy<=src.rows-DOMAINBLOCKSIZE;dy+=RANGEBLOCKSIZE)
			{
				for(int dx=0;dx<=src.cols-DOMAINBLOCKSIZE;dx+=RANGEBLOCKSIZE)
				{

					//exception
				
					if((rmax[0]==rmin[0])||(rmax[1]==rmin[1])||(rmax[2]==rmin[2])){
						
						cv::Mat dBlock(DOMAINBLOCKSIZE,DOMAINBLOCKSIZE,CV_8UC3);
						Trim(src,dBlock,rx,ry,DOMAINBLOCKSIZE);
						cv::Mat smalldBlock;
						cv::resize(dBlock,smalldBlock,cv::Size(),(float)RANGEBLOCKSIZE/DOMAINBLOCKSIZE,(float)RANGEBLOCKSIZE/DOMAINBLOCKSIZE);

						searchFlag=false;
						float specialcontrast[3]={1.0f,1.0f,1.0f};
						int specialoffset[3];
						Trim(src,dBlock,dx,dy,DOMAINBLOCKSIZE);
						cv::Mat specialsmalldBlock;
						smalldBlock.convertTo(specialsmalldBlock,CV_8UC3);

						getOffset(specialsmalldBlock,rBlock,specialoffset);
						Update(obj,src,rx,ry,rx,ry,ROTATION,0,specialcontrast,specialoffset,RANGEBLOCKSIZE);
						break;
					}else{

						cv::Mat dBlock(DOMAINBLOCKSIZE,DOMAINBLOCKSIZE,CV_8UC3);

						Trim(src,dBlock,dx,dy,DOMAINBLOCKSIZE);

						cv::Mat smalldBlock;
						cv::resize(dBlock,smalldBlock,cv::Size(),(float)RANGEBLOCKSIZE/DOMAINBLOCKSIZE,(float)RANGEBLOCKSIZE/DOMAINBLOCKSIZE);

						//rotation

						for(int theta=0;theta<360;theta+=90)
						{
							cv::Mat rotation;
							cv::Mat cvContrast(RANGEBLOCKSIZE,RANGEBLOCKSIZE,CV_8UC3);
							int tempAngle=theta;
							float scale= 1.0f;
							double tempEvaluation;
							float tempcontrast[3];
							int tempoffset[3];
							cv::Point2i center(RANGEBLOCKSIZE*0.5,RANGEBLOCKSIZE*0.5);

							cv::Mat affine_matrix=cv::getRotationMatrix2D(center,tempAngle,scale);
							cv::warpAffine(smalldBlock,rotation,affine_matrix,smalldBlock.size());

							int dmax[3],dmin[3];
							get_Min_Max(rotation,dmin,dmax);

							for(int i=0;i<ch;i++){
								tempcontrast[i]=(rmax[i]-rmin[i])/(float)(dmax[i]-dmin[i]);
							}

							AdjustContrast(rotation,cvContrast,tempcontrast);

							getOffset(cvContrast,rBlock,tempoffset);

							tempEvaluation=getEvaluation(rBlock,cvContrast,tempoffset);
							

							if(tempEvaluation<evaluation)
							{
								Update(obj,src,rx,ry,dx,dy,ROTATION,theta,tempcontrast,tempoffset,RANGEBLOCKSIZE);
								//std::cout<<"tempEvaluation="<<tempEvaluation<<std::endl;
							}
						}//rotation end

						//refection

						for(int theta=0;theta<180;theta+=45)
						{
							cv::Mat reflection;
							cv::Mat cvContrast(RANGEBLOCKSIZE,RANGEBLOCKSIZE,CV_8UC3);
							int tempAngle=theta;
							double tempEvaluation;
							float contrast[3];
							int offset[3];

							reflection=Reflection(smalldBlock,theta,RANGEBLOCKSIZE);

							int dmax[3],dmin[3];
							get_Min_Max(reflection,dmin,dmax);

							for(int i=0;i<ch;i++){
								contrast[i]=(rmax[i]-rmin[i])/(float)(dmax[i]-dmin[i]);
							}

							AdjustContrast(reflection,cvContrast,contrast);

							getOffset(cvContrast,rBlock,offset);

							tempEvaluation=getEvaluation(rBlock,cvContrast,offset);

							if(tempEvaluation<evaluation)
							{
								Update(obj,src,rx,ry,dx,dy,REFLECTION,theta,contrast,offset,RANGEBLOCKSIZE);
							}
						}//reflection end

					}//else end

				}

				if(!searchFlag){
					break;
				}

			}//Domain Block Loop End

		}

	}//Range Block Loop End

	std::cout<<"getFractalCodeElements function End"<<std::endl;

}