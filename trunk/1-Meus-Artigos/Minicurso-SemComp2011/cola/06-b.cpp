/*

SEMCOMP 14 - Minicurso Criação de imagens e vídeos 3D com OpenCV
Matheus Ricardo Uihara Zingarelli (zinga@icmc.usp.br)
19/09/2011

Programa que recebe um arquivo de imagem como parâmetro, divide ao meio e cria duas novas imagens
Utilizando apenas uma imagem e ROI

*/

#include "highgui.h"
#include "cv.h"
#include <stdio.h>

int main(int argc, char** argv){
    //abrir imagem
    IplImage *img1 = cvLoadImage(argv[1], 1);

     //aplicar ROI na primeira metade da imagem
    cvSetImageROI(img1, cvRect(0,0,img1->width/2,img1->height));

    //salvar imagem
    if(cvSaveImage("Imagem-esquerda.bmp",img1)){
        printf("Operacao realizada com sucesso -- primeira metade!\n");
    }
    else{
        printf("Erro ao salvar imagem em disco -- primeira metade\n");
    }

    //aplicar o ROI à segunda metade da imagem
    cvSetImageROI(img1, cvRect(img1->width/2,0,img1->width/2,img1->height));
    if(cvSaveImage("Imagem-direita.bmp",img1)){
        printf("Operacao realizada com sucesso -- segunda metade!\n");
    }
    else{
        printf("Erro ao salvar imagem em disco -- segunda metade\n");
    }

    //limpeza
    cvReleaseImage(&img1);
}
