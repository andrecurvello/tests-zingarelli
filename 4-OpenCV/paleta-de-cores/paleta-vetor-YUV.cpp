/*
  This program executes the conversion of a stereo pair of images into an anaglyph
  image as well as the reversion, from an anaglyph image to a stereo pair. The
  stereo pair can be of two types: side-by-side or above below. In the reversion
  process, an auxiliary vector is used, which contains channels that were disposed 
  in the anaglyph process. The anaglyph image is converted to YUV color space with
  4:2:2 subsampling, reducing the amount of information. We generate a gree/magenta
  anaglyph image

  Developed by: Matheus Ricardo Uihara Zingarelli
  Creation date: April, 30th 2011
  Last modification: April, 30th 2011
  
  Usage:
        paleta-imagem <image.bmp> {-sbs | -ab} {-e | -d}
        
        -sbs: side-by-side image
        -ab: above/below image
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
           imageType - -sbs if the original image has side-by-side images, -ab
            if it has above-below images                
*/
void separateImages(IplImage *frame, IplImage **frameL, IplImage **frameR, int width, int height, char *imageType){    
    if(!strcmp(imageType,"-sbs")){
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
    else if(!strcmp(imageType, "-ab")){
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
}

/*
  Get an anaglyph image and create the stereo pair, using an auxiliary image to
  get channels disposed in the anaglyph process.
  Entries: frame - anaglyph image
           imageType - -sbs to create a side-by-side image or -ab to create a above-below one           
*/
void revertAnaglyph(IplImage *frame, char *imageType){
    IplImage *stereoPair;
    int width = frame->width;
    int height = frame->height;  
    int imageSize = width*height;    
    
    //new image will have double width or double height, depending on the image type
    CvSize size;
    if(!strcmp(imageType,"-sbs")){
        size = cvSize( width*2, height);
    }
    else if(!strcmp(imageType, "-ab")){
        size = cvSize( width, height*2);
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
    if(!strcmp(imageType, "-sbs")){    
        for(int row = 0; row < frame->height; row++){
            //set pointer to the correct position in each row
            uchar* ptrAn = (uchar*)(frame->imageData + row * frame->widthStep);
            uchar* ptrStp = (uchar*)(stereoPair->imageData + row * stereoPair->widthStep);                
            for(int col = 0; col < frame->width; col++){
                /* Copy values from frame (R and B channels) and vet (G 
                channel) to stereoPair for the first half of image width */
                ptrStp[3*col] = ptrAn[3*col];
                ptrStp[3*col+2] = ptrAn[3*col+2];
                ptrStp[3*col+1] = vet[count];
                /* Copy values from frame (G channel) and vet (R and B 
                channels) to stereoPair for the rest of image width */
                ptrStp[3*(col+frame->width)] = vet[count+(2*imageSize)];
                ptrStp[3*(col+frame->width)+2] = vet[count+imageSize];
                ptrStp[3*(col+frame->width)+1] = ptrAn[3*col+1];
                
                count++;
            }
        }
    }
    else if(!strcmp(imageType, "-ab")){
        for(int row = 0; row < frame->height; row++){
             //set pointer to the correct position in each row
             uchar* ptrAn = (uchar*)(frame->imageData + row * frame->widthStep);
             uchar* ptrStp = (uchar*)(stereoPair->imageData + row * stereoPair->widthStep);                
             for(int col = 0; col < frame->width; col++){
                 /* Copy values from frame (R and B channels) and vet (G 
                 channel) to stereoPair for the first half of image height */
                 ptrStp[3*col] = ptrAn[3*col];
                 ptrStp[3*col+1] = vet[count];
                 ptrStp[3*col+2] = ptrAn[3*col+2];
                 
                 count++;
             }
        }
        count = 0;
        for(int row = 0; row < frame->height; row++){
             //set pointer to the correct position in each row
             uchar* ptrAn = (uchar*)(frame->imageData + row * frame->widthStep);
             uchar* ptrStp = (uchar*)(stereoPair->imageData + (row + frame->height) * stereoPair->widthStep);
             for(int col = 0; col < frame->width; col++){
                 /* Copy values from frame (G channel) and vet (R and B 
                 channels) to stereoPair for the rest of image height */
                 ptrStp[3*col] = vet[count+(2*imageSize)]; //blue
                 ptrStp[3*col+2] = vet[count+imageSize]; //red
                 ptrStp[3*col+1] = ptrAn[3*col+1];
                 
                 count++;
             }
        }
     }
     cvSaveImage("Par_Estereo.bmp",stereoPair);
     printf("Reversao para par estereo feita com sucesso! - Par_Estereo.bmp\n");       
     cvReleaseImage(&stereoPair);
     free(vet);
}


/*
  Get the stereo pair and create the green-magenta anaglyph. Copy the removed channels
  to an auxiliary file to be reused in order to make the reverse process (from 
  anaglyph to stereo pair, without any losses).
  Entries: frameL, frameR - stores left and right image, respectively
*/
void createAnaglyph(IplImage *frameL, IplImage *frameR){
    IplImage *anaglyph, *aux; 
    /* The following vector will hold channels lost during anaglyph transformation */
    uchar *disposed_channels;
    
    //prepare anaglyph image
    CvSize size = cvGetSize(frameL);
    anaglyph = cvCreateImage(size, frameL->depth, frameL->nChannels);
    aux = cvCreateImage(size, frameL->depth, frameL->nChannels);
    cvZero(anaglyph);
    cvZero(aux);
    
    //prepare vector
    int imageSize = frameL->width*frameL->height;
    disposed_channels = (uchar*)malloc(3*imageSize);
    
    //anaglyph transformation
    int count = 0;
    for(int row = 0; row < frameL->height; row++){
            //set pointer to the correct position in each row
            uchar* ptrR = (uchar*)(frameR->imageData + row * frameR->widthStep);
            uchar* ptrL = (uchar*)(frameL->imageData + row * frameL->widthStep);
            uchar* ptrA = (uchar*)(aux->imageData + row * aux->widthStep);
            for(int col = 0; col < frameL->width; col++){
                    //copy green channel from the right image and red and blue channel from the left image
                    ptrA[3*col+1] = ptrR[3*col+1];
                    ptrA[3*col] = ptrL[3*col];
                    ptrA[3*col+2] = ptrL[3*col+2];
                    //copy remain channels to their corresponding vectors
                    disposed_channels[count] = ptrL[3*col+1]; //green
                    disposed_channels[count+imageSize] = ptrR[3*col+2]; //red
                    disposed_channels[count+(2*imageSize)] = ptrR[3*col]; //blue
                    count++;
            }            
    }
    
    //RGB to YUV conversion
    for(int row = 0; row < frameL->height; row++){
        //set pointer to the correct position in each row
        float* ptrAn = (float*)(anaglyph->imageData + row * anaglyph->widthStep);
        uchar* ptrAux = (uchar*)(aux->imageData + row * aux->widthStep);
        for(int col = 0; col < frameL->width; col++){
            /* RGB TO YUV 
               Y = 0.299f * R + 0.587 * G + 0.114 * B
               U = 0.565f * B - Y;
               V = 0.713f * R - Y;
            */
            /*TODO: resolver essa conversão*/
            float Y = 0.299f * ptrAux[3*col+2] + 0.587f * ptrAux[3*col+2] + 0.114f * ptrAux[3*col];
            ptrAn[3*col] = Y; //Y
            ptrAn[3*col+1] = 0.565f * ptrAux[3*col] - Y; //U
            ptrAn[3*col+2] = 0.713f * ptrAux[3*col+2] - Y; //V
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
    printf("Chamada do programa: paleta-vetor <arquivo> {-sbs | -ab} {-e | -d}\n");
    printf("\n-sbs:side-by-side ou -ab:above/below");
    printf("\n-e:encode (transformacao anaglifica) ou -d:decode (reversao para par estereo)");
}

int main(int argc, char* argv[]){
    if(argc < 4){
        printHelp();
        exit(-1);  
    }
    
    IplImage *frame;
    char *imageType = argv[2]; //-sbs:sid-by-side or -ab:above/below
    
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
              printf("Imagem possui largura não divisível por 2. Favor cortar!");
              exit(-1);
        }
            
        int width = frame->width;
        int height = frame->height;      
            
        //new images will have half width of original image
        CvSize size;
        if(!strcmp(imageType, "-sbs")){
            size = cvSize( width/2, height);
        }
        else if(!strcmp(imageType, "-ab")){
            size = cvSize( width, height/2);
        }
        else{
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
