#include "cv.h"
#include "highgui.h"

//store the slider position within the video player
int g_slider_position = 0;
CvCapture* g_capture = NULL;

//Callback routine to update the video according to the slider movement
void onTrackbarSlide(int pos){
     cvSetCaptureProperty(g_capture, CV_CAP_PROP_POS_FRAMES, pos);     
}

int main(int argc, char** argv){
    cvNamedWindow("Example2", CV_WINDOW_AUTOSIZE);
    //Get an AVI file and set a pointer to the beginning of it
    g_capture = cvCreateFileCapture(argv[1]);
    int frames = (int) cvGetCaptureProperty(g_capture, CV_CAP_PROP_FRAME_COUNT);
    if(frames != 0){
              //create and attach the trackbar to a window. Also set a callback routine
              cvCreateTrackbar(
                               "Position",
                               "Example2",
                               &g_slider_position,
                               frames,
                               onTrackbarSlide
              );              
    }
    IplImage* frame;
    while(1){
             //Grab the next frame from the AVI file and set a pointer to it
             frame = cvQueryFrame(g_capture);
             if(!frame) break;
             cvShowImage("Example2", frame);
             //Wait 33ms in case the user "stops" the video (ESC key)
             char c = cvWaitKey(33);
             if(c==27) break;             
    }
    //Free the pointer associated with the AVI file
    cvReleaseCapture(&g_capture);
    cvDestroyWindow("Example2");
    
    return(0);
}
