/*
  Developed by: Matheus Ricardo Uihara Zingarelli
  Creation date: April, 9th 2012
  Last modification: April, 9th 2012

  Given two stereoscopic images (in two separate files), creates an
  above-below stereopair.

  Usage:
    createAboveBelow img_top.bmp(left) img_bottom.bmp (right)
*/

#include <stdio.h>
#include <cv.h>
#include <highgui.h>

int main(int argc, char* argv[])
{
    printf("Creating the stereo pair... ");
    IplImage* img_top = cvLoadImage(argv[1], 1);
    IplImage* img_bottom = cvLoadImage(argv[2], 1);
    IplImage* stereoPair = cvCreateImage(cvSize((img_top->width), img_top->height * 2), img_top->depth, img_top->nChannels);
    cvZero(stereoPair);
    for(int row = 0; row < img_top->height; row++){
        //set pointer to the correct position in each row
        uchar* ptrStp = (uchar*)(stereoPair->imageData + row * stereoPair->widthStep);
        uchar* ptrTop = (uchar*)(img_top->imageData + row * img_top->widthStep);
        for(int col = 0; col < img_top->width; col++){
            ptrStp[3*col] = ptrTop[3*col];
            ptrStp[3*col+1] = ptrTop[3*col+1];
            ptrStp[3*col+2] = ptrTop[3*col+2];
        }
    }
    int rowStp = img_top->height; //pointer to the second image of the stereopair
    for(int row = 0; row < img_top->height; row++){
        //set pointer to the correct position in each row
        uchar* ptrBottom = (uchar*)(img_bottom->imageData + row * img_bottom->widthStep);
        //the second image of the stereopair is height rows below, so we need to set the pointer accordingly
        uchar* ptrStp = (uchar*)(stereoPair->imageData + rowStp * stereoPair->widthStep);
        for(int col = 0; col < img_top->width; col++){
            ptrStp[3*col] = ptrBottom[3*col];
            ptrStp[3*col+1] = ptrBottom[3*col+1];
            ptrStp[3*col+2] = ptrBottom[3*col+2];
        }
        rowStp++;
    }

    //save image
    cvSaveImage("above-below.bmp", stereoPair);
    printf("OK!\n");

    cvReleaseImage(&stereoPair);
    cvReleaseImage(&img_top);
    cvReleaseImage(&img_bottom);
}
