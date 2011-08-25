/* 
  Developed by: Matheus Ricardo Uihara Zingarelli
  Creation date: August, 25th 2011
  Last modification: August, 25th 2011
  
  Given an stereo pair (sbs or ab), split it into two separate images
  
  Usage:
        splitter <image.bmp> {-sbs | -ab}
        
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
void separateImages(IplImage *frame, IplImage **frameL, IplImage **frameR, int width, int height, char *imageType){    
    printf("Dividing the stereo pair into two images... ");
    //split images
    if(!strcmp(imageType,"-sbs")){//side-by-side
        //set ROI to cut the image on the left (half of the original)
        cvSetImageROI(frame, cvRect(0,0, width/2, height));            
        //copy image portion
        cvCopy(frame, *frameL);            
        //reset ROI
        cvResetImageROI(frame);            
        //set ROI to cut the image on the right (second half of the original)
        cvSetImageROI(frame, cvRect(width/2, 0, width/2, height));            
        //copy image portion
        cvCopy(frame, *frameR);            
        //reset ROI
        cvResetImageROI(frame);
    }
    else{//above-below
        //set ROI to cut the image on the top (half of the original)
        cvSetImageROI(frame, cvRect(0,0, width, height/2));            
        //copy image portion
        cvCopy(frame, *frameL);            
        //reset ROI
        cvResetImageROI(frame);            
        //set ROI to cut the image on the bottom (second half of the original)
        cvSetImageROI(frame, cvRect(0, height/2, width, height/2));            
        //copy image portion
        cvCopy(frame, *frameR);            
        //reset ROI
        cvResetImageROI(frame);
    }      
    printf("OK!\n");
}


int main(int argc, char* argv[]){    
    char *imageType = argv[2]; //-sbs:sid-by-side or -ab:above/below
    IplImage *frame, *frameL, *frameR;        
    //load image
    //images are loaded by openCV with color channels in this order: BGR
    frame = cvLoadImage(argv[1], 1);
            
    //Simple error handling
    if(!frame){
       printf("Error opening BMP file.");
       exit(-1);
    }            
    //some verifications regarding the image
    if(frame->width % 2 != 0){
          printf("Image width is not divisible by 2. Please, resize it!");
          exit(-1);
    }            
    int width = frame->width;
    int height = frame->height;      
        
    //get size of the two images from the stereo pair, based on its type
    CvSize size;
    if(!strcmp(imageType, "-sbs")){
        size = cvSize( width/2, height);
    }
    else if(!strcmp(imageType, "-ab")){
        size = cvSize( width, height/2);
    }
    else{
        exit(-1);                    
    }
    
    //copy image properties
    frameL = cvCreateImage(size, frame->depth, frame->nChannels);
    frameR = cvCreateImage(size, frame->depth, frame->nChannels);
    cvZero(frameL);
    cvZero(frameR);
    
    //divide the stereo pair into two images
    separateImages(frame, &frameL, &frameR, width, height, imageType);
          
    //save images
    printf("Saving images... ");
    char* fileNoExtension = strtok(argv[1],".");
    char* left = (char*) malloc(sizeof(char)*(strlen(fileNoExtension)+9));
    char* right = (char*) malloc(sizeof(char)*(strlen(fileNoExtension)+10));
    strcpy(left, fileNoExtension);
    strcat(left,"-left.bmp");
    strcpy(right, fileNoExtension);
    strcat(right,"-right.bmp");
    cvSaveImage(left, frameL);
    cvSaveImage(right, frameR);
    printf("OK!\n");
    
    return 0;
}
