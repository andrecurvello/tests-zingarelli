/*
  Given a side-by-side or above/below  video, split it into 2 videos and convert
  it to anaglyph (red-cyan).

  Developed by: Matheus Ricardo Uihara Zingarelli
  Creation date: Feb, 24nd. 2011
  Last modification: Feb, 24nd. 2011
*/

//SBS-AB-to-anaglyph-video <side-by-side_video> 0|1
//0: side-by-side video | 1: above/below video

#include <stdio.h>
#include <ctype.h>
#include <cv.h>
#include <highgui.h>

//separate images from a side-by-side or above/below image
void separateImages(IplImage *frame, IplImage **frameL, IplImage **frameR, int width, int height, int videoType){    
    switch(videoType){
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
        default:
             printf("Command call: SBS-UB-to-anaglyph-video <side-by-side_video> 0|1\n0:side-by-side and 1:above/below");
             exit(-1);
    }
}

//get the stereo pair and create the anaglyph
void createAnaglyph(IplImage *frameL, IplImage *frameR, IplImage **anaglyph){
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
    
    //junctions images
    cvAdd(frameR, frameL, *anaglyph);    
}

int main(int argc, char* argv[]){
    IplImage *frame, *frameL, *frameR, *anaglyph;
    CvCapture *capture = cvCreateFileCapture(argv[1]);
    int videoType = atoi(argv[2]); //0:sid-by-side or 1:above/below
    
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
    
    //prepare anaglyph video
    double fps = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
    CvSize videosize;
    switch(videoType){    
        case 0:
             videosize = cvSize(
                                      (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH)/2,
                                      (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT)
                                );
             break;
        case 1:
             videosize = cvSize(
                                      (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH),
                                      (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT)/2
                                );
             break;
        default:                
             printf("Command call: SBS-UB-to-anaglyph-video <side-by-side_video> 0|1\n0:side-by-side and 1:above/below");
             exit(-1);          
    }
    CvVideoWriter *writer = cvCreateVideoWriter("anaglyph.avi",CV_FOURCC('H','F','Y','U'), fps, videosize);
    
    //start working on each frame
    while((frame = cvQueryFrame(capture)) != NULL){ 
        //get width and height from original image
        int width = frame->width;
        int height = frame->height;      
        
        //new images will have half width of original image
        CvSize size;
        switch(videoType){
            case 0:
                 size = cvSize( width/2, height);
                 break;
            case 1:
                 size = cvSize( width, height/2);
                 break;
            default:
                 printf("Command call: SBS-UB-to-anaglyph-video <side-by-side_video> 0|1\n0:side-by-side and 1:above/below");
                 exit(-1);                    
        }
        
        //copy image properties
        frameL = cvCreateImage(size, frame->depth, frame->nChannels);
        frameR = cvCreateImage(size, frame->depth, frame->nChannels);
        cvZero(frameL);
        cvZero(frameR);
        
        //divide frames in two
        separateImages(frame, &frameL, &frameR, width, height, videoType);           
        
        anaglyph = cvCreateImage(size, frameL->depth, frameL->nChannels);
        cvZero(anaglyph);
        
        //create anaglyph
        createAnaglyph(frameL, frameR, &anaglyph);
        
        //if any error occurr (f.e. segmentation fault, check if you have the codec installed)
        //Huffyuv codec (lossless): http://neuron2.net/www.math.berkeley.edu/benrg/huffyuv.html
        cvWriteFrame(writer, anaglyph);  
    }
        
    //free pointers
    cvReleaseImage(&frameL);
    cvReleaseImage(&frameR);
    cvReleaseImage(&frameR);  
    cvReleaseCapture(&capture);
    cvReleaseVideoWriter(&writer);
    
    return 0;
}
