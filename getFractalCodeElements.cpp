#include "getFractalCodeElements.h"
#include "FractalCode.h"
#include "getPSNR.h"
#include "getReflection.h"
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

			std::ostringstream rBlockName;
			rBlockName<<"./output/Block/rBlock("<<ry<<","<<rx<<")(obj["<<ry+rx/RANGEBLOCKSIZE<<"])"<<".png";
			imwrite(rBlockName.str(),rBlock);

			//cv::imwrite("./output/obj");

			cv::Mat rVector;
			rVector=rBlock.reshape(3,RANGEBLOCKSIZE*RANGEBLOCKSIZE);

			//split channels
			std::vector<cv::Mat> singleRVector;
			cv::split(rVector,singleRVector);

			int iRangeAve[3];
			int iRangeMin[3];
			int iRangeMax[3];

			for(int i=0;i<ch;i++)
			{
				cv::Mat cvRangeAve,cvRangeMin,cvRangeMax;

				cv::reduce(singleRVector[i], cvRangeAve, 0, CV_REDUCE_AVG); 
				cv::reduce(singleRVector[i], cvRangeMin, 0, CV_REDUCE_MIN); 
				cv::reduce(singleRVector[i], cvRangeMax, 0, CV_REDUCE_MAX); 

				iRangeAve[i]=cvRangeAve.at<uchar>(0.0);
				iRangeMin[i]=cvRangeMin.at<uchar>(0.0);
				iRangeMax[i]=cvRangeMax.at<uchar>(0.0);

				//std::cout<<"ch="<<i<<std::endl;//B,G,R
				//std::cout<<"Ave,Min,Max of rBlock="<<iRangeAve[i]<<","<<iRangeMin[i]<<","<<iRangeMax[i]<<std::endl;
			}

			//std::cout<<rangeBlockVector.channels()<<std::endl;
			//std::cout<<rangeBlockVector.rows<<std::endl;
			//std::cout<<rangeBlockVector.cols<<std::endl;
			

			enum MODE{ROTATION=0,REFLECTION};
			MODE mode;
			double psnr=0;

			//double psnr;
			//if(rx==0&&ry==0)
			//{
			//psnr=0;
			//}
			//else{
			//;
			//}
			int angle;
			int dX;
			int dY;
			float contrast[3];
			int offset[3];

			//Domain Block Loop start

			for(int dy=0;dy<ROWS-DOMAINBLOCKSIZE;dy++)
			{
				for(int dx=0;dx<COLS-DOMAINBLOCKSIZE;dx++)
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

					//std::ostringstream dBlockName;

					//dBlockName<<"./output/Block/dBlock("<<dy<<","<<dx<<"](obj["<<ry+rx/RANGEBLOCKSIZE<<"])"<<".png";
					//imwrite(dBlockName.str(),dBlock);

					//cv::imwrite("./output/dBlock.png",dBlock);

					cv::Mat smalldBlock;
					cv::resize(dBlock,smalldBlock,cv::Size(),(float)RANGEBLOCKSIZE/DOMAINBLOCKSIZE,(float)RANGEBLOCKSIZE/DOMAINBLOCKSIZE);

					cv::Mat dVector;
					dVector=smalldBlock.reshape(3,RANGEBLOCKSIZE*RANGEBLOCKSIZE);

					//check smalldBlockvector
					//std::cout<<smalldBlockVector.channels()<<std::endl;
					//std::cout<<smalldBlockVector.rows<<std::endl;
					//std::cout<<smalldBlockVector.cols<<std::endl;

					//split channels
					std::vector<cv::Mat> singleDVector(3);

					cv::split(dVector,singleDVector);

					//std::cout<<(int)dVector.data[0]<<std::endl;
					//std::cout<<(int)dVector.at<cv::Vec3b>(0,0)[0]<<std::endl;
					//std::cout<<(int)singleDVector[0].at<uchar>(0,0)<<std::endl;

					//check
					//std::cout<<singlesmalldomainBlockVector[0].channels()<<std::endl;
					//std::cout<<singlesmalldomainBlockVector[0].rows<<std::endl;
					//std::cout<<singlesmalldomainBlockVector[0].cols<<std::endl;

					//get MAX,AVE,MIN

					int iDomainAve[3];

					int iDomainMin[3];

					int iDomainMax[3];

					for(int i=0;i<ch;i++)
					{
						cv::Mat cvDomainAve,cvDomainMin,cvDomainMax;

						cv::reduce(singleDVector[i], cvDomainAve, 0, CV_REDUCE_AVG); 
						cv::reduce(singleDVector[i], cvDomainMin, 0, CV_REDUCE_MIN); 
						cv::reduce(singleDVector[i], cvDomainMax, 0, CV_REDUCE_MAX); 

						//std::cout<<cvRangeAve<<std::endl;
						//std::cout<<cvRangeMin<<std::endl;
						//std::cout<<cvRangeMax<<std::endl;

						//std::cout<<cvDomainAve<<std::endl;
						//std::cout<<cvDomainMin<<std::endl;
						//std::cout<<cvDomainMax<<std::endl;

						iDomainAve[i]=cvDomainAve.at<uchar>(0.0);
						iDomainMin[i]=cvDomainMin.at<uchar>(0.0);
						iDomainMax[i]=cvDomainMax.at<uchar>(0.0);

						//std::cout<<"ch="<<i<<","<<"dCoordinate=("<<dx<<","<<dy<<")"<<std::endl;//B,G,R
						//std::cout<<"Ave,Min,Max of dBlock="<<iDomainAve[i]<<","<<iDomainMin[i]<<","<<iDomainMax[i]<<std::endl;
					}

					//////////get contrast//////////

					float tempcontrast[3];
					int tempoffset[3];

					for(int i=0;i<ch;i++)
					{
						if(iDomainMax[i]==iDomainMin[i])
						{
							//std::cout<<"DomainMax==DomainMin"<<std::endl;
							tempcontrast[i]=1;
						}
						else{
							tempcontrast[i]=(iRangeMax[i]-iRangeMin[i])/(float)(iDomainMax[i]-iDomainMin[i]);
						}
					}

					//std::cout<<"dx,dy"<<dx<<","<<dy<<std::endl;
					//std::cout<<tempcontrast[0]<<std::endl;
					//std::cout<<tempcontrast[1]<<std::endl;
					//std::cout<<tempcontrast[2]<<std::endl;

					//////////adjust contrast//////////

					cv::Mat ContrastVector(RANGEBLOCKSIZE*RANGEBLOCKSIZE,1,CV_8UC3);

					//split channels
					std::vector<cv::Mat> singleContrastVector;

					cv::split(ContrastVector,singleContrastVector); //bug reason??

					for(int y=0;y<RANGEBLOCKSIZE*RANGEBLOCKSIZE;y++)
					{
						for(int i=0;i<ch;i++)
						{
							uchar* s=singleDVector[i].ptr<uchar>(y);
							uchar* d=singleContrastVector[i].ptr<uchar>(y);

							//std::cout<<iDomainAve[i]<<std::endl;
							//std::cout<<(int)singleDVector[i].at<uchar>(0,0)<<std::endl;
							//std::cout<<(int)d[i]<<std::endl;
							//std::cout<<(int)s[i]<<std::endl;
							//std::cout<<contrastFactor[i]<<std::endl;
							//std::cout<<(iDomainAve[i]-(int)s[i])<<std::endl;
							d[i]=iDomainAve[i]-((iDomainAve[i]-(int)s[i])*tempcontrast[i]);
							//std::cout<<(int)d[i]<<std::endl;
						}
					}

					//std::cout<<iDomainAve[0]<<std::endl;
					//std::cout<<singleDVector[0]<<std::endl;
					//std::cout<<singleContrastVector[0]<<std::endl;

					///////////get offset//////////

					for(int i=0;i<ch;i++)
					{
						tempoffset[i]=iRangeAve[i]-iDomainAve[i];
					}

					//std::cout<<offset[0]<<std::endl;

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

							d[i]=s[i]+tempoffset[i];
						}
					}

					cv::merge(singleOffsetVector,OffsetVector);

					cv::Mat Offset;
					Offset=OffsetVector.reshape(3,RANGEBLOCKSIZE);

					//std::cout<<Offset.channels()<<std::endl;
					//std::cout<<Offset.rows<<std::endl;
					//std::cout<<Offset.cols<<std::endl;
					//std::cout<<Offset<<std::endl;

					//delete[] iRangeAve;
					//delete[] iRangeMin;
					//delete[] iRangeMax;
					//delete[] iDomainAve;
					//delete[] iDomainMin;
					//delete[] iDomainMax;

					///////////get rotation and reflection//////////

					for(int theta=0;theta<360;theta+=90)
					{

						//static int count1=0;
						//count1++;
						//std::cout<<"count1="<<count1<<std::endl;

						cv::Mat rotation;
						int tempAngle=theta;
						float scale= 1.0f;
						cv::Point2i center(RANGEBLOCKSIZE*0.5,RANGEBLOCKSIZE*0.5);

						cv::Mat affine_matrix=cv::getRotationMatrix2D(center,tempAngle,scale);
						cv::warpAffine(Offset,rotation,affine_matrix,Offset.size());

						imwrite("./output/Offset.png",Offset);
						imwrite("./output/rotation.png",rotation);

						double tempPSNR=getPSNR(rBlock,rotation);
						if(tempPSNR>psnr)
						{
							//std::cout<<"best angle is "<<temptempAngle<<std::endl;
							mode=ROTATION;
							angle=tempAngle;
							psnr=tempPSNR;
							for(int i=0;i<ch;i++)
							{
								contrast[i]=tempcontrast[i];
								offset[i]=tempoffset[i];
							}
							dX=dx;
							dY=dy;
						}else{
							;
						}
					}

					for(int theta=0;theta<180;theta+=45)
					{
						//static int count=0;
						//count++;
						//std::cout<<"count="<<count<<std::endl;

						//std::cout<<"theta="<<theta<<std::endl;
						//cv::Mat reflection(RANGEBLOCKSIZE,RANGEBLOCKSIZE//range loop end,CV_8UC3);
						cv::Mat reflection;

						reflection=getReflection(Offset,theta,RANGEBLOCKSIZE);

						int tempAngle=theta;
						double tempPSNR=getPSNR(rBlock,reflection);

						if(tempPSNR>psnr)
						{
							//std::cout<<"best reflection angle is "<<temptempAngle<<std::endl;
							mode=REFLECTION;
							angle=tempAngle;
							psnr=tempPSNR;
							for(int i=0;i<ch;i++) //break point put
							{
								contrast[i]=tempcontrast[i];
								offset[i]=tempoffset[i];
							}
							dX=dx;
							dY=dy;
						}else{
							;
						}
					}

				}//Domain Block Loop End

			}//Domain Block Loop End

			//Set Fractal Code Elements
			cv::Point2i dPoint;
			dPoint.x=dX;
			dPoint.y=dY;
			obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].setdCoordinate(dPoint);

			obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].setContrast(contrast);
			//std::cout<<contrast[0]<<std::endl;
			//std::cout<<contrast[1]<<std::endl;
			//std::cout<<contrast[2]<<std::endl;

			//delete[] contrast;

			obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].setOffset(offset);
			//delete[] offset;

			obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].setMode(mode);
			obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].setAngle(angle);

			//std::cout<<"arrayX="<<rx/RANGEBLOCKSIZE<<std::endl;
			//std::cout<<"arrayY="<<ry/RANGEBLOCKSIZE<<std::endl;

			std::cout<<"domain coordinate="<<obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getdCoordinate()<<std::endl;
			std::cout<<"contrast GBR=("
				<<(int)obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getContrast()[0]<<","
				<<obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getContrast()[1]<<","
				<<obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getContrast()[2]<<")"<<std::endl;

			std::cout<<"offset GBR=("
				<<(int)obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getOffset()[0]<<","
				<<obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getOffset()[1]<<","
				<<obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getOffset()[2]<<")"<<std::endl;

			std::cout<<"mode="<<obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getMode()<<" (0 means rotation,1 means reflection)"<<std::endl;

			std::cout<<"angle="<<obj[ry/RANGEBLOCKSIZE*OBJECTREFERENCE+rx/RANGEBLOCKSIZE].getAngle()<<std::endl;

			std::cout<<"PSNR="<<psnr<<std::endl;

			std::cout<<"processing time="<<((double)cv::getTickCount() - t)/cv::getTickFrequency()/60<<"min"<<std::endl;

		}//Range Block Loop End
	}//Range Block Loop End

	std::cout<<"getFractalCodeElements function End"<<std::endl;

}