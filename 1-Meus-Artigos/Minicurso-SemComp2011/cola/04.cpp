/*

SEMCOMP 14 - Minicurso Criação de imagens e vídeos 3D com OpenCV
Matheus Ricardo Uihara Zingarelli (zinga@icmc.usp.br)

Programa que recebe um arquivo de imagem, corta-a verticalmente e exibe cada metade em uma janela diferente


*/

#include "highgui.h"
#include "cv.h"

int main(int argc, char** argv){
    //abrir imagem
    IplImage *img = cvLoadImage(argv[1], 1);

    //aplicar ROI na primeira metade da imagem
    cvSetImageROI(img, cvRect(0,0,img->width/2,img->height));

    //mostrar a imagem
    cvNamedWindow( "Metade Esquerda", CV_WINDOW_AUTOSIZE );
    cvShowImage( "Metade Esquerda", img );

    //aplicar o ROI à segunda metade da imagem
    cvSetImageROI(img, cvRect(img->width/2,0,img->width/2,img->height));

    //mostrar nova imagem
    cvNamedWindow( "Metade Direita", CV_WINDOW_AUTOSIZE );
    cvShowImage( "Metade Direita", img );

    cvWaitKey(0);

    //limpeza
    cvResetImageROI(img);
    cvReleaseImage(&img);
    cvDestroyWindow("Metade Direita");
    cvDestroyWindow("Metade Esquerda");
}
