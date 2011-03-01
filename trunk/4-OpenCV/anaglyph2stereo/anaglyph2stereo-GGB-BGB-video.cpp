/*
  Given an anaglyph video, get video from the left (missing red channel) and 
  copy green or blue channel values to replace the missing red channel.

  Developed by: Matheus Ricardo Uihara Zingarelli
  Creation date: Mar, 1st. 2011
  Last modification: Mar, 1st. 2011
*/

//anaglyph2stereo-GGB-BGB-video <anaglyph_video> 0|1
//0: copy green values
//1: copy blue values

#include <stdio.h>
#include <cv.h>
#include <highgui.h>

int main(int argc, char* argv[]){    
    IplImage *frame, *frameL;
    CvCapture *capture = cvCreateFileCapture(argv[1]);
        
    //Simple error handling
    if(!capture){
       printf("Erro ao abrir o video.");
       exit(-1);
    }
    
    //some verifications regarding the video
    frame = cvQueryFrame(capture);
    if(!frame){
        printf("Video vazio.");
        exit(-1);
    }
    if(frame->width % 2 != 0){
          printf("Video possui largura não divisível por 2. Favor cortar!");
          exit(-1);
    }
    
    //prepare  video
    double fps = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
    CvSize videosize;
    videosize = cvSize(
                       (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH),
                       (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT)
                       );
            
    CvVideoWriter *writer = cvCreateVideoWriter("left-video.avi",CV_FOURCC('H','F','Y','U'), fps, videosize);    
    
    printf("\n\nSeparando os videos, aguarde...\n\n");
    
    //start working on each frame
    while((frame = cvQueryFrame(capture)) != NULL){ 
        //copy image properties
        CvSize size = cvSize(frame->width, frame->height);
        frameL = cvCreateImage(size, frame->depth, frame->nChannels);
        cvZero(frameL);
        
        //loop into anaglyp image
        for(int row = 0; row < frame->height; row++){
            //set pointer to the correct position in each row
            uchar* ptrA = (uchar*)(frame->imageData + row * frame->widthStep);
            uchar* ptrL = (uchar*)(frameL->imageData + row * frameL->widthStep);
            for(int col = 0; col < frame->width; col++){               
                //copy G and B channels to the left image of the pair
                ptrL[3*col] = ptrA[3*col];
                ptrL[3*col+1] = ptrA[3*col+1];
                if(atoi(argv[2])==0){
                    //copy G channel values to R channel (missing channel from anaglyph convertion)
                    ptrL[3*col+2] = ptrA[3*col+1];
                } else{
                    //copy B channel values to R channel (missing channel from anaglyph convertion)
                    ptrL[3*col+2] = ptrA[3*col];
                }
            }        
        }        
        
        //if any error occurr (f.e. segmentation fault, check if you have the codec installed)
        //Huffyuv codec (lossless): http://neuron2.net/www.math.berkeley.edu/benrg/huffyuv.html
        cvWriteFrame(writer, frameL);  
        
        cvReleaseImage(&frameL);
    }
    
    //free pointers 
    cvReleaseCapture(&capture);
    cvReleaseVideoWriter(&writer); 
    
    printf("\nSeparacao feita com sucesso!\n\n\n");
    
    return 0;
}
