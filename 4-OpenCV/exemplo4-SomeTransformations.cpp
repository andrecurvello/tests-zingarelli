#include "cv.h"
#include "highgui.h"

//Create a new image that is half of the original one
IplImage* doPyrDown(IplImage* in, int filter = IPL_GAUSSIAN_5x5){
          //Best to make sure input image is divisible by two
          assert(in->width%2 == 0 && in->height%2 == 0);
          
          IplImage* out = cvCreateImage(
                    cvSize(in->width/2, in->height/2),
                    in->depth,
                    in->nChannels
          );
          
          cvPyrDown(in, out);
          
          return(out);
}

//Canny edge detector
IplImage* doCanny(IplImage* in, double lowThresh, double highThresh, int aperture){
          if(in->nChannels != 1) return(0); //Canny only handles grayscale images
          
          IplImage* out = cvCreateImage(
                    cvGetSize(in),
                    IPL_DEPTH_8U,
                    1
          );
          
          cvCanny(in, out, lowThresh, highThresh, aperture);
          
          return(out);
}

int main(int argc, char** argv){
    //load an image that comes from the arguments within command line
    //Create a pointer to that image
    IplImage* img = cvLoadImage(argv[1]);
    
    //call the functions to applay transformation
    IplImage* reduced = doPyrDown(img);
    reduced = doPyrDown(reduced, IPL_GAUSSIAN_5x5);
    IplImage* canny = doCanny(reduced, 10, 100, 3);
    
    cvNamedWindow("PyrDown", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Canny", CV_WINDOW_AUTOSIZE);
        
    //Attach the loaded image to the window
    cvShowImage("PyrDown", reduced);
    cvShowImage("Canny", canny);
    
    //Wait until user press some key
    cvWaitKey(0);
    
    //Release everything
    cvReleaseImage(&img);
    cvReleaseImage(&reduced);
    cvDestroyWindow("PyrDown");
    cvDestroyWindow("Canny");
}
