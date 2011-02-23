/*
  Anaglyph split. Given an anaglyph image, splits it up into the stereo pair,
  with right image containing red channel and left image containing green and
  blue channel.
  
  Developed by: Matheus Ricardo Uihara Zingarelli
  Creation date: Feb, 23rd. 2011
  Last modification: Feb, 23rd. 2011
*/

//anaglyph-split <anaglyph_image>

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
        }        
    }        
    
    //save new images
    cvSaveImage("right.bmp", imgR);
    cvSaveImage("left.bmp", imgL);
    
    //release pointers
    cvReleaseImage(&anaglyph);
    cvReleaseImage(&imgR);
    cvReleaseImage(&imgL);
    
    return 0;
}
