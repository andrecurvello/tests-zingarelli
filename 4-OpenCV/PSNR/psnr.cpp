/*
  Given two images, img_A (modified) and img_B (original),
  calculates the PSNR between them

  Developed by: Matheus Ricardo Uihara Zingarelli
  Adapted from: Rodolfo Ribeiro Silva
  Creation date: February, 16th 2011
  Last modification: February, 17th 2011

  Usage:
        psnr <img_A.bmp> <img_B.bmp> -TYPE

        TYPE: rgb or ycbcr or yuv

  ycbcr uses RGB -> YCbCr conversion from OpenCV Library
  yuv implements REC.601 RGB->YUV conversion obtained from (http://compression.ru/video/quality_measure/info_en.html)

  Updates:
  Feb. 17h 2012:
    created RGB to YUV conversion.
    corrected order from results of the PSNR for YCbCr.
  Mar. 07th. 2012:
    added case when both images are equal (MSE return zero and the PSNR should return 100), avoiding division by zero.
*/

#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include "psnr.h"

void BGR2YUV(IplImage* src, IplImage* dst){
    for(int row = 0; row < src->height; row++){
            uchar* ptrSrc = (uchar*)(src->imageData + row * src->widthStep);
            uchar* ptrDst = (uchar*)(dst->imageData + row * dst->widthStep);
            for(int col=0; col < src->width; col++){
                //REC.601
                double Y = 0.257*ptrSrc[3*col+2] + 0.504*ptrSrc[3*col+1] + 0.098*ptrSrc[3*col] + 16;
                double U = -(0.148*ptrSrc[3*col+2]) - (0.291*ptrSrc[3*col+1]) + 0.439*ptrSrc[3*col] + 128;
                double V = 0.439*ptrSrc[3*col+2] - (0.368*ptrSrc[3*col+1]) - (0.071*ptrSrc[3*col]) + 128;

                //PC.601 with level shift
                /*double Y = 0.299*ptrSrc[3*col+2] + 0.587*ptrSrc[3*col+1] + 0.114*ptrSrc[3*col];
                double U = -(0.147*ptrSrc[3*col+2]) - (0.289*ptrSrc[3*col+1]) + 0.436*ptrSrc[3*col] + 128;
                double V = 0.615*ptrSrc[3*col+2] - (0.515*ptrSrc[3*col+1]) - (0.100*ptrSrc[3*col]) + 128;*/

                ptrDst[3*col] = (uchar)round(Y);
                ptrDst[3*col+1] = (uchar)round(U);
                ptrDst[3*col+2] = (uchar)round(V);
            }
    }
}

double MSE(IplImage* original, IplImage* processed){
    double diff = 0;
    for(int row = 0; row < original->height; row++){
            uchar* ptrOri = (uchar*)(original->imageData + row * original->widthStep);
            uchar* ptrProc = (uchar*)(processed->imageData + row * processed->widthStep);
            for(int col=0; col<original->width; col++){
                diff += (ptrOri[col] - ptrProc[col]) * (ptrOri[col] - ptrProc[col]);
            }
    }
    double result = diff / (original->width * original->height);
    return result;
}

double getPSNR(IplImage* original, IplImage* processed, int maxError){
    double mse = MSE(original, processed);
    //when both images are equal, MSE return zero and PSNR should be 100
    if(mse==0){return 100;}
    double result = 10 * log10(maxError*maxError / mse);
    return result;
}

void PSNR(IplImage* original, IplImage* processed, int maxError, double *res1, double *res2, double *res3, char* type){
    printf("Computing PSNR... ");
    //verify image size
    if((original->width != processed->width) || (original->height != processed->height) ||
       (original->nChannels != processed->nChannels) || (original->depth != processed->depth)){
        printf("ERROR!\n\tImages have different sizes and/or properties");
        exit(-1);
    }

    //verify if it is either RGB or YCbCr measurement
    //todo: print help if arguments are invalid
    if(!strcmp(type,"-ycbcr")){
        cvCvtColor(original,original,CV_BGR2YCrCb);
        cvCvtColor(processed,processed,CV_BGR2YCrCb);
    }
    else if(!strcmp(type,"-yuv")){
        BGR2YUV(original,original);
        BGR2YUV(processed,processed);
            //debug
            /*cvSaveImage("teste-YUV-1.bmp",original);
            cvSaveImage("teste-YUV-2.bmp",processed);*/
    }

    //split image channels
    CvSize size = cvSize(original->width, original->height);
    IplImage* ori_R = cvCreateImage(size, original->depth, 1);
    IplImage* ori_G = cvCreateImage(size, original->depth, 1);
    IplImage* ori_B = cvCreateImage(size, original->depth, 1);
    IplImage* proc_R = cvCreateImage(size, original->depth, 1);
    IplImage* proc_G = cvCreateImage(size, original->depth, 1);
    IplImage* proc_B = cvCreateImage(size, original->depth, 1);

    cvSplit(original, ori_B, ori_G, ori_R, NULL);
    cvSplit(processed, proc_B, proc_G, proc_R, NULL);

    //calculate PSNR for each channel
    //for YCbCr, ori_B will be Y, ori_R will be Cb and ori_G will be Cr
    //for YUV, ori_B will be Y, ori_G will be U and ori_R will be V
    if(!strcmp(type,"-rgb")){
        *res1 = getPSNR(ori_R, proc_R, maxError);
        *res2 = getPSNR(ori_G, proc_G, maxError);
        *res3 = getPSNR(ori_B, proc_B, maxError);
    }
    else if(!strcmp(type,"-ycbcr")){
        *res1 = getPSNR(ori_B, proc_B, maxError);
        *res2 = getPSNR(ori_R, proc_R, maxError);
        *res3 = getPSNR(ori_G, proc_G, maxError);
    }
    else if(!strcmp(type,"-yuv")){
        *res1 = getPSNR(ori_B, proc_B, maxError);
        *res2 = getPSNR(ori_G, proc_G, maxError);
        *res3 = getPSNR(ori_R, proc_R, maxError);
    }

    cvReleaseImage(&ori_R);
    cvReleaseImage(&ori_G);
    cvReleaseImage(&ori_B);
    cvReleaseImage(&proc_R);
    cvReleaseImage(&proc_G);
    cvReleaseImage(&proc_B);
    printf("OK!\n");
}
