#include "cv.h"
#include "highgui.h"

void example2_3(IplImage* image){
     //Windows to show input and output images
     cvNamedWindow("Example3-in");
     cvNamedWindow("Example3-out");
     
     //Attach image to input window
     cvShowImage("Example3-in", image);
     
     //Create an image to hold the smoothed output
     IplImage* out = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 3);
     
     //Do the smoothing (gaussian over a 3x3 area centered on each pixel
     cvSmooth(image, out, CV_GAUSSIAN, 3, 3);
     
     //Show the smoothed image in the output window
     cvShowImage("Example3-out", out);
     
     //Be tidy
     cvReleaseImage(&out);
     
     //Wait for user to hit a key, then clean up the windows
     cvWaitKey(0);
     cvDestroyWindow("Example3-in");
     cvDestroyWindow("Example3-out");
}

int main(int argc, char** argv){
    //load an image that comes from the arguments within command line
    //Create a pointer to that image
    IplImage* img = cvLoadImage(argv[1]);
    
    //call the function to applay gaussian transformation
    example2_3(img);
    
    //Set the image pointer to null
    cvReleaseImage(&img);
}
