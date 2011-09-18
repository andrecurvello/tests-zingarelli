/*

SEMCOMP 14 - Minicurso Cria��o de imagens e v�deos 3D com OpenCV
Matheus Ricardo Uihara Zingarelli (zinga@icmc.usp.br)

Programa que recebe um arquivo de imagem como par�metro (par est�reo), separa as duas imagens
e aplica a tranforma��o anagl�fica vermelho-ciano.

*/

#include "highgui.h"
#include "cv.h"
#include <stdio.h>

int main(int argc, char** argv){
    //ABRIR IMAGENS
    IplImage *img1 = cvLoadImage(argv[1], 1);
    
    //CRIAR NOVA IMAGEM (AN�GLIFO)
    IplImage *anaglyph = cvCreateImage(cvSize(img1->width/2, img1->height), img1->depth, img1->nChannels);
    
    //SEPARAR IMAGENS
    IplImage *imgLeft  = cvCreateImage(cvSize(img1->width/2,img1->height),img1->depth,img1->nChannels);
    IplImage *imgRight = cvCreateImage(cvSize(img1->width/2,img1->height),img1->depth,img1->nChannels);

    //esquerda
    cvSetImageROI(img1, cvRect(0,0,img1->width/2,img1->height));
    cvCopy(img1,imgLeft);
    
    //direita
    cvSetImageROI(img1, cvRect(img1->width/2,0,img1->width/2,img1->height));
    cvCopy(img1,imgRight);
    
    //SELECIONAR COMPONENTES DE COR NECESS�RIAS
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
    
    //GRAVAR O AN�GLIFO
    if(cvSaveImage("anaglifo.bmp",anaglyph)){
        printf("Anaglifo criado com sucesso!\n");
    }
    else{
        printf("Erro na grava��o do anaglifo\n");
    }
    
    //LIMPEZA
    cvReleaseImage(&img1);
    cvReleaseImage(&imgLeft);
    cvReleaseImage(&imgRight);
    cvReleaseImage(&anaglyph);
    
}
