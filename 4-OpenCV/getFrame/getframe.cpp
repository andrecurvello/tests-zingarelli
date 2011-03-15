/*
  Given a video, get the first frame of it.

  Developed by: Matheus Ricardo Uihara Zingarelli
  Creation date: Feb, 24nd. 2011
  Last modification: Feb, 24nd. 2011
*/

//getframe <video.avi>

#include <stdio.h>
#include <ctype.h>
#include <cv.h>
#include <highgui.h>

int main(int argc, char* argv[]){
    IplImage *frame;
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
    
    cvSaveImage("frame.bmp", frame);
            
    //free pointers 
    cvReleaseCapture(&capture);
    
    return 0;
}
