/**
 * TUTORIAL OPENCV
 * EXEMPLO 2: Converter a imagem de entrada para tons de cinza.
 *
 * @author   MALF - malf@tecgraf.puc-rio.br
 * @date     29/06/07
 */

#include <cv.h>
#include <cvaux.h>
#include <highgui.h>
#include <stdio.h>
#include <math.h>

/** 
 Main
*/
int main( int argc, char** argv )
{
  
  /* Carregar imagem de entrada */
  IplImage* img_color = cvLoadImage( "..\\imagens\\tecgraf.jpg" );

  /* Criar as imagens para receber as conversões */
  IplImage* img_gray = cvCreateImage( cvGetSize( img_color ), IPL_DEPTH_8U, 1 );

  /* Converter a imagem original para tons de cinza */
  cvCvtColor( img_color, img_gray, CV_BGR2GRAY );

  /* Criar janelas para exibir as imagens */
  cvNamedWindow( "img_color_wnd", 0 );
  cvNamedWindow( "img_gray_wnd", 0 );

  /* Exibindo a imagem na janela criada */
  cvShowImage( "img_color_wnd", img_color );
  cvShowImage( "img_gray_wnd", img_gray );

  /* Manter o programa ativo enquanto nenhuma tecla for pressionada */
  int keyCode = cvWaitKey( );

  /* Desalocar imagens */
  cvReleaseImage( &img_color );
  cvReleaseImage( &img_gray );

  /* Desalocar janelas*/
  cvDestroyWindow( "img_color_wnd" );
  cvDestroyWindow( "img_gray_wnd" );

  /*
    //////////////////////////////////////////////////////////////////////////
    Observação: Toda essa conversão não é necessária caso não seja desejado armazenar a informação
                original de entrada colorida. Então seria necessário apenas um

                  IplImage* img = cvLoadImage( "tecgraf.jpg", 0 );

                A flag "0" faz o trabalho de converter a imagem colorida de entrada para tons de 
                cinza.
    //////////////////////////////////////////////////////////////////////////
  */
  return 0;
}
