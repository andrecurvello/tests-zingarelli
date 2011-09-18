/*

SEMCOMP 14 - Minicurso Cria��o de imagens e v�deos 3D com OpenCV
Matheus Ricardo Uihara Zingarelli (zinga@icmc.usp.br)

Programa que recebe um arquivo de v�deo e exibe o primeiro frame deste em uma janela

*/

#include "highgui.h"
#include "cv.h"
#include <stdio.h>

int main(int argc, char** argv){
    //abrir v�deo
    CvCapture* video = cvCreateFileCapture( argv[1] );

    //exibir primeiro frame do v�deo
    cvNamedWindow( "Frame de video",CV_WINDOW_AUTOSIZE );
    IplImage* frame;

    frame = cvQueryFrame( video );

    cvShowImage( "Frame de video", frame );

    cvWaitKey(0);

    //limpeza
    cvReleaseCapture( &video );
    cvDestroyWindow("Frame de video");

    return(0);
}
