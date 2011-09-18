/*7

SEMCOMP 14 - Minicurso Criação de imagens e vídeos 3D com OpenCV
Matheus Ricardo Uihara Zingarelli (zinga@icmc.usp.br)

Programa que recebe um arquivo de vídeo, aplica uma transformação em cada frame e
grava em um novo arquivo de vídeo

*/

#include "highgui.h"
#include "cv.h"
#include <stdio.h>
#include <time.h>

int main(int argc, char** argv){
    //abrir vídeo
    CvCapture* video = cvCreateFileCapture( argv[1] );

    //recebe cada frame de vídeo
    IplImage* frame;

    //copiar propriedades do vídeo
    double fps = cvGetCaptureProperty(video, CV_CAP_PROP_FPS);
    CvSize size = cvSize(
                         (int)cvGetCaptureProperty(video, CV_CAP_PROP_FRAME_WIDTH),
                         (int)cvGetCaptureProperty(video, CV_CAP_PROP_FRAME_HEIGHT)
                         );

    //criar escritor de vídeo
    CvVideoWriter *writer = cvCreateVideoWriter(
                                               "novo-video.avi",
                                               -1,//CV_FOURCC('M','J','P','G') -dando pau, escolher codec manualmente com -1
                                               fps,
                                               size,
                                               1
                                               );

    //imagem processada
    IplImage* image = cvCreateImage(size, IPL_DEPTH_8U, 3);

    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    printf("\n\nGravando arquivo, aguarde alguns minutos...\nInicio: %s\n", asctime (timeinfo) );
    while((frame = cvQueryFrame(video)) != NULL){ //enquanto o vídeo não terminou
        cvCvtColor(frame,frame, CV_BGR2YCrCb);
        cvWriteFrame(writer, frame);
    }
    time( &rawtime );
    timeinfo = localtime( &rawtime );
    printf("\n\nArquivo gravado com sucesso!\nFim: %s \n",asctime (timeinfo));

    //limpeza
    cvReleaseCapture( &video );
    cvReleaseVideoWriter( &writer );
    cvReleaseImage( &image );

    return(0);
}
