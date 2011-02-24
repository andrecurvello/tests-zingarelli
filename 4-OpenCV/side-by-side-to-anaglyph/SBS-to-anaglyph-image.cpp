/*
  Given a side-by-side frame, split it into 2 images and convert it to anaglyph
  (red-cyan).

  Developed by: Matheus Ricardo Uihara Zingarelli
  Creation date: Feb, 24nd. 2011
  Last modification: Feb, 24nd. 2011
*/

//SBS-to-anaglyph-image <side-by-side_image>.bmp

#include <stdio.h>
#include <cv.h>
#include <highgui.h>

//get the stereo pair and create the anaglyph
void createAnaglyph(IplImage *frameL, IplImage *frameR){
    IplImage *anaglyph; 
            
    //remove channels
    for(int row = 0; row < frameL->height; row++){
            //set pointer to the correct position in each row
            uchar* ptrR = (uchar*)(frameR->imageData + row * frameR->widthStep);
            uchar* ptrL = (uchar*)(frameL->imageData + row * frameL->widthStep);
            
            for(int col = 0; col < frameL->width; col++){
                    //remove blue and green channel from the right image
                    ptrR[3*col] = 0;
                    ptrR[3*col+1] = 0;
                    //remove red channel from the left image
                    ptrL[3*col+2] = 0;
            }            
    }
           
    //prepare anaglyph image
    CvSize size = cvGetSize(frameL);
    anaglyph = cvCreateImage(size, frameL->depth, frameL->nChannels);
    cvZero(anaglyph);
    
    //junctions images
    cvAdd(frameR, frameL, anaglyph);
    
    //save junctioned image
    cvSaveImage("anaglyph.bmp", anaglyph);    
}

int main(int argc, char* argv[]){
    IplImage *frame, *frameL, *frameR;

    //load image
    //images are loaded by openCV with color channels in this order: BGR
    frame = cvLoadImage(argv[1], 1);  
    
    //Simple error handling
    if(!frame){
       printf("Erro ao abrir a imagem.");
       exit(-1);
    }
    
    //Verify if image have width divisible by 2
    if(frame->width % 2 != 0){
          printf("Imagem possui largura não divisível por 2. Favor cortar!");
          exit(-1);
    }
    
    //get width and height from original image
    int width = frame->width;
    int height = frame->height;
    
    
    //new images will have half width of original image
    CvSize size = cvSize( width/2, height);
    
    //copy image properties
    frameL = cvCreateImage(size, frame->depth, frame->nChannels);
    frameR = cvCreateImage(size, frame->depth, frame->nChannels);
    cvZero(frameL);
    cvZero(frameR);
    
    //set ROI to cut the image on the left (half of the original)
    cvSetImageROI(frame, cvRect(0,0, width/2, height));
    
    //copy image portion
    cvCopy(frame, frameL);
    
    //reset ROI
    cvResetImageROI(frame);
    
    //set ROI to cut the image on the right (second half of the original)
    cvSetImageROI(frame, cvRect(width/2, 0, width/2, height));
    
    //copy image portion
    cvCopy(frame, frameR);
    
    //reset ROI
    cvResetImageROI(frame);
    
    //save images
    cvSaveImage("frameLeft.bmp", frameL);
    cvSaveImage("frameRight.bmp", frameR);
    
    //create anaglyph
    createAnaglyph(frameL, frameR);
    
    //free pointers
    cvReleaseImage(&frame);
    cvReleaseImage(&frameL);
    cvReleaseImage(&frameR);
    
    printf("\n\nImagens separadas com sucesso!\n\n");
    
    return 0;
}
