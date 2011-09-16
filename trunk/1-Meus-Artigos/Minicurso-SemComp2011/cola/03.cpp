/*

SEMCOMP 14 - Minicurso Cria��o de imagens e v�deos 3D com OpenCV
Matheus Ricardo Uihara Zingarelli (zinga@icmc.usp.br)

Programa que recebe um arquivo de imagem como par�metro e o exibe na tela juntamente com sua vers�o no espa�o de cores YCbCr

C�digo adaptado do livro: Bradski, G; Kaehler, A. � Learning OpenCV: Computer Vision with the OpenCV Library. O�Reilly, 2008.

*/

#include "highgui.h"
#include "cv.h"
int main( int argc, char** argv ) {
    //abertura da imagem
    //1 - colorida
    //2 - grayscale
    IplImage* img = cvLoadImage( argv[1], 1 );

    //cria��o de janela
    cvNamedWindow( "Imagem Original", CV_WINDOW_AUTOSIZE );
    cvShowImage( "Imagem Original", img );

    //transforma��o em escala de cinza
    //cvCvtColor(img, img, CV_BGR2GRAY);
    //gera ERRO, pois n�o grayscale precisa de apenas um canal!

    //transforma��o YCbCr
    cvCvtColor(img, img, CV_BGR2YCrCb);
    cvNamedWindow( "Imagem YCbCr", CV_WINDOW_AUTOSIZE );
    cvShowImage( "Imagem YCbCr", img );

    //aguarda a��o do usu�rio antes de fechar a janela
    cvWaitKey(0);

    //libera��o das estruturas (free dos ponteiros)
    cvReleaseImage( &img );
    cvDestroyWindow( "Imagem Original" );
    cvDestroyWindow( "Imagem YCbCr" );

    return 0;
}
