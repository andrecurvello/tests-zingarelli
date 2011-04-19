/*
  Saves pixel's data from a given image.

  Developed by: Matheus Ricardo Uihara Zingarelli
  Creation date: April, 12th 2011
  Last modification: April, 19th 2011
  
  Usage:
        saveImageData <file.bmp>
*/

#include <stdio.h>
#include <cv.h>
#include <highgui.h>

int main(int argc, char* argv[]){
    IplImage *img;
    FILE *fpR, *fpG, *fpB, *fp;
    uchar *vetR, *vetG, *vetB, *vet;
    
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
    int imageSize = img->width*img->height;
    vet = (uchar*)malloc(3*imageSize); //holds R data and then G data and then B data
    vetR = (uchar*)malloc(imageSize);
    vetG = (uchar*)malloc(imageSize);
    vetB = (uchar*)malloc(imageSize);
    fpR = fopen("pixelDataRed.dat", "wb");
    fpG = fopen("pixelDataGreen.dat", "wb");
    fpB = fopen("pixelDataBlue.dat", "wb");
    fp = fopen("pixelData.dat", "wb");
    if(!fpR || !fpG || !fpB || !fp){
        printf("Error creating pixel data file. Please, verify\n");
    }
    
    int count = 0;
    for(int row = 0; row < img->height; row++){
        uchar *ptrImg = (uchar*) (img->imageData + row * img->widthStep);
        for(int col = 0; col < img->width; col++){
            //get values from red channel of the image
            vetB[count] = ptrImg[3*col];
            vetG[count] = ptrImg[3*col+1];
            vetR[count] = ptrImg[3*col+2];
            vet[count]= ptrImg[3*col+2];
            vet[count+imageSize]= ptrImg[3*col+1];
            vet[count+(2*imageSize)]= ptrImg[3*col];
            count++;
        }
    }   
    
    fwrite(vetR, sizeof(uchar), imageSize, fpR);
    fwrite(vetG, sizeof(uchar), imageSize, fpG);
    fwrite(vetB, sizeof(uchar), imageSize, fpB);
    fwrite(vet, sizeof(uchar), 3*imageSize, fp);
    
    printf("Data saved successfully!\n");
    
    fclose(fpR);
    fclose(fpG);
    fclose(fpB);
    fclose(fp);
    
    //testing creating an image with data from file
    fpR = fopen("pixelDataRed.dat", "rb");
    fpG = fopen("pixelDataGreen.dat", "rb");
    fpB = fopen("pixelDataBlue.dat", "rb");
    fp = fopen("pixelData.dat", "rb");
    
    uchar *reverseR = (uchar*)malloc(imageSize);
    uchar *reverseG = (uchar*)malloc(imageSize);
    uchar *reverseB = (uchar*)malloc(imageSize);    
    uchar *reverse = (uchar*)malloc(3*imageSize);
    
    /* RESOLVIDO: o problema está na leitura do arquivo. Se usarmos o mesmo vetor
     acima, a imagem é revertida com sucesso. RESOLUÇÃO: a leitura e a escrita 
     deve ser feita em binário */
    fread(reverseR, sizeof(uchar), imageSize, fpR);
    fread(reverseG, sizeof(uchar), imageSize, fpG);
    fread(reverseB, sizeof(uchar), imageSize, fpB);
    fread(reverse, sizeof(uchar), 3*imageSize, fp);
    
    IplImage *Rchannel;
    CvSize size = cvSize(img->width, img->height);
    Rchannel = cvCreateImage(size, img->depth, img->nChannels);
    cvZero(Rchannel);
    
    //utilizando os 3 vetores
    count = 0;    
    for(int row = 0; row < Rchannel->height; row++){
        uchar *ptrImg = (uchar*) (Rchannel->imageData + row * Rchannel->widthStep);
        for(int col = 0; col < Rchannel->width; col++){
            ptrImg[3*col] = reverseB[count];
            ptrImg[3*col+1] = reverseG[count];
            ptrImg[3*col+2] = reverseR[count];
            count++;
        }
    } 
    
    cvSaveImage("ImagemReconvertida-3vetores.bmp", Rchannel);
    
    //utilizando apenas 1 vetor
    cvZero(Rchannel);
    
    count = 0;    
    for(int row = 0; row < Rchannel->height; row++){
        uchar *ptrImg = (uchar*) (Rchannel->imageData + row * Rchannel->widthStep);
        for(int col = 0; col < Rchannel->width; col++){
            ptrImg[3*col] = reverse[count+(2*imageSize)];
            ptrImg[3*col+1] = reverse[count+imageSize];
            ptrImg[3*col+2] = reverse[count];
            count++;
        }
    } 
    
    cvSaveImage("ImagemReconvertida-vetor-unico.bmp", Rchannel);
    
    fclose(fpR);
    fclose(fpG);
    fclose(fpB);
    fclose(fp);
    free(vetR);
    free(reverseR);
    free(vetG);
    free(reverseG);
    free(vetB);
    free(reverseB);
    free(vet);
    free(reverse);
    return 0;
}
