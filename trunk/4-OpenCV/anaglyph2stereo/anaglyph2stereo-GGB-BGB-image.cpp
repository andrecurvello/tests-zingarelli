/*
  Given an anaglyph image, splits it into the left and right pair of images.
  For the image on the left (missing red channel from the anaglyph convertion),
  copy green or blue channel values.

  Developed by: Matheus Ricardo Uihara Zingarelli
  Creation date: Mar, 1st. 2011
  Last modification: Mar, 1st. 2011
*/

//anaglyph2stereo-GGB-BGB-image <anaglyph_image> 0|1
//0: copy green values
//1: copy blue values

#include <stdio.h>
#include <cv.h>
#include <highgui.h>

int main(int argc, char* argv[]){
    IplImage *anaglyph, *imgR, *imgL;
    
    //images are loaded by openCV with colors in this order: BGR
    //load anaglyph image
    anaglyph = cvLoadImage(argv[1],1);
    
    if(!anaglyph){
        printf("Erro ao abrir arquivo de imagem.");
        exit(-1);
    }
        
    //copy anaglyph image properties to the new images
    CvSize size = cvGetSize(anaglyph);
    imgR = cvCreateImage(size, anaglyph->depth, anaglyph->nChannels);
    imgL = cvCreateImage(size, anaglyph->depth, anaglyph->nChannels);
    cvZero(imgR);
    cvZero(imgL);
    
    //loop into anaglyp image
    for(int row = 0; row < anaglyph->height; row++){
        //set pointer to the correct position in each row
        uchar* ptrA = (uchar*)(anaglyph->imageData + row * anaglyph->widthStep);
        uchar* ptrR = (uchar*)(imgR->imageData + row * imgR->widthStep);
        uchar* ptrL = (uchar*)(imgL->imageData + row * imgL->widthStep);
        for(int col = 0; col < anaglyph->width; col++){
            //copy R channel to the right image of the pair
            ptrR[3*col+2] = ptrA[3*col+2];
            //copy G and B channels to the left image of the pair
            ptrL[3*col] = ptrA[3*col];
            ptrL[3*col+1] = ptrA[3*col+1];
            if(atoi(argv[2])==0){
                //copy G channel values to R channel (missing channel from anaglyph convertion)
                ptrL[3*col+2] = ptrA[3*col+1];
            } else{
                //copy B channel values to R channel (missing channel from anaglyph convertion)
                ptrL[3*col+2] = ptrA[3*col];
            }
        }        
    }        
    
    //save new images
    cvSaveImage("right-Red.bmp", imgR);
    if(atoi(argv[2])==0){
        cvSaveImage("left-GGB.bmp", imgL);
    } else{
        cvSaveImage("left-BGB.bmp", imgL);
    }
    
    //release pointers
    cvReleaseImage(&anaglyph);
    cvReleaseImage(&imgR);
    cvReleaseImage(&imgL);
    
    
    return 0;
}
