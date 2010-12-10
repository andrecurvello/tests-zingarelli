#include "highgui.h"

int main(int argc, char** argv){
    //load an image that comes from the arguments within command line
    //Create a pointer to that image
    IplImage* img = cvLoadImage(argv[1]);
    
    //Create a window to show images. Second argument enables the window to be
    //the same size of the image
    cvNamedWindow("Example1", CV_WINDOW_AUTOSIZE);
    
    //Attach the loaded image to the window
    cvShowImage("Example1", img);
    
    //Wait until user press some key
    cvWaitKey(0);
    
    //Set the image pointer to null
    cvReleaseImage(&img);
    
    //Close the window and deallocate any memory resource
    cvDestroyWindow("Example1");
}
