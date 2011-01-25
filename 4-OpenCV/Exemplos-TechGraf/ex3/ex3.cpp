/**
* TUTORIAL OPENCV
* EXEMPLO 3: Trabalhar com diferentes espaços de cor
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
  IplImage* img_color = cvLoadImage( "..\\imagens\\carro.jpg" );

  /* Criar as imagens para receber as conversões */
  IplImage* img_hsv = cvCreateImage( cvGetSize( img_color ), IPL_DEPTH_8U, 3 );

  /* Converter a imagem original para o espaço de cores HSV */
  /* Outros espaços possíveis: RGB, BGR, GRAY, YCrCb, XYZ, Lab, Luv, HLS */
  cvCvtColor( img_color, img_hsv, CV_BGR2HSV );

  /* Criando imagens independentes para os canais h, s e v */
  IplImage* img_h = cvCreateImage( cvGetSize( img_hsv ), IPL_DEPTH_8U, 1 ); //Espaço de 0 a 180
  IplImage* img_s = cvCreateImage( cvGetSize( img_hsv ), IPL_DEPTH_8U, 1 ); //Espaço de 0 a 255
  IplImage* img_v = cvCreateImage( cvGetSize( img_hsv ), IPL_DEPTH_8U, 1 ); //Espaço de 0 a 255
  cvSplit( img_hsv, img_h, img_s, img_v, 0 );

  /* Criar janelas para exibir as imagens */
  cvNamedWindow( "img_color_wnd", 0 );
  cvNamedWindow( "img_h_wnd", 0 );
  cvNamedWindow( "img_s_wnd", 0 );
  cvNamedWindow( "img_v_wnd", 0 );

  /* Exibindo a imagem na janela criada */
  cvShowImage( "img_color_wnd", img_color );
  cvShowImage( "img_h_wnd", img_h );
  cvShowImage( "img_s_wnd", img_s );
  cvShowImage( "img_v_wnd", img_v );

  /* Manter o programa ativo enquanto nenhuma tecla for pressionada */
  int keyCode = cvWaitKey( );

  /* Desalocar imagens */
  cvReleaseImage( &img_color );
  cvReleaseImage( &img_hsv );
  cvReleaseImage( &img_h );
  cvReleaseImage( &img_s);
  cvReleaseImage( &img_v );

  /* Desalocar janelas*/
  cvDestroyWindow( "img_color_wnd" );
  cvDestroyWindow( "img_h_wnd" );
  cvDestroyWindow( "img_s_wnd" );
  cvDestroyWindow( "img_v_wnd" );

  return 0;
}
