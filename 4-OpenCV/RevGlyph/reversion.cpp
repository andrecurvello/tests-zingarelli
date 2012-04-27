/*
  Developed by: Matheus Ricardo Uihara Zingarelli
  Creation date: January, 4th 2012
  Last modification: February, 1st 2012

  This library contains procedures for anaglyph reversion of stereoscopic images.
  The reversion is enabled due to proper conversion using the Color Index Table.

  Modifications (dd/mm/aaaa):
  (01/02/2012)
  Creation of the luminanceDiff vector, to test the hypothesis of storing the differences
  from both luminance components, and use this difference to recreate Y from the complementary
  anaglyph (Yc). Our hypothesis is that this difference will have a large amount of values
  near zero, so we could stablish a threshold to obtain string of zeroes and the compress it
  with run-length, obtaining great compression.

  (01/02/2012)
  Implemented Run-length Decoding algorithm (RLD)

  (09/04/2012)
  Created function to create an above-below stereopair
*/

#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include "reversion.h"

#define PIXELDATA_OFFSET 13 //position where pixel data starts in the compressed file (1 byte from metadata + 4 bytes for width + 4 bytes for height + 4 bytes for pixel depth)

//RLE struct
struct rle_struct{
    char value; //value of the difference between pixels of Ym and Yc
    uchar qty;  //number of times the value is repeated in a sequence
};

void createABImage(IplImage* mainAnaglyph, IplImage* complAnaglyph, char* filename){
    printf("Creating the stereo pair... ");
    IplImage* stereoPair = cvCreateImage(cvSize((mainAnaglyph->width), mainAnaglyph->height * 2), mainAnaglyph->depth, mainAnaglyph->nChannels);
    cvZero(stereoPair);
    for(int row = 0; row < mainAnaglyph->height; row++){
        //set pointer to the correct position in each row
        uchar* ptrStp = (uchar*)(stereoPair->imageData + row * stereoPair->widthStep);
        uchar* ptrAn = (uchar*)(mainAnaglyph->imageData + row * mainAnaglyph->widthStep);
        uchar* ptrAc = (uchar*)(complAnaglyph->imageData + row * complAnaglyph->widthStep);
        for(int col = 0; col < mainAnaglyph->width; col++){
            ptrStp[3*col] = ptrAn[3*col];                           //B left
            ptrStp[3*col+1] = ptrAc[3*col+1];                       //G left
            ptrStp[3*col+2] = ptrAn[3*col+2];                       //R left
        }
    }
    int rowStp = mainAnaglyph->height; //pointer to the second image of the stereopair
    for(int row = 0; row < mainAnaglyph->height; row++){
        //set pointer to the correct position in each row
        uchar* ptrAn = (uchar*)(mainAnaglyph->imageData + row * mainAnaglyph->widthStep);
        uchar* ptrAc = (uchar*)(complAnaglyph->imageData + row * complAnaglyph->widthStep);
        //the second image of the stereopair is height rows below, so we need to set the pointer accordingly
        uchar* ptrStp = (uchar*)(stereoPair->imageData + rowStp * stereoPair->widthStep);
        for(int col = 0; col < mainAnaglyph->width; col++){
            ptrStp[3*col] = ptrAc[3*col];     //B right
            ptrStp[3*col+1] = ptrAn[3*col+1]; //G right
            ptrStp[3*col+2] = ptrAc[3*col+2]; //R right
        }
        rowStp++;
    }

    //save image
    char* imageName = strtok(filename,"-");
    strcat(imageName,"reversed.bmp");
    //printf("\n\n%s\n\n", imageName);
    cvSaveImage(imageName, stereoPair);
    printf("OK!\n");

    cvReleaseImage(&stereoPair);
}

void createSBSImage(IplImage* mainAnaglyph, IplImage* complAnaglyph, char* filename){
    printf("Creating the stereo pair... ");
    IplImage* stereoPair = cvCreateImage(cvSize((mainAnaglyph->width) * 2, mainAnaglyph->height), mainAnaglyph->depth, mainAnaglyph->nChannels);
    cvZero(stereoPair);
    for(int row = 0; row < mainAnaglyph->height; row++){
        //set pointer to the correct position in each row
        uchar* ptrStp = (uchar*)(stereoPair->imageData + row * stereoPair->widthStep);
        uchar* ptrAn = (uchar*)(mainAnaglyph->imageData + row * mainAnaglyph->widthStep);
        uchar* ptrAc = (uchar*)(complAnaglyph->imageData + row * complAnaglyph->widthStep);
        for(int col = 0; col < mainAnaglyph->width; col++){
            ptrStp[3*col] = ptrAn[3*col];                           //B left
            ptrStp[3*(col+mainAnaglyph->width)] = ptrAc[3*col];     //B right
            ptrStp[3*col+1] = ptrAc[3*col+1];                       //G left
            ptrStp[3*col+2] = ptrAn[3*col+2];                       //R left
            ptrStp[3*(col+mainAnaglyph->width)+1] = ptrAn[3*col+1]; //G right
            ptrStp[3*(col+mainAnaglyph->width)+2] = ptrAc[3*col+2]; //R right
        }
    }

    //save image
    char* imageName = strtok(filename,"-");
    strcat(imageName,"reversed.bmp");
    //printf("\n\n%s\n\n", imageName);
    cvSaveImage(imageName, stereoPair);
    printf("OK!\n");

    cvReleaseImage(&stereoPair);
}

int extractRepresentation(char metadata){
    int result = metadata << 2;
    return (result & 128);
}

void rebuildStereoPair(IplImage* mainAnaglyph, IplImage* complAnaglyph, uchar metadata, char* filename){
    //find out if the original stereo pair was a side-by-side or above-below image
    int representation = extractRepresentation(metadata);

    //reorder color components
    switch(representation){
        case 0:
            createSBSImage(mainAnaglyph, complAnaglyph, filename);
            break;
        case 128:
            createABImage(mainAnaglyph, complAnaglyph, filename);
            break;
    }

//----- UNIT TEST
//printf("\n\nRepresentation: %d\n\n", representation);
}

void revertSubsampling440(IplImage* image, uchar* imageData, int* imageSize){
    int countY = 0; //iterate through Y data
    int countCbCr = 0; //iterate through Cb and Cr data
    for(int row = 0; row < image->height; row++){
        uchar* ptr = (uchar*)(image->imageData + row * image->widthStep);
        //in an odd row, we need to copy values from the last row
        //We achieve this by returning image->width positions in imageData.
        if(row % 2 != 0){
            countCbCr -= image->width;
        }
        for(int col = 0; col < image->width; col++){
            ptr[3*col]   = imageData[countY+imageSize[0]*imageSize[1]]; //Y
            ptr[3*col+1] = imageData[countCbCr]; //Cb
            ptr[3*col+2] = imageData[countCbCr+((imageSize[0]*imageSize[1])/2)]; //Cr
            countY++;
            countCbCr++;
        }
    }
}

int extractSubsampling(uchar metadata){
    int result = metadata << 3;
    return (result & 128);
}

void convert2RGB(IplImage* image, uchar* imageData, uchar metadata, int* imageSize){
    //find out which chrominance subsampling was used
    int subsampling = extractSubsampling(metadata);

    //revert chrominance subsampling
    printf("Reverting image to 4:4:4... ");
    switch(subsampling){
        case 0://4:4:0
            revertSubsampling440(image, imageData, imageSize);
            break;
        case 128://4:2:2
            //TODO
            printf("\n\nRevert subsampling 4:2:2... TBD\n\n");
        break;
    }
    printf("OK!\n");

    //YCbCr to RGB conversion
    printf("Converting from YCbCr to RGB color space... ");
    cvCvtColor(image, image, CV_YCrCb2BGR);
    printf("OK!\n");

//---- UNIT TEST
//printf("\n\nSubsampling type: %d\n\n", subsampling);
}

uchar* buildComplementaryAnaglyph(uchar* cit, uchar* Y, int imageSize){
    printf("Rebuilding the complementary anaglyph... ");
    uchar* data = (uchar*) malloc(sizeof(uchar)*2*imageSize);
    for(int i=0; i<imageSize; i++){
        data[i] = cit[i];
        data[i+imageSize] = Y[i];
    }
    printf("OK!\n");
    return data;
}

uchar* extractY(uchar* anaglyph, int imageSize){
    printf("Extracting Y data from main anaglyph... ");
    uchar* data = (uchar*) malloc(sizeof(uchar)*imageSize);
    for(int i=0; i<imageSize; i++){
        data[i] = anaglyph[i+imageSize];
    }
    printf("OK!\n");
    return data;
}

uchar* recoverComplY(char* filename, uchar* anaglyph, int imageSize){
    //luminance from main anaglyph
    uchar* Ym = extractY(anaglyph, imageSize);

    printf("Reading luminance differences data... ");
    //luminance from complementary anaglyph
    uchar* Yc = (uchar*)malloc(imageSize*sizeof(uchar));

    //luminance differences
    char* Yd = (char*)malloc(imageSize*sizeof(char));

    //open compressed file
    FILE *fp = fopen(filename,"rb");
    if(fp == NULL){
        printf("ERROR!\n\tError opening file %s", filename);
        exit(-1);
    }

    //ensure that it is pointing to the beginning of the file
    rewind(fp);

    //lumDiff data starts after PIXELDATA_OFFSET + 2*imageSize (main anaglyph) + imageSize (CIT) bytes
    int lumDiffOffset = PIXELDATA_OFFSET + (3*imageSize*sizeof(uchar));
    fseek(fp, lumDiffOffset, SEEK_SET);

    //read number of elements of the rle array
    int nelements[1];
    fread(nelements,sizeof(int),1,fp);

    //read lumDiff data
    struct rle_struct rle_elements[imageSize];
    fread(rle_elements,sizeof(rle_struct),nelements[0], fp);
    fclose(fp);

    //run-length decoding
    int i =0;
    for(int j = 0; j < nelements[0]; j++){
        while(rle_elements[j].qty > 0){
            Yd[i++] = rle_elements[j].value;
            rle_elements[j].qty--;
        }
    }

    for(int i=0; i<imageSize; i++){
        Yc[i] = Ym[i] - Yd[i];
    }

    printf("OK!\n");
    return Yc;
}

void rebuildAnaglyph(char* filename, IplImage* mainAnaglyph, IplImage* complAnaglyph, uchar* anaglyph, uchar* cit, uchar metadata, int* imageSize){
    int size = imageSize[0]*imageSize[1]; //width*height

    //extract Y from main anaglyph (modified to use the luminance difference approach
    //uchar* Y = extractY(anaglyph, size);

    //luminance difference approach
    uchar* Y = recoverComplY(filename, anaglyph, size);

    //build complementary anaglyph
    uchar* complementary = buildComplementaryAnaglyph(cit, Y, size);

    //revert to 4:4:4 RGB
    convert2RGB(mainAnaglyph, anaglyph, metadata, imageSize);
    convert2RGB(complAnaglyph, complementary, metadata, imageSize);

    free(Y);
    free(complementary);

//-----UNIT TEST
/*cvSaveImage("main.bmp", mainAnaglyph);
cvSaveImage("compl.bmp", complAnaglyph);*/
}

void readPixelData(char* filename, uchar** anaglyph, uchar** cit, int* imageSize){
    //open file
    FILE *fp = fopen(filename,"rb");
    if(fp == NULL){
        printf("ERROR!\n\tError opening file %s", filename);
        exit(-1);
    }

    //set pointer to the position where pixel data starts (13th byte of the file)
    fseek(fp, PIXELDATA_OFFSET, SEEK_SET);

    //read main anaglyph data
    int size = imageSize[0]*imageSize[1];
    uchar* mainData = (uchar*)malloc(2*size*sizeof(uchar));
    fread(mainData, sizeof(uchar), 2*size, fp);
    *anaglyph = mainData;

    //read Color Index Table data
    uchar* citData = (uchar*)malloc(size*sizeof(uchar));
    fread(citData, sizeof(uchar), size, fp);
    *cit = citData;
    fclose(fp);
}

void readMetadata(char* filename, uchar *metadata, int* imageSize){
    //open file
    FILE *fp = fopen(filename,"rb");
    if(fp == NULL){
        printf("ERROR!\n\tError opening file %s", filename);
        exit(-1);
    }

    //read data regarding anaglyph type, stereo pair disposition and subsampling type
    *metadata = fgetc(fp);

    //read the size of anaglyph (width, height and pixel depth)
    int imgSize[3] = {0};
    fread(imgSize, sizeof(int), 3, fp);
    imageSize[0] = imgSize[0];
    imageSize[1] = imgSize[1];
    imageSize[2] = imgSize[2];

    fclose(fp);

//----- UNIT TEST
//printf("\n**readMetadada**\n IMAGE SIZE: width - %d | height - %d | pixel depth - %d\n\n", imageSize[0], imageSize[1], imageSize[2]);
}

void anaglyphReversion(char* filename){
    //load and separate data from the compressed file
    uchar metadata = NULL;
    int imageSize[3] = {0};
    printf("Reading compressed file... ");
    readMetadata(filename, &metadata, imageSize);
    uchar* anaglyph = (uchar*)malloc(2*sizeof(uchar)*imageSize[0]*imageSize[1]);
    uchar* cit = (uchar*)malloc(sizeof(uchar)*imageSize[0]*imageSize[1]);
    readPixelData(filename, &anaglyph, &cit, imageSize);
    printf("OK!\n");

    //rebuild the complementary and main anaglyphs
    IplImage* mainAnaglyph = cvCreateImage(cvSize(imageSize[0], imageSize[1]), imageSize[2], 3);
    IplImage* complAnaglyph = cvCreateImage(cvSize(imageSize[0], imageSize[1]), imageSize[2], 3);
    cvZero(mainAnaglyph);
    cvZero(complAnaglyph);
    rebuildAnaglyph(filename, mainAnaglyph, complAnaglyph, anaglyph, cit, metadata, imageSize);

    //recreate the stereo pair
    rebuildStereoPair(mainAnaglyph, complAnaglyph, metadata, filename);

    cvReleaseImage(&mainAnaglyph);
    cvReleaseImage(&complAnaglyph);
    free(anaglyph);
    free(cit);

//---- UNIT TEST
//printf("\nCONTROL DATA: %d\n\n", metadata);
//printf("\nIMAGE SIZE: width - %d | height - %d | pixel depth - %d\n\n", imageSize[0], imageSize[1], imageSize[2]);
//FILE* fileA = fopen("Anaglyph.dat", "wb");
//fwrite(anaglyph, sizeof(uchar), 2*imageSize, fileA);
//fclose(fileA);
//fileA = fopen("cit.dat", "wb");
//fwrite(cit, sizeof(uchar), imageSize, fileA);
//fclose(fileA);
}
