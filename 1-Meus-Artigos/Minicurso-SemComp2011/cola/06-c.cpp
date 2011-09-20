/*

SEMCOMP 14 - Minicurso Criação de imagens e vídeos 3D com OpenCV
Matheus Ricardo Uihara Zingarelli (zinga@icmc.usp.br)
19/09/2011

Programa que recebe um arquivo de imagem como parâmetro, divide ao meio e cria duas novas imagens
Utilizando a leitura dos dados

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

    //aplicar ROI na primeira metade da imagem (Não é necessário)
    //cvSetImageROI(img1, cvRect(0,0,img1->width/2,img1->height));

    //aplicar o ROI à segunda metade da imagem (Não é necessário)
    //cvSetImageROI(img1, cvRect(img1->width/2,0,img1->width/2,img1->height));

    //percorrer dados da imagem (primeira metade)
    for(int row=0; row<img1->height; row++){
        uchar *ptr1 = (uchar*) (img1->imageData + row * img1->widthStep);
        uchar *ptr2 = (uchar*) (img2->imageData + row * img2->widthStep);
        for(int col=0; col<img1->width/2; col++){
            ptr2[3*col] = ptr1[3*col];
            ptr2[3*col+1] = ptr1[3*col+1];
            ptr2[3*col+2] = ptr1[3*col+2];
        }
    }

    //salvar imagem
    if(cvSaveImage("Imagem-esquerda.bmp",img2)){
        printf("Operacao realizada com sucesso -- primeira metade!\n");
    }
    else{
        printf("Erro ao salvar imagem em disco -- primeira metade\n");
    }

    //percorrer dados da imagem (segunda metade)
    for(int row=0; row<img1->height; row++){
        uchar *ptr1 = (uchar*) (img1->imageData + row * img1->widthStep);
        uchar *ptr2 = (uchar*) (img3->imageData + row * img3->widthStep);
        int count=0;
        for(int col=img1->width/2; col<img1->width; col++){
            ptr2[3*count] = ptr1[3*col];
            ptr2[3*count+1] = ptr1[3*col+1];
            ptr2[3*count+2] = ptr1[3*col+2];
            count++;
        }
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
