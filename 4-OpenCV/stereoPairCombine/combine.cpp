/*
  Developed by: Matheus Ricardo Uihara Zingarelli
  Creation date: November, 17th 2012
  Last modification: November, 17th 2012

  Given two separate images of a stereo pair, combine them into a single one.
  User chooses either side-by-side or above-below formats

  Usage:
        combine <imageLeft.bmp> <imageRight.bmp> {-sbs | -ab}

        -sbs: side-by-side image
        -ab: above/below image
*/

#include <stdio.h>
#include <cv.h>
#include <highgui.h>

/*
  Separate images from a side-by-side or above/below image. Given the original
  image dimensions, set a ROI (region of interest) based on the image type. If
  it's side-by-side, the ROI will be half of the original image's width. If it's
  above-below, the ROI will be half of the original image's height. Only the pixels
  inside the ROI will be copied by cvCopy
  Input: frame - original image
         frameL, frameR - stores left and right image, respectively
         width, height - original image dimensions
         imageType - -sbs if the original image has side-by-side images, -ab
            if it has above-below images
*/

int main(int argc, char* argv[]){
    printf("\n\nSTEREOPAIR COMBINE\n\nUsage:\tcombine <imageLeft.bmp> <imageRight.bmp> {-sbs | -ab}\n\n");
    char *imageType = argv[3]; //-sbs:sid-by-side or -ab:above/below

    printf("[INFO]\tLoading images...\n");
    IplImage *imageLeft = cvLoadImage(argv[1], 1);
    IplImage *imageRight = cvLoadImage(argv[2], 1);

    //Simple error handling
    if(!imageLeft || ! imageRight){
       printf("| [ERROR] Error opening BMP file.\n");
       exit(-1);
    }

    printf("[INFO]\tCreating container for the stereo pair...\n");
    int width = imageLeft->width;
    int height = imageLeft->height;

    //get size of the two images from the stereo pair, based on its type
    CvSize size;
    if(!strcmp(imageType, "-sbs")){
        size = cvSize( width*2, height);
    }
    else if(!strcmp(imageType, "-ab")){
        size = cvSize( width, height*2);
    }
    else{
        printf("| [ERROR] Invalid argument: %s\n", imageType);
        exit(-1);
    }

    IplImage *stereopair = cvCreateImage(size, imageLeft->depth, imageLeft->nChannels);
    cvZero(stereopair);

    printf("[INFO]\tCombining images...\n");
    if(!strcmp(imageType, "-sbs")){
        for(int row = 0; row < height; row++){
            //set pointer to the correct position in each row
            uchar* ptrS = (uchar*)(stereopair->imageData + row * stereopair->widthStep);
            uchar* ptrL = (uchar*)(imageLeft->imageData + row * imageLeft->widthStep);
            uchar* ptrR = (uchar*)(imageRight->imageData + row * imageRight->widthStep);
            //TODO: ver porque a segunda metade não está funcionando!!
            for(int col = 0; col < width; col++){
                ptrS[3*col] = ptrL[3*col];                           //B left
                ptrS[3*(col+width)] = ptrR[3*col];        //B right
                ptrS[3*col+1] = ptrL[3*col+1];                       //G left
                ptrS[3*(col+width)+1] = ptrR[3*col+1];    //G right
                ptrS[3*col+2] = ptrL[3*col+2];                       //R left
                ptrS[3*(col+width)+2] = ptrR[3*col+2];    //R right
            }
        }
    }

    printf("[INFO]\tSaving image...\n");
    /*char* s1 = strtok(argv[1],".");
    char* s2 = strtok(argv[2],".");
    char imageName[] = "";
    strcat(imageName,s1);
    strcat(imageName,"-");
    strcat(imageName,s2);
    strcat(imageName,"-combined.bmp");*/



    cvSaveImage("combined.bmp", stereopair);

    cvReleaseImage(&stereopair);
    cvReleaseImage(&imageLeft);
    cvReleaseImage(&imageRight);

    return 0;
}
