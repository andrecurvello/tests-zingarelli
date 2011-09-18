/*

SEMCOMP 14 - Minicurso Criação de imagens e vídeos 3D com OpenCV
Matheus Ricardo Uihara Zingarelli (zinga@icmc.usp.br)

Programa que recebe um arquivo de vídeo como parâmetro e aplica a tranformação anaglífica vermelho-ciano.

*/

#include "highgui.h"
#include "cv.h"
#include <stdio.h>
#include <time.h>

int main(int argc, char** argv){
    //ABRIR VIDEO
    CvCapture* video = cvCreateFileCapture( argv[1] );
    IplImage *frame;

    //copiar propriedades do vídeo
    double fps = cvGetCaptureProperty(video, CV_CAP_PROP_FPS);
    CvSize size = cvSize(
                         (int)cvGetCaptureProperty(video, CV_CAP_PROP_FRAME_WIDTH)/2,
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

    //CRIAR NOVA IMAGEM (ANÁGLIFO)
    IplImage *anaglyph = cvCreateImage(size, IPL_DEPTH_8U, 3);
    IplImage *imgLeft  = cvCreateImage(size, IPL_DEPTH_8U, 3);
    IplImage *imgRight = cvCreateImage(size, IPL_DEPTH_8U, 3);


    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    printf("\n\nGravando arquivo, aguarde alguns minutos...\nInicio: %s\n", asctime (timeinfo) );
    while((frame = cvQueryFrame(video)) != NULL){ //enquanto o vídeo não terminou
        //SEPARAR IMAGENS
        //esquerda
        cvSetImageROI(frame, cvRect(0,0,frame->width/2,frame->height));
        cvCopy(frame,imgLeft);

        //direita
        cvSetImageROI(frame, cvRect(frame->width/2,0,frame->width/2,frame->height));
        cvCopy(frame,imgRight);

        //SELECIONAR COMPONENTES DE COR NECESSÁRIAS
        for( int row = 0; row < imgLeft->height; row++ ){
            uchar* ptrLeft  = (uchar*) ( imgLeft->imageData + row * imgLeft->widthStep );
            uchar* ptrRight = (uchar*) ( imgRight->imageData + row * imgRight->widthStep );
            uchar* ptrAnag  = (uchar*) ( anaglyph->imageData + row * anaglyph->widthStep );
            for( int col = 0; col < imgLeft->width; col++ ) {
                //Lembrando ordem das cores: B - G - R
                //G e B vem do lado esquerdo
                ptrAnag[3*col] = ptrLeft[3*col];
                ptrAnag[3*col+1] = ptrLeft[3*col+1];
                //R vem do lado direito
                ptrAnag[3*col+2] = ptrRight[3*col+2];
            }
        }

        //GRAVAR O ANÁGLIFO
        cvWriteFrame(writer, anaglyph);

    }
    time( &rawtime );
    timeinfo = localtime( &rawtime );
    printf("\n\nArquivo gravado com sucesso!\nFim: %s \n",asctime (timeinfo));


    //LIMPEZA
    cvReleaseImage(&imgLeft);
    cvReleaseImage(&imgRight);
    cvReleaseImage(&anaglyph);
    cvReleaseCapture(&video);
    cvReleaseVideoWriter(&writer);

}
