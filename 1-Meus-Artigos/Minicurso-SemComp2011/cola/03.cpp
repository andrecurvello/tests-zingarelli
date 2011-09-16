/*

SEMCOMP 14 - Minicurso Criação de imagens e vídeos 3D com OpenCV
Matheus Ricardo Uihara Zingarelli (zinga@icmc.usp.br)

Programa que recebe um arquivo de imagem como parâmetro e o exibe na tela juntamente com sua versão no espaço de cores YCbCr

Código adaptado do livro: Bradski, G; Kaehler, A. – Learning OpenCV: Computer Vision with the OpenCV Library. O’Reilly, 2008.

*/

#include "highgui.h"
#include "cv.h"
int main( int argc, char** argv ) {
    //abertura da imagem
    //1 - colorida
    //2 - grayscale
    IplImage* img = cvLoadImage( argv[1], 1 );

    //criação de janela
    cvNamedWindow( "Imagem Original", CV_WINDOW_AUTOSIZE );
    cvShowImage( "Imagem Original", img );

    //transformação em escala de cinza
    //cvCvtColor(img, img, CV_BGR2GRAY);
    //gera ERRO, pois não grayscale precisa de apenas um canal!

    //transformação YCbCr
    cvCvtColor(img, img, CV_BGR2YCrCb);
    cvNamedWindow( "Imagem YCbCr", CV_WINDOW_AUTOSIZE );
    cvShowImage( "Imagem YCbCr", img );

    //aguarda ação do usuário antes de fechar a janela
    cvWaitKey(0);

    //liberação das estruturas (free dos ponteiros)
    cvReleaseImage( &img );
    cvDestroyWindow( "Imagem Original" );
    cvDestroyWindow( "Imagem YCbCr" );

    return 0;
}
