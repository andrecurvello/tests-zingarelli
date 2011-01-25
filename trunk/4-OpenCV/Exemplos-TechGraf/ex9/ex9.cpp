/**
* TUTORIAL OPENCV
* EXEMPLO 
*
* @author   MALF - malf@tecgraf.puc-rio.br
* @date     29/06/07
*/


#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <stdio.h>

#define HOUGH_STAND 1 // Standard
#define HOUGH_PROB  0 // Probabilistic


/** A function to draw outline of an image using Hough transform
* @param src - the source image (an 8-bit 1channel IplImage)
* @param dst - the destination image (an 8-bit 1 chaneel IplImage)
* @param houghMethod - 0 or 1 (0=Standard, 1=Probabilistic)
* @return 0 on success, -1 on failure
* 	See Documentation for cvHoughLines2
*/
int drawHoughLines(IplImage* src, IplImage* dst, int houghMethod) // houghMethod should be 1 or 0
{
  IplImage* color_dst;
  CvMemStorage* storage = cvCreateMemStorage(0);
  CvSeq* lines = 0;
  int i;

  if( !src || !dst)
    return -1;

  //dst = cvCreateImage( cvGetSize(src), 8, 1 );
  color_dst = cvCreateImage( cvGetSize(src), 8, 3 );

  cvCanny( src, dst, 50, 200, 3 );
  cvCvtColor( dst, color_dst, CV_GRAY2BGR );
  if(houghMethod)
  {
    //printf("CV_HOUGH_STANDARD\n");
    lines = cvHoughLines2( dst, storage, CV_HOUGH_STANDARD, 1, CV_PI/180, 100, 0, 0 );

    for( i = 0; i < MIN(lines->total,100); i++ )
    {
      float* line = (float*)cvGetSeqElem(lines,i);
      float rho = line[0];
      float theta = line[1];
      CvPoint pt1, pt2;
      double a = cos(theta), b = sin(theta);
      double x0 = a*rho, y0 = b*rho;
      pt1.x = cvRound(x0 + 1000*(-b));
      pt1.y = cvRound(y0 + 1000*(a));
      pt2.x = cvRound(x0 - 1000*(-b));
      pt2.y = cvRound(y0 - 1000*(a));
      cvLine( color_dst, pt1, pt2, CV_RGB(255,0,0), 3, CV_AA, 0 );
    }
  }
  else
  {
    //printf("CV_HOUGH_PROBABILISTIC\n");
    lines = cvHoughLines2( dst, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 50, 50, 10 );
    for( i = 0; i < lines->total; i++ )
    {
      CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
      cvLine( color_dst, line[0], line[1], CV_RGB(255,0,0), 3, CV_AA, 0 );
    }
  }// end else

  cvShowImage("color", color_dst);

  cvReleaseMemStorage(&storage);
  cvReleaseImage(&color_dst);
  return 0;
}

/** 
Main
*/
int main (int argc, char** argv) 
{
  const char* outputWindow = "Hough Lines"; // name of the window
  CvCapture* camera;	// camera is a video capture structure
  IplImage* frame;	// image frame captured from the camera
  IplImage* image;	// image used to perform hough algorithm
  IplImage* temp;		// image used to perform hough algorithm 

  int inputkey = 0;	// ascii code for an input key
  int width = 640;	// width of output window
  int height = 480;	// height of output window


  cvNamedWindow("color", CV_WINDOW_AUTOSIZE);

  //create a window
  if(!cvNamedWindow(outputWindow, CV_WINDOW_AUTOSIZE))
  {
    return -1;
  }

  camera = cvCreateCameraCapture(-1); // -1 for any camera

  while(1)
  {
    frame = cvQueryFrame(camera);
    //frame = cvLoadImage("carro.jpg",1);

    // Set Channel of Interest, then copy the image, and Show it.
    cvSetImageCOI(frame, 2); // channel 2
    temp =  cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1); // create the image
    image =  cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1); // create the image
    cvCopy(frame, temp, NULL); // copy the image  
    cvCopy(frame, image, NULL); // copy the image  

    //if(drawHoughLines(temp, image, HOUGH_STAND) < 0)
    if(drawHoughLines(temp, image, HOUGH_PROB) < 0)
    {
      printf("drawHoughLines crashed...");
      exit(-1);
    }

    // We can alter the output in various ways by just doing binary math
    // You can get different results with cvSub, cvMul, cvOr, cvAnd, etc...
    // cvNot(image, image); // black lines, white background

    // Create some larger images
    temp = cvCreateImage(cvSize(width,height), IPL_DEPTH_8U, 1);
    // Resize output images (uses bilinear interpolation by default)
    cvResize(image, temp, 0); // resized image stored in temp

    cvShowImage(outputWindow, temp); // display the resized image

    // Release images we just created for resizing
    cvReleaseImage(&temp);
    cvReleaseImage(&image);

    inputkey = cvWaitKey(100);
    if(inputkey == 27) // 'ESC'
      break;  // break out of while
  }// end while

  cvReleaseCapture(&camera);
  cvDestroyWindow(outputWindow);
  cvDestroyWindow("color");	
  return 0;
}
