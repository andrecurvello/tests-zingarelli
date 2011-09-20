/*

SEMCOMP 14 - Minicurso Cria��o de imagens e v�deos 3D com OpenCV
19/09/2011
Matheus Ricardo Uihara Zingarelli (zinga@icmc.usp.br)

Programa demonstra��o para teste do OpenCV

*/

#include "highgui.h"
int main( int argc, char** argv ) {
    //abertura da imagem
    IplImage* img = cvLoadImage( "semcomp.jpg", 1 );

    //cria��o de janela
    cvNamedWindow( "Teste OpenCV", CV_WINDOW_AUTOSIZE );
    cvShowImage( "Teste OpenCV", img );

    //aguarda a��o do usu�rio antes de fechar a janela
    cvWaitKey(0);

    //libera��o das estruturas (free dos ponteiros)
    cvReleaseImage( &img );
    cvDestroyWindow( "Teste OpenCV" );

    return 0;
}
