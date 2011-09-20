/*

SEMCOMP 14 - Minicurso Criação de imagens e vídeos 3D com OpenCV
Matheus Ricardo Uihara Zingarelli (zinga@icmc.usp.br)
19/09/2011

Programa que realiza a leitura de dados de uma imagem RGB e imprime os dados na tela

Código adaptado do livro: Bradski, G; Kaehler, A. – Learning OpenCV: Computer Vision with the OpenCV Library. O’Reilly, 2008.

*/

#include <stdio.h>
#include "highgui.h"
int main( int argc, char** argv ) {
    //abertura da imagem
    //1 - colorida
    //2 - grayscale
    IplImage* img = cvLoadImage( argv[1], 1 );

    //criação de janela
    /*cvNamedWindow( "Example1", CV_WINDOW_AUTOSIZE );
    cvShowImage( "Example1", img );*/

    //leitura dos dados da imagem
    for( int row = 0; row < img->height; row++ ){
        uchar* ptr = (uchar*) ( img->imageData + row * img->widthStep );
        for( int col = 0; col < img->width; col++ ) {
            printf( "%d ",  ptr[3*col] ); //acessa componente azul B
            printf( "%d ",  ptr[3*col+1] ); //acessa componente verde G
            printf( "%d\n", ptr[3*col+2] ); //acessa componente vermelha R
        }
    }

    //aguarda ação do usuário antes de fechar a janela
    //cvWaitKey(0);

    //liberação das estruturas (free dos ponteiros)
    cvReleaseImage( &img );
    //cvDestroyWindow( "Example1" );

    return 0;
}
