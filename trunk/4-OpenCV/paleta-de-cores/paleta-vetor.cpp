/*
  This program executes the conversion of a stereo pair of images into an anaglyph
  image as well as the reversion, from an anaglyph image to a stereo pair. The
  stereo pair can be of two types: side-by-side or above below. In the reversion
  process, an auxiliary vector is used, which contains channels that were disposed 
  in the anaglyph process.

  Developed by: Matheus Ricardo Uihara Zingarelli
  Creation date: April, 12th 2011
  Last modification: April, 12th 2011
  
  Usage:
        paleta-imagem <image.bmp> {O | 1} {-e | -d}
        
        0: side-by-side image
        1: above/below image
        -e: encoding
        -d: decoding
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
  Entries: frame - original image
           frameL, frameR - stores left and right image, respectively
           width, height - original image dimensions
           imageType - 0 if the original image has side-by-side images, 1 if it has above-below images                
*/
void separateImages(IplImage *frame, IplImage **frameL, IplImage **frameR, int width, int height, int imageType){    
    switch(imageType){
        case 0:
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
            break;
        case 1:
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
            break;
    }
}

/*
  Get an anaglyph image and create the stereo pair, using an auxiliary image to
  get channels disposed in the anaglyph process.
  Entries: frame - anaglyph image
           imageType - 0 to create a side-by-side image or 1 to create a above-below one           
*/
void revertAnaglyph(IplImage *frame, int imageType){
    IplImage *stereoPair;
    int width = frame->width;
    int height = frame->height;  
    int imageSize = width*height;    
    
    //new image will have double width or double height, depending on the image type
    CvSize size;
    switch(imageType){
        case 0: //side-by-side type
             size = cvSize( width*2, height);
             break;
        case 1: //above-below type
             size = cvSize( width, height*2);
             break;                
    }
    
    //copy image properties
    stereoPair = cvCreateImage(size, frame->depth, frame->nChannels);
    cvZero(stereoPair);
    
    //get data from disposed channels
    FILE *fp = fopen("pixelData.dat", "rb");       
    uchar *vet = (uchar*)malloc(3*imageSize);
    fread(vet, sizeof(uchar), 3*imageSize, fp);
    fclose(fp);
    int count=0;
    switch(imageType){
        case 0:
             for(int row = 0; row < frame->height; row++){
                 //set pointer to the correct position in each row
                 uchar* ptrAn = (uchar*)(frame->imageData + row * frame->widthStep);
                 uchar* ptrStp = (uchar*)(stereoPair->imageData + row * stereoPair->widthStep);                
                 for(int col = 0; col < frame->width; col++){
                     /* Copy values from frame (G and B channels) and vet (R 
                     channel) to stereoPair for the first half of image width */
                     ptrStp[3*col] = ptrAn[3*col];
                     ptrStp[3*col+1] = ptrAn[3*col+1];
                     ptrStp[3*col+2] = vet[count];
                     /* Copy values from frame (R channel) and vet (G and B 
                     channels) to stereoPair for the rest of image width */
                     ptrStp[3*(col+frame->width)] = vet[count+(2*imageSize)];
                     ptrStp[3*(col+frame->width)+1] = vet[count+imageSize];
                     ptrStp[3*(col+frame->width)+2] = ptrAn[3*col+2];
                     
                     count++;
                 }                  
             }             
             break;
        case 1:
             for(int row = 0; row < frame->height; row++){
                 //set pointer to the correct position in each row
                 uchar* ptrAn = (uchar*)(frame->imageData + row * frame->widthStep);
                 uchar* ptrStp = (uchar*)(stereoPair->imageData + row * stereoPair->widthStep);                
                 for(int col = 0; col < frame->width; col++){
                     /* Copy values from frame (G and B channels) and vet (R 
                     channel) to stereoPair for the first half of image height */
                     ptrStp[3*col] = ptrAn[3*col];
                     ptrStp[3*col+1] = ptrAn[3*col+1];
                     ptrStp[3*col+2] = vet[count];
                     
                     count++;
                 }
            }
            count = 0;
            for(int row = 0; row < frame->height; row++){
                 //set pointer to the correct position in each row
                 uchar* ptrAn = (uchar*)(frame->imageData + row * frame->widthStep);
                 uchar* ptrStp = (uchar*)(stereoPair->imageData + (row + frame->height) * stereoPair->widthStep);
                 for(int col = 0; col < frame->width; col++){
                     /* Copy values from frame (R channel) and vet (G and B 
                     channels) to stereoPair for the first half of image height */
                     ptrStp[3*col] = vet[count+(2*imageSize)];
                     ptrStp[3*col+1] = vet[count+imageSize];
                     ptrStp[3*col+2] = ptrAn[3*col+2];
                     
                     count++;
                 }
            }
            break;
     }
     cvSaveImage("Par_Estereo.bmp",stereoPair);
     printf("Reversao para par estereo feita com sucesso! - Par_Estereo.bmp\n");       
     cvReleaseImage(&stereoPair);
     free(vet);
}


/*
  Get the stereo pair and create the red-cyan anaglyph. Copy the removed channels
  to an auxiliary file to be reused in order to make the reverse process (from 
  anaglyph to stereo pair, without any losses).
  Entries: frameL, frameR - stores left and right image, respectively
*/
void createAnaglyph(IplImage *frameL, IplImage *frameR){
    IplImage *anaglyph; 
    /* The following vector will hold channels lost during anaglyph transformation */
    uchar *disposed_channels;
    
    //prepare anaglyph image
    CvSize size = cvGetSize(frameL);
    anaglyph = cvCreateImage(size, frameL->depth, frameL->nChannels);
    cvZero(anaglyph);
    
    //prepare vector
    int imageSize = frameL->width*frameL->height;
    disposed_channels = (uchar*)malloc(3*imageSize);
    
    //anaglyph transformation
    int count = 0;
    for(int row = 0; row < frameL->height; row++){
            //set pointer to the correct position in each row
            uchar* ptrR = (uchar*)(frameR->imageData + row * frameR->widthStep);
            uchar* ptrL = (uchar*)(frameL->imageData + row * frameL->widthStep);
            uchar* ptrA = (uchar*)(anaglyph->imageData + row * anaglyph->widthStep);
            for(int col = 0; col < frameL->width; col++){
                    //copy red channel from the right image and blue and green channel from the left image
                    ptrA[3*col+2] = ptrR[3*col+2];
                    ptrA[3*col+1] = ptrL[3*col+1];
                    ptrA[3*col] = ptrL[3*col];
                    //copy remain channels to their corresponding vectors
                    disposed_channels[count] = ptrL[3*col+2]; //red
                    disposed_channels[count+imageSize] = ptrR[3*col+1]; //green
                    disposed_channels[count+(2*imageSize)] = ptrR[3*col]; //blue
                    count++;
            }            
    }
    
    //save junctioned image
    cvSaveImage("anaglyph.bmp", anaglyph);
    
    cvReleaseImage(&anaglyph);
    
    //save vector
    FILE *fp;
    fp = fopen("pixelData.dat", "wb");
    
    if(!fp){
        printf("Erro na criacao de arquivo das paletas de cores. Favor verificar\n");
        exit(-1);
    }
    
    fwrite(disposed_channels, sizeof(uchar), 3*imageSize, fp);
    
    fclose(fp);
    free(disposed_channels);
    printf("\nImagem anaglifica criada com sucesso! - anaglyph.bmp\n");
}

/* Prints help message to user */
void printHelp(){
    printf("Chamada do programa: paleta-vetor <arquivo> {O | 1} {-e | -d}\n");
    printf("\n0:side-by-side ou 1:above/below");
    printf("\n-e:encode (transformacao anaglifica) ou -d:decode (reversao para par estereo)");
}

int main(int argc, char* argv[]){
    if(argc < 4){
        printHelp();
        exit(-1);  
    }
    
    IplImage *frame;
    int imageType = atoi(argv[2]); //0:sid-by-side or 1:above/below
    
    //load image
    //images are loaded by openCV with color channels in this order: BGR
    frame = cvLoadImage(argv[1], 1);  
    
    //Simple error handling
    if(!frame){
       printf("Erro ao abrir imagem.");
       exit(-1);
    }
    
    // ----- Anaglyph conversion
    if(!strcmp(argv[3],"-e")){    
        IplImage *frameL, *frameR;
            
        //some verifications regarding the image
        if(frame->width % 2 != 0){
              printf("Imagem possui largura n�o divis�vel por 2. Favor cortar!");
              exit(-1);
        }
            
        int width = frame->width;
        int height = frame->height;      
            
        //new images will have half width of original image
        CvSize size;
        switch(imageType){
            case 0:
                 size = cvSize( width/2, height);
                 break;
            case 1:
                 size = cvSize( width, height/2);
                 break;
            default:
                 printHelp();
                 exit(-1);                    
        }
        
        //copy image properties
        frameL = cvCreateImage(size, frame->depth, frame->nChannels);
        frameR = cvCreateImage(size, frame->depth, frame->nChannels);
        cvZero(frameL);
        cvZero(frameR);
        
        //divide frames in two
        separateImages(frame, &frameL, &frameR, width, height, imageType);
        
        
        //create anaglyph and auxiliary image
        createAnaglyph(frameL, frameR);      
        
        cvReleaseImage(&frameL);
        cvReleaseImage(&frameR);
    }
    // ----- Anaglyph reversion
    else if(!strcmp(argv[3],"-d")){
        revertAnaglyph(frame, imageType);
    }
    else{
        printHelp();
        exit(-1);      
    }
    
    cvReleaseImage(&frame);
    
    return 0;
}