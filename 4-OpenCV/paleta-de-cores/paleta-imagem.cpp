/*
  This program executes the conversion of a stereo pair of images into an anaglyph
  image as well as the reversion, from an anaglyph image to a stereo pair. The
  stereo pair can be of two types: side-by-side or above below. In the reversion
  process, an auxiliary image is used, which contains channels that were disposed 
  in the anaglyph process.

  Developed by: Matheus Ricardo Uihara Zingarelli
  Creation date: April, 6th 2011
  Last modification: April, 6th 2011
  
  Usage:
        paleta-imagem <image.bmp> {O | 1} {-e | -d} <auxiliary_image.bmp>
        
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
           stereoPair - stereoPair to be formed
           aux - auxiliary image
           imageType - 0 to create a side-by-side image or 1 to create a above-below one           
*/
void revertAnaglyph(IplImage *frame, IplImage **stereoPair, IplImage *aux, int imageType){
     //TODO
     /*
       Case imageType = side-by-side     
       Copy values from frame (G and B channels) and aux (R channel)to stereoPair
       until half of total width. For the other half, copy R channel from frame and
       G and B channels from aux
     */
     
     /*
       Case imageType = above-below
       Copy values from frame (G and B channels) and aux (R channel)to stereoPair
       until half of total height. For the other half, copy R channel from frame and
       G and B channels from aux
     */
}


/*
  Get the stereo pair and create the red-cyan anaglyph. Copy the removed channels
  to an auxiliary file to be reused in order to make the reverse process (from 
  anaglyph to stereo pair, without any losses).
  Entries: frameL, frameR - stores left and right image, respectively
*/
void createAnaglyph(IplImage *frameL, IplImage *frameR){
    IplImage *anaglyph, *aux; 

    //prepare anaglyph image
    CvSize size = cvGetSize(frameL);
    anaglyph = cvCreateImage(size, frameL->depth, frameL->nChannels);
    cvZero(anaglyph);
    
    //prepare auxiliary image (holds removed channels)
    aux = cvCreateImage(size, frameL->depth, frameL->nChannels);
    cvZero(aux);
    
    //remove channels
    for(int row = 0; row < frameL->height; row++){
            //set pointer to the correct position in each row
            uchar* ptrR = (uchar*)(frameR->imageData + row * frameR->widthStep);
            uchar* ptrL = (uchar*)(frameL->imageData + row * frameL->widthStep);
            uchar* ptrA = (uchar*)(anaglyph->imageData + row * anaglyph->widthStep);
            uchar* ptrAux = (uchar*)(aux->imageData + row * aux->widthStep);
            
            for(int col = 0; col < frameL->width; col++){
                    //copy red channel from the right image and blue and green channel from the left image
                    ptrA[3*col+2] = ptrR[3*col+2];
                    ptrA[3*col+1] = ptrL[3*col+1];
                    ptrA[3*col] = ptrL[3*col];
                    //copy remain channels into aux
                    ptrAux[3*col] = ptrR[3*col];
                    ptrAux[3*col+1] = ptrR[3*col+1];
                    ptrAux[3*col+2] = ptrL[3*col+2];
            }            
    }
    
    //save junctioned image
    cvSaveImage("anaglyph.bmp", anaglyph);
    cvSaveImage("auxiliary.bmp", aux);
    
    cvReleaseImage(&anaglyph);
    cvReleaseImage(&aux);
    
    printf("\nAnaglyph created!\n\n");
}

int main(int argc, char* argv[]){
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
                 printf("Command call: paleta-imagem <arquivo> {O | 1} {-e | -d}\n");
                 printf("\n0:side-by-side or 1:above/below");
                 printf("\n-e:encode (transform to anaglyph) or -d:decode (transform to stereo image)");
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
        IplImage *stereoPair, *aux;
        
        //get auxiliary image
        aux = cvLoadImage(argv[4], 1);
        if(!aux){
           printf("Erro ao abrir imagem.");
           exit(-1);
        }
        
        int width = frame->width;
        int height = frame->height;      
        
        //new image will have double width or double height, depending on the image type
        CvSize size;
        switch(imageType){
            case 0: //side-by-side type
                 size = cvSize( width*2, height);
                 break;
            case 1: //above-below type
                 size = cvSize( width, height*2);
                 break;
            default:
                 printf("Command call: paleta-imagem <arquivo> {O | 1} {-e | -d}\n");
                 printf("\n0:side-by-side or 1:above/below");
                 printf("\n-e:encode (transform to anaglyph) or -d:decode (transform to stereo image)");
                 exit(-1);                    
        }
        
        //copy image properties
        stereoPair = cvCreateImage(size, frame->depth, frame->nChannels);
        cvZero(stereoPair);
        
        //revert anaglyph image to stereo pair
        revertAnaglyph(frame, &stereoPair, aux, imageType);
        
        cvReleaseImage(&stereoPair);
        cvReleaseImage(&aux);
    }
    else{
        printf("\n\nCommand call: paleta-imagem <arquivo> {O | 1} {-e | -d}\n");
        printf("\n0:side-by-side or 1:above/below");
        printf("\n-e:encode (transform to anaglyph) or -d:decode (transform to stereo image)\n\n");
        exit(-1);      
    }
    
    cvReleaseImage(&frame);
    
    return 0;
}