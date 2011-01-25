//exemplo6-ImageROI <image> <x> <y> <width> <height> <add>
#include <cv.h>
#include <highgui.h>

int main(int argc, char** argv){
    IplImage *src;
    if(argc == 7 && (src = cvLoadImage(argv[1])) != 0){
            int x = atoi(argv[2]);
            int y = atoi(argv[3]);
            int width = atoi(argv[4]);
            int height = atoi(argv[5]);
            int add = atoi(argv[6]);
            
            printf("X: %d, Y: %d, width: %d, height: %d, add: %d \n", x, y, width, height, add);
            printf("Image width: %d, Image height: %d", src->width, src->height);
            
            //set the ROI (Region Of Interest) with the arguments passed via command line
            cvSetImageROI(src, cvRect(x, y, width, height));
            
            //apply the Add operation to work only on the ROI
            cvAddS(src, cvScalar(add), src);
            
            //Reset ROI to display the entire image on the window
            cvResetImageROI(src);
            
            cvNamedWindow("Roi_Add", 1);
            cvShowImage("Roi_Add", src);
            cvWaitKey();
            
            cvReleaseImage(&src);
            cvDestroyWindow("Roi_Add");
    }    
    return(0);
}

