/**
* TUTORIAL OPENCV
* EXEMPLO 5: Exibir um vídeo à partir de uma câmera ou arquivo
*
* @author   MALF - malf@tecgraf.puc-rio.br
* @date     29/06/07
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "cvcam.h"
#include "cv.h"
#include "highgui.h"

/************************************************************************
 * 
 ************************************************************************/
int main( int argc, char** argv )
{
  IplImage* frame = 0;
  CvCapture* capture = 0;
   
  capture = cvCaptureFromCAM(1);
  //capture = cvCaptureFromAVI( "..\\videos\\PetsD1TeC1.avi" ); 

  frame = cvQueryFrame( capture );

  if( !capture )
  {
    fprintf(stderr,"Could not initialize capturing...\n");
    return -1;
  }

  cvNamedWindow( "in", 0 );

  for(;;)
  {
    if(!cvGrabFrame(capture)) 
      break;

    frame = cvRetrieveFrame(capture);

    cvShowImage("in", frame );

    if( cvWaitKey(10) >= 0 )
      break;
  }

  cvReleaseCapture( &capture );
  cvDestroyWindow("in");

  return 0;
}

