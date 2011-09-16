/*

SEMCOMP 14 - Minicurso Criação de imagens e vídeos 3D com OpenCV
Matheus Ricardo Uihara Zingarelli (zinga@icmc.usp.br)

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

    //criar novas imagems com metade da largura original
    IplImage *img2 = cvCreateImage(cvSize(img1->width/2,img1->height),img1->depth,img1->nChannels);
    IplImage *img3 = cvCreateImage(cvSize(img1->width/2,img1->height),img1->depth,img1->nChannels);

     //aplicar ROI na primeira metade da imagem
    cvSetImageROI(img1, cvRect(0,0,img1->width/2,img1->height));

    //copiar dados em img2
    cvCopy(img1,img2);

    //aplicar o ROI à segunda metade da imagem
    cvSetImageROI(img1, cvRect(img1->width/2,0,img1->width/2,img1->height));

    //copiar dados em img2
    cvCopy(img1,img3);

    //salvar imagem
    if(cvSaveImage("Imagem-esquerda.bmp",img2)){
        printf("Operacao realizada com sucesso -- primeira metade!\n");
    }
    else{
        printf("Erro ao salvar imagem em disco -- primeira metade\n");
    }
    if(cvSaveImage("Imagem-direita.bmp",img3)){
        printf("Operacao realizada com sucesso -- segunda metade!\n");
    }
    else{
        printf("Erro ao salvar imagem em disco -- segunda metade\n");
    }

    //limpeza
    cvReleaseImage(&img1);
    cvReleaseImage(&img2);
    cvReleaseImage(&img3);
}
