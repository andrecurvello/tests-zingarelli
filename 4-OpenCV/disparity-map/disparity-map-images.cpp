/*
  Computes the disparity map of stereo pair images

  Developed by: Matheus Ricardo Uihara Zingarelli
  Creation date: Feb, 28th. 2011
  Last modification: Feb, 28th. 2011
*/

//disparity-map-images <left_image> <right_image>

#include <stdio.h>
#include <cv.h>
#include <highgui.h>

int main(int argc, char* argv[]){
    //load images
    //ATTENTION!! 8bits IMAGES ONLY
    IplImage* imgL = cvLoadImage(argv[1],0);
    IplImage* imgR = cvLoadImage(argv[2],0);
    
    //Simple error handling
    if((!imgL)||(!imgR)){
       printf("Erro ao abrir as imagens.");
       exit(-1);
    }
    
    //Verify if both images are of the same size
    if((imgR->width != imgL->width) ||
       (imgR->height != imgL->height)){
          printf("Imagens possuem tamanhos diferentes. Favor redimensionar!");
          exit(-1);
    }
    
    //setup stereo correspondence structure
    CvStereoBMState *BMState = cvCreateStereoBMState();
    BMState->preFilterSize = 41;
    BMState->preFilterCap=31;
    BMState->SADWindowSize=41;
    BMState->minDisparity=-64;
    BMState->numberOfDisparities=128;
    BMState->textureThreshold=10;
    BMState->uniquenessRatio=15;
    
    //run block matching algorithm
    CvMat* disp = cvCreateMat(imgL->height, imgL->width, CV_16S);
    CvMat* vdisp = cvCreateMat(imgL->height,imgL->width, CV_8U);
    cvFindStereoCorrespondenceBM(imgL, imgR, disp, BMState);
    cvNormalize(disp, vdisp, 0, 256, CV_MINMAX);
    
    cvSaveImage("disparity.bmp", disp);
    cvSaveImage("disparity-normalized.bmp", vdisp);
    
    //free pointers
    cvReleaseImage(&imgL);
    cvReleaseImage(&imgR);
    cvReleaseMat(&disp);
    cvReleaseMat(&vdisp);
    
    return 0;
}
