/*
  Saves pixel's data from a given image.

  Developed by: Matheus Ricardo Uihara Zingarelli
  Creation date: April, 12th 2011
  Last modification: April, 6th 2011
  
  Usage:
        saveImageData <file.bmp>
*/

#include <stdio.h>
#include <cv.h>
#include <highgui.h>

int main(int argc, char* argv[]){
    IplImage *img;
    FILE *fp;
    uchar *vet;
    
    if(argc != 2){
        printf("Usage:\n\tprintImageData <file.bmp>\n");
        exit(-1);
    }
    
    img = cvLoadImage(argv[1], 1);
    
    if(!img){
        printf("Error opening file. Please, try again\n");
        exit(-1);         
    }
    
    //traverse image and save data
    vet = (uchar*)malloc(img->width*img->height);
    fp = fopen("pixelData.txt", "w");
    if(!fp){
        printf("Error creating pixel data file. Please, verify\n");
    }
    
    int count = 0;
    for(int row = 0; row < img->height; row++){
        uchar *ptrImg = (uchar*) (img->imageData + row * img->widthStep);
        for(int col = 0; col < img->width; col++){
            //get values from red channel of the image
            vet[count] = ptrImg[3*col+2];
            count++;
        }
    }   
    
    fwrite(vet, sizeof(uchar), count, fp);
    
    printf("Data saved successfully!\n");
    fclose(fp);
    
    //testing creating an image with data from file
    fp = fopen("pixelData.txt","r");
    uchar *reverse = (uchar*)malloc(img->width*img->height);
    fread(reverse, sizeof(uchar), count, fp);
    
    IplImage *Rchannel;
    CvSize size = cvSize(img->width, img->height);
    Rchannel = cvCreateImage(size, img->depth, img->nChannels);
    cvZero(Rchannel);
    
    count = 0;    
    for(int row = 0; row < Rchannel->height; row++){
        uchar *ptrImg = (uchar*) (Rchannel->imageData + row * Rchannel->widthStep);
        for(int col = 0; col < Rchannel->width; col++){
            ptrImg[3*col+2] = reverse[count];
            count++;
        }      
        printf("Count %d\n", count);
    } 
    
    cvSaveImage("Red_Channel.bmp", Rchannel);
    
    fclose(fp);
    return 0;
}
