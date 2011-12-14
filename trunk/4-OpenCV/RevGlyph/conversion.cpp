/*
  Developed by: Matheus Ricardo Uihara Zingarelli
  Creation date: December, 14th 2011
  Last modification: December, 14th 2011

  This library contains procedures for anaglyph conversion of stereoscopic images.
  The conversion is implemented in a way that enables posterior reversion of the process,
  with the creation of what we called "Color Index Table".
*/

#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include "conversion.h"

uchar createMetadata(char* parameters[]){
    uchar control = 0;
    //verify anaglyph type. Nothing needs to be done to green-magenta (00 bits)
    if(!strcmp(parameters[4],"-rc")){//01000000 = 64
        control = control | 64;
    } else if(!strcmp(parameters[4],"-by")){//10000000 = 128
        control |= 128;
    }
    //verify stereopair orientation. Nothing needs to be done to side-by-side (0 bit)
    if(!strcmp(parameters[3],"-ab")){//00100000 = 32
        control |= 32;
    }
    //verify subsampling type. Nothing needs to be done to 4:4:0 (0 bit)
    if(!strcmp(parameters[5],"-422")){//00010000 = 16
        control |= 16;
    }
    return control;
}


void saveData(uchar* anaglyph,uchar* cit, char* parameters[], int imageSize){
    printf("Saving data... ");
    char* imageName = strtok(parameters[2],".");
    char filename[] = "";
    strcpy(filename, imageName);
    strcat(filename,"compressed.dat");
    FILE *fp;
    fp = fopen(filename,"wb");
    if(fp == NULL){
        printf("ERROR!\n\tError opening file %s", filename);
        exit(-1);
    }
    //metadata
    uchar control = createMetadata(parameters);
    fputc((int)control, fp);
    int imgSize[1] = {imageSize};
    fwrite(imgSize, sizeof(int), 1, fp);
    //anaglyph data (2*imageSize bytes)
    fwrite(anaglyph, sizeof(uchar), 2*imageSize, fp);
    //color index table data (imageSize bytes)
    fwrite(cit, sizeof(uchar), imageSize, fp);
    printf("OK!\n");
    fclose(fp);

//-------UNIT TEST
//printf("\n\n\tControl %d\n\n", control);
}


uchar* createCIT(uchar* data, int imageSize){
    printf("Creating the Color Index Table... ");
    uchar* cit = (uchar*)malloc(imageSize*sizeof(uchar));
    //copy Cb and Cr data
    for(int i=0; i<imageSize;i++){cit[i]=data[i];}
    printf("OK!\n");
    return cit;
}


uchar* subsample440(IplImage* image){
    //YCbCr conversion
    printf("\tConverting from RGB to YCbCr color space... ");
    cvCvtColor(image, image, CV_BGR2YCrCb);
    printf("OK!\n");

    //placeholder for subsampled data
    int imageSize = image->width * image->height;
    uchar *dataStream = (uchar*) malloc(2*imageSize*sizeof(uchar));

    //4:4:0 works through copying the average color values from even and odd rows
    //this way, it is a horizontal spatial reduction
    printf("\tApplying 4:4:0 subsampling... ");
    int countCbCr = 0;
    int countY = 0;
    for(int row = 0; row < image->height; row++){
        uchar* ptr = (uchar*)(image->imageData + row * image->widthStep);
        if(row % 2 == 0){
            //copy Y, Cb and Cr values of even rows
            for(int col = 0; col < image->width; col++){
                dataStream[countCbCr] =             ptr[3*col+1]; //Cb
                dataStream[countCbCr+imageSize/2] = ptr[3*col+2]; //Cr
                dataStream[countY+imageSize] =      ptr[3*col];   //Y
                countY++;
                countCbCr++;
            }//column iteration
        }//if even rows
        else{
            //copy Y, backtrack width positions on dataStream (that holds Cb and Cr data)
            //and calculate the average values
            countCbCr -= image->width;
            for(int col = 0; col < image->width; col++){
                dataStream[countCbCr] =             (dataStream[countCbCr]+ptr[3*col+1])/2; //Cb
                dataStream[countCbCr+imageSize/2] = (dataStream[countCbCr+imageSize/2] + ptr[3*col+2])/2; //Cr
                dataStream[countY+imageSize] =      ptr[3*col]; //Y
                countY++;
                countCbCr++;
            }//column iteration
        }//if odd rows
    }//row iteration
    printf("OK!\n");
    return dataStream;
}


void createAnaglyph(IplImage* left, IplImage* right, IplImage* mainAnaglyph, IplImage* complAnaglyph, char* type){
    if(!strcmp(type,"-gm")){//green-magenta anaglyph
        printf("Creating green-magenta and complementary anaglyphs... ");
        for(int row = 0; row < left->height; row++){
            uchar* ptrR = (uchar*)(right->imageData + row * right->widthStep);
            uchar* ptrL = (uchar*)(left->imageData + row * left->widthStep);
            uchar* ptrA = (uchar*)(mainAnaglyph->imageData + row * mainAnaglyph->widthStep);
            uchar* ptrC = (uchar*)(complAnaglyph->imageData + row * complAnaglyph->widthStep);
            for(int col = 0; col < left->width; col++){
                //Main anaglyph: green channel from the right image and red and blue channel from the left image
                ptrA[3*col] = ptrL[3*col]; //B
                ptrA[3*col+1] = ptrR[3*col+1]; //G
                ptrA[3*col+2] = ptrL[3*col+2]; //R
                //Complementary anaglyph: green channel from the left image and red and blue channel from the right image
                ptrC[3*col] = ptrR[3*col]; //B
                ptrC[3*col+1] = ptrL[3*col+1]; //G
                ptrC[3*col+2] = ptrR[3*col+2]; //R
            }
        }
        printf("OK!\n");
    }
    else if(!strcmp(type,"-rc")){//red-cyan anaglyph
        printf("Creating red-cyan and complementary anaglyphs... ");
        for(int row = 0; row < left->height; row++){
            uchar* ptrR = (uchar*)(right->imageData + row * right->widthStep);
            uchar* ptrL = (uchar*)(left->imageData + row * left->widthStep);
            uchar* ptrA = (uchar*)(mainAnaglyph->imageData + row * mainAnaglyph->widthStep);
            uchar* ptrC = (uchar*)(complAnaglyph->imageData + row * complAnaglyph->widthStep);
            for(int col = 0; col < left->width; col++){
                //Main anaglyph: red channel from the right image and green and blue channel from the left image
                ptrA[3*col] = ptrL[3*col]; //B
                ptrA[3*col+1] = ptrL[3*col+1]; //G
                ptrA[3*col+2] = ptrR[3*col+2]; //R
                //Complementary Anaglyph: copy red channel from the left image and green and blue channel from the right image
                ptrC[3*col] = ptrR[3*col]; //B
                ptrC[3*col+1] = ptrR[3*col+1]; //G
                ptrC[3*col+2] = ptrL[3*col+2]; //R
            }
        }
        printf("OK!\n");
    }
    else if(!strcmp(type,"-by")){//blue-yellow anaglyph
        printf("Creating blue-yellow and complementary anaglyphs... ");
        for(int row = 0; row < left->height; row++){
            uchar* ptrR = (uchar*)(right->imageData + row * right->widthStep);
            uchar* ptrL = (uchar*)(left->imageData + row * left->widthStep);
            uchar* ptrA = (uchar*)(mainAnaglyph->imageData + row * mainAnaglyph->widthStep);
            uchar* ptrC = (uchar*)(complAnaglyph->imageData + row * complAnaglyph->widthStep);
            for(int col = 0; col < left->width; col++){
                //Main anaglyph: blue channel from the right image and red and green channel from the left image
                ptrA[3*col] = ptrR[3*col]; //B
                ptrA[3*col+1] = ptrL[3*col+1]; //G
                ptrA[3*col+2] = ptrL[3*col+2]; //R
                //Complementary anaglyph: blue channel from the left image and red and green channel from the right image
                ptrC[3*col] = ptrL[3*col]; //B
                ptrC[3*col+1] = ptrR[3*col+1]; //G
                ptrC[3*col+2] = ptrR[3*col+2]; //R
            }
        }
        printf("OK!\n");
    }
}


IplImage* loadAndVerify(char* image, char* type){
    //load image file
    printf("Loading original image... ");
    IplImage* stereoPair = cvLoadImage(image, 1);
    if(!stereoPair){
       printf("ERROR!\n\tError opening BMP file %s.", image);
       exit(-1);
    }

    //verify image size
    if(!strcmp(type,"-sbs") && stereoPair->width % 2 != 0){
          printf("ERROR!\n\tImage width is not divisible by 2. Please, resize it!");
          exit(-1);
    }
    if(!strcmp(type,"-ab") && stereoPair->height % 2 != 0){
          printf("ERROR!\n\tImage height is not divisible by 2. Please, resize it!");
          exit(-1);
    }
    printf("OK!\n");
    return(stereoPair);
}


void prepareImages(IplImage* stereoPair, char* type, IplImage** left,IplImage** right, IplImage** mainAnaglyph, IplImage** complAnaglyph){
    //get size of the stereo pair, based on its type
    CvSize size;
    if(!strcmp(type, "-sbs")){
        size = cvSize( stereoPair->width/2, stereoPair->height);
    }
    else if(!strcmp(type, "-ab")){
        size = cvSize( stereoPair->width, stereoPair->height/2);
    }

    //copy image properties
    *left = cvCreateImage(size, stereoPair->depth, stereoPair->nChannels);
    *right = cvCreateImage(size, stereoPair->depth, stereoPair->nChannels);
    *mainAnaglyph = cvCreateImage(size, stereoPair->depth, stereoPair->nChannels);
    *complAnaglyph = cvCreateImage(size, stereoPair->depth, stereoPair->nChannels);
    cvZero(*left);
    cvZero(*right);
    cvZero(*mainAnaglyph);
    cvZero(*complAnaglyph);
}


void splitImage(IplImage* stereoPair, char* type, IplImage** left, IplImage** right){
    printf("Dividing the stereo pair into two images... ");
    if(!strcmp(type,"-sbs")){
        //set ROI to copy the first half of the image width (left image)
        cvSetImageROI(stereoPair, cvRect(0,0, stereoPair->width/2, stereoPair->height));
        cvCopy(stereoPair, *left);
        cvResetImageROI(stereoPair);

        //set ROI to copy the second half of the image width (right image)
        cvSetImageROI(stereoPair, cvRect(stereoPair->width/2, 0, stereoPair->width/2, stereoPair->height));
        cvCopy(stereoPair, *right);
        cvResetImageROI(stereoPair);
    }
    else if(!strcmp(type, "-ab")){
        //set ROI to copy the first half of the image height (left image)
        cvSetImageROI(stereoPair, cvRect(0,0, stereoPair->width, stereoPair->height/2));
        cvCopy(stereoPair, *left);
        cvResetImageROI(stereoPair);

        //set ROI to copy the second half of the image height (right image)
        cvSetImageROI(stereoPair, cvRect(0, stereoPair->height/2, stereoPair->width, stereoPair->height/2));
        cvCopy(stereoPair, *right);
        cvResetImageROI(stereoPair);
    }
    printf("OK!\n");
}


void anaglyphConversion(char* parameters[]){
    IplImage *stereopair = NULL;
    IplImage *left = NULL;
    IplImage *right = NULL;
    IplImage *mainAnaglyph = NULL;
    IplImage *complAnaglyph = NULL;

    //load the stereo pair
    stereopair = loadAndVerify(parameters[2],parameters[3]);

    //prepare images with properties from the original stereo image
    prepareImages(stereopair, parameters[3], &left, &right, &mainAnaglyph, &complAnaglyph);

    //split the stereo pair
    splitImage(stereopair, parameters[3], &left, &right);

    //create the anaglyphs
    createAnaglyph(left, right, mainAnaglyph, complAnaglyph, parameters[4]);

    //spacial compression
    printf("Compressing main anaglyph...\n");
    uchar* anaglyph = subsample440(mainAnaglyph);
    printf("OK!\n");
    printf("Compressing complementary anaglyph...\n");
    uchar* c_anaglyph = subsample440(complAnaglyph);
    printf("OK!\n");

    //Color Index Table
    uchar* cit = createCIT(c_anaglyph, complAnaglyph->width*complAnaglyph->height);

    //store data in a single file
    saveData(anaglyph, cit, parameters, complAnaglyph->width*complAnaglyph->height);


//------UNIT TEST
//cvSaveImage("left.bmp", left);
//cvSaveImage("right.bmp", right);
//cvSaveImage("main-anaglyph.bmp", mainAnaglyph);
//cvSaveImage("complementary-anaglyph.bmp", complAnaglyph);
//FILE *fp = fopen("saida.dat", "wb");
//if(!fp){
//    printf(" ERROR!\nError creating green-magenta anaglyph file\n");
//    exit(-1);
//}
//fwrite(anaglyph, sizeof(uchar),2*left->width*left->height, fp);
//fclose(fp);
//FILE *fp = fopen("color-index-table.dat", "wb");
//if(!fp){
//    printf(" ERROR!\nError creating green-magenta anaglyph file\n");
//    exit(-1);
//}
//fwrite(cit, sizeof(uchar),left->width*left->height, fp);
//fclose(fp);


    free(anaglyph);
    free(c_anaglyph);
    free(cit);
    cvReleaseImage(&stereopair);
    cvReleaseImage(&left);
    cvReleaseImage(&right);
    cvReleaseImage(&mainAnaglyph);
    cvReleaseImage(&complAnaglyph);
}

