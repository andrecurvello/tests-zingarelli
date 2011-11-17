/*
  Open an image and save a copy using OpenCV library

  Developed by: Matheus Ricardo Uihara Zingarelli
  Creation date: November, 17th 2011
  Last modification: November, 17th 2011

  Usage:
        openSaveOpenCV <image.bmp>
*/

#include <stdio.h>
#include <highgui.h>

int main(int argc, char* argv[]){
    IplImage *img;

    //load image
    img = cvLoadImage(argv[1],1);

    //save image
    cvSaveImage("new-image-openCv.bmp", img);

    cvReleaseImage(&img);
    printf("New image created!\n");
    return 0;
}
