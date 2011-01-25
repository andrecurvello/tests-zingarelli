/**
 * TUTORIAL OPENCV
 * EXEMPLO 1: Manipulando imagens
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
  IplImage* img = cvLoadImage( "..\\imagens\\tecgraf.jpg" );

  /* Criar uma janela para exibir a imagem */
  cvNamedWindow( "img_wnd", 0 );

  /* Exibindo a imagem na janela criada */
  cvShowImage( "img_wnd", img );

  /* Redimensionar a janela */
  cvResizeWindow( "img_wnd", 320, 240 );

  /* Mover a janela para o canto superior esquedo coordenada(0,0) da tela */
  cvMoveWindow( "img_wnd", 0, 0 );

  /* Manter o programa ativo enquanto nenhuma tecla for pressionada */
  int keyCode = cvWaitKey( );

  /* Desalocar imagem */
  cvReleaseImage( &img );

  /* Desalocar janela*/
  cvDestroyWindow( "img_wnd" );

  return 0;
}
