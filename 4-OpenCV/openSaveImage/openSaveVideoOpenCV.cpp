/*
  Open a video and save first frame

  Developed by: Matheus Ricardo Uihara Zingarelli
  Creation date: November, 17th 2011
  Last modification: November, 17th 2011

  Usage:
        openSaveOpenCV <video.avi>
*/

#include <stdio.h>
#include <highgui.h>
#include <cv.h>

int main(int argc, char* argv[]){
    //load video
    CvCapture *capture = cvCreateFileCapture(argv[1]);

    //get video frame
    IplImage *frame = cvQueryFrame(capture);

    //save image
    cvSaveImage("new-frame-openCv.bmp", frame);

    cvReleaseCapture(&capture);
    printf("New image created!\n");
    return 0;
}
