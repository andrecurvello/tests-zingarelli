/**
 * TUTORIAL OPENCV
 * EXEMPLO 4: 
 *
 * @author   MALF - malf@tecgraf.puc-rio.br
 * @date     29/06/07
 */

#include <cv.h>
#include <cvaux.h>
#include <highgui.h>
#include <stdio.h>
#include <math.h>

int threshold = 128;

/** 
 Main
 */
int main( int argc, char** argv )
{

  /* Carregar imagem de entrada */
  IplImage* img = cvLoadImage( "..\\imagens\\tecgraf.jpg" );

  /* Criar uma janela para exibir a imagem */
  cvNamedWindow( "img_wnd", 0 );


  cvCreateTrackbar( "Threshold", "img_wnd", &threshold, 255, NULL );

  for(;;)
  {
    for( int h = 0; h < img->height; h++ )
    {   
      for( int w = 0; w < img->width; w++ )
      {
        ((uchar *)(img->imageData + h*img->widthStep))[w*img->nChannels + 0] = threshold; 
//      ((uchar *)(img->imageData + h*img->widthStep))[w*img->nChannels + 1] = 0; 
//      ((uchar *)(img->imageData + h*img->widthStep))[w*img->nChannels + 2] = 0; 
      }
    }

    cvShowImage("img_wnd", img );

    if( cvWaitKey(10) >= 0 )
      break;
  }

  /* Desalocar imagem */
  cvReleaseImage( &img );

  /* Desalocar janela*/
  cvDestroyWindow( "img_wnd" );

  return 0;
}
