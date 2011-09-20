/*

SEMCOMP 14 - Minicurso Criação de imagens e vídeos 3D com OpenCV
Matheus Ricardo Uihara Zingarelli (zinga@icmc.usp.br)
19/09/2011

Programa que recebe um arquivo de imagem como parâmetro, cria uma nova imagem com apenas um canal de cor e aplica
a transformação da primeira imagem em tons de cinza, armazenando o resultado na nova imagem criada. Salva então a nova imagem
em disco.

*/

#include "highgui.h"
#include "cv.h"
#include <stdio.h>

int main(int argc, char** argv){
    //abrir imagem
    IplImage *img1 = cvLoadImage(argv[1], 1);

    //criar nova imagem com 1 canal
    IplImage *img2 = cvCreateImage(cvGetSize(img1),img1->depth,1);

    //conversão grayscale
    cvCvtColor(img1, img2, CV_BGR2GRAY);

    //salvar imagem
    if(cvSaveImage("Imagem-grayscale.bmp",img2)){
        printf("Operacao realizada com sucesso!\n");
    }
    else{
        printf("Erro ao salvar imagem em disco\n");
    }

    //limpeza
    cvReleaseImage(&img1);
    cvReleaseImage(&img2);
}
