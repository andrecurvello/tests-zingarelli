/*

SEMCOMP 14 - Minicurso Criação de imagens e vídeos 3D com OpenCV
Matheus Ricardo Uihara Zingarelli (zinga@icmc.usp.br)
19/09/2011

Programa que recebe um arquivo de vídeo e o reproduz

*/

#include "highgui.h"
#include "cv.h"
#include <stdio.h>

int main(int argc, char** argv){
    //abrir vídeo
    CvCapture* video = cvCreateFileCapture( argv[1] );

    cvNamedWindow( "Player",CV_WINDOW_AUTOSIZE );
    IplImage* frame;

    //exibir frames de vídeo em loop infinito
    /* versão sem utilizar o fps, não toca o vídeo na taxa certa
    while(1){
        frame = cvQueryFrame( video );
        if(!frame){break;}//fim de video
        cvShowImage( "Player", frame );
        char c = cvWaitKey(33); //exibe por 33milisegundos antes de passar ao próximo frame
        if(c==27){break;}//para exibição se usuário apertar ESC
    }
    */

    /*versão com FPS para tocar o vídeo na taxa certa*/
    int fps = (int) cvGetCaptureProperty(video, CV_CAP_PROP_FPS);
    while(1){
        frame = cvQueryFrame( video );
        if(!frame){break;}//fim de video
        cvShowImage( "Player", frame );
        char c = cvWaitKey(1000/fps); //fps a cada 1 segundo (1000milisegundos)
        if(c==27){break;}//para exibição se usuário apertar ESC
    }

    //limpeza
    cvReleaseCapture( &video );
    cvDestroyWindow("Player");

    return(0);
}
