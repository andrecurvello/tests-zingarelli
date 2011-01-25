//RGB_Split <original_image> <channel_R> <channel_G> <channel_B> <singleChannelR> <singleChannelG> <singleChannelB> 

#include <cv.h>
#include <highgui.h>

int main(int argc, char* argv[]){
    //open image
    IplImage* original;
       
    original = cvLoadImage(argv[1],1);
    
    //new images to be produced
    IplImage* imgR = cvCloneImage(original);
    cvCopy(original, imgR);
    IplImage* imgG = cvCloneImage(original);
    cvCopy(original, imgG);
    IplImage* imgB = cvCloneImage(original);
    cvCopy(original, imgB);
    
    IplImage *oneChannelR, *oneChannelG, *oneChannelB;
    CvSize size = cvGetSize(original);
    oneChannelR = cvCreateImage(size, IPL_DEPTH_8U, 1);
    oneChannelG = cvCreateImage(size, IPL_DEPTH_8U, 1);
    oneChannelB = cvCreateImage(size, IPL_DEPTH_8U, 1);
    
    //split up original image and copy each channel into a single channel image
    cvSplit(original, oneChannelB, oneChannelG, oneChannelR, NULL);
    
    //acess image data to create new images with only one of the three channels (R, G or B) active.
    for(int row = 0; row < original->height; row++){
            //points correctly to the next row
            uchar* ptrR = (uchar*)(imgR->imageData + row * imgR->widthStep);
            uchar* ptrG = (uchar*)(imgG->imageData + row * imgG->widthStep);
            uchar* ptrB = (uchar*)(imgB->imageData + row * imgB->widthStep);
            for(int col=0; col<original->width; col++){
                    //set G and B to zero
                    ptrR[3*col]=0;
                    ptrR[3*col+1]=0;                    
                    //set R and B to zero
                    ptrG[3*col]=0;
                    ptrG[3*col+2]=0;                    
                    //set R and G to zero
                    ptrB[3*col+1]=0;
                    ptrB[3*col+2]=0;                    
            }
    }
    
    //show images
    cvNamedWindow("Original Image",1);
    cvNamedWindow("Channel R",1);
    cvNamedWindow("Channel B",1);
    cvNamedWindow("Channel G",1);
    cvNamedWindow("Single Channel R",1);
    cvNamedWindow("Single Channel B",1);
    cvNamedWindow("Single Channel G",1);    

    cvShowImage("Original Image", original);
    cvShowImage("Channel R", imgR);
    cvShowImage("Channel G", imgG);
    cvShowImage("Channel B", imgB);
    cvShowImage("Single Channel R", oneChannelR);
    cvShowImage("Single Channel G", oneChannelG);
    cvShowImage("Single Channel B", oneChannelB);
    
    //Save images
    /*cvSaveImage(argv[2], imgR);
    cvSaveImage(argv[3], imgG);
    cvSaveImage(argv[4], imgB);
    cvSaveImage(argv[5], oneChannelR);
    cvSaveImage(argv[6], oneChannelG);
    cvSaveImage(argv[7], oneChannelB);*/
    
    cvWaitKey(0);
    
    //release pointers
    cvDestroyAllWindows();
    cvReleaseImage(&original);
    cvReleaseImage(&imgR);
    cvReleaseImage(&imgG);
    cvReleaseImage(&imgB);
    cvReleaseImage(&oneChannelR);
    cvReleaseImage(&oneChannelG);
    cvReleaseImage(&oneChannelB);
}
