/*
  Given two images, img_A (modified) and img_B (original),
  calculates the PSNR between them

  Developed by: Matheus Ricardo Uihara Zingarelli
  Adapted from: Rodolfo Ribeiro Silva
  Creation date: February, 16th 2011
  Last modification: February, 17th 2011

  Usage:
        psnr <img_A.bmp> <img_B.bmp> -TYPE

        TYPE: rgb or ycbcr
*/

#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include "psnr.h"

int main(int argc, char* argv[]){
    //load images
    IplImage* original = cvLoadImage(argv[1], 1);
    IplImage* processed = cvLoadImage(argv[2], 1);

    //PNSR values
    double res1 = 0.0;
    double res2 = 0.0;
    double res3 = 0.0;

    if(!original || !processed){
       printf("ERROR!\n\tError opening BMP files.");
       exit(-1);
    }

    PSNR(original, processed, 255, &res1, &res2, &res3, argv[3]);

    if(!strcmp(argv[3],"-rgb")){
        printf("\tPNSR - RGB\n");
        printf("\tR: %lf\n", res1);
        printf("\tG: %lf\n", res2);
        printf("\tB: %lf\n", res3);

    }
    else if(!strcmp(argv[3],"-ycbcr")){
        printf("\tPSNR - YCbCr");
        printf("\tY: %lf\n", res1);
        printf("\tCb: %lf\n", res2);
        printf("\tCr: %lf\n", res3);
    }
    else if(!strcmp(argv[3],"-yuv")){
        printf("\tPNSR - YUV\n");
        printf("\tY: %lf\n", res1);
        printf("\tU: %lf\n", res2);
        printf("\tV: %lf\n", res3);
    }

    cvReleaseImage(&original);
    cvReleaseImage(&processed);

}
