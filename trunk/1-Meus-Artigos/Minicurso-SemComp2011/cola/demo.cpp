/*

SEMCOMP 14 - Minicurso Criação de imagens e vídeos 3D com OpenCV
19/09/2011
Matheus Ricardo Uihara Zingarelli (zinga@icmc.usp.br)

Programa demonstração para teste do OpenCV

*/

#include "highgui.h"
int main( int argc, char** argv ) {
    //abertura da imagem
    IplImage* img = cvLoadImage( "semcomp.jpg", 1 );

    //criação de janela
    cvNamedWindow( "Teste OpenCV", CV_WINDOW_AUTOSIZE );
    cvShowImage( "Teste OpenCV", img );

    //aguarda ação do usuário antes de fechar a janela
    cvWaitKey(0);

    //liberação das estruturas (free dos ponteiros)
    cvReleaseImage( &img );
    cvDestroyWindow( "Teste OpenCV" );

    return 0;
}
