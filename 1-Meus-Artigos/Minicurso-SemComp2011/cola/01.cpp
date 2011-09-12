/*

SEMCOMP 14 - Minicurso Cria��o de imagens e v�deos 3D com OpenCV
Matheus Ricardo Uihara Zingarelli (zinga@icmc.usp.br)

Programa que recebe um arquivo de imagem como par�metro e o exibe na tela

C�digo adaptado do livro: Bradski, G; Kaehler, A. � Learning OpenCV: Computer Vision with the OpenCV Library. O�Reilly, 2008.

*/

#include "highgui.h"
int main( int argc, char** argv ) {
    //abertura da imagem
    IplImage* img = cvLoadImage( argv[1], 1 );

    //cria��o de janela
    cvNamedWindow( "Example1", CV_WINDOW_AUTOSIZE );
    cvShowImage( "Example1", img );

    //aguarda a��o do usu�rio antes de fechar a janela
    cvWaitKey(0);

    //libera��o das estruturas (free dos ponteiros)
    cvReleaseImage( &img );
    cvDestroyWindow( "Example1" );

    return 0;
}
