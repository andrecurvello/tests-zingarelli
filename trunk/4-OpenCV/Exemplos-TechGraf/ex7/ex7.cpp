/**
* TUTORIAL OPENCV
* EXEMPLO 
*
* @author   MALF - malf@tecgraf.puc-rio.br
* @date     29/06/07
*/


#include <cv.h>
#include <cvaux.h>
#include <highgui.h>
#include <stdio.h>
#include <math.h>

/** 
Main
*/
int main( int argc, char** argv )
{
  //////////////////////////////////////////////////////////////////////////
  //Carregando imagem de entrada
  IplImage* bgr = cvLoadImage("..\\imagens\\casa.jpg");
  cvNamedWindow("bgr",0);
  cvShowImage( "bgr", bgr );

  //////////////////////////////////////////////////////////////////////////
  //Alterar espaço de cores para LAB
  IplImage* lab = cvCreateImage( cvGetSize(bgr), 8, 3 );
  cvCvtColor( bgr, lab, CV_BGR2Lab );

  IplImage* a = cvCreateImage( cvGetSize(bgr), 8, 1 );
  cvSplit( bgr, NULL, a, NULL, 0 );

  //////////////////////////////////////////////////////////////////////////
  // Parametros da criação dos histogramas
  int dim = 1;                        //Número de dimensões do histograma
  int hist_size = 256;  
  int sizes[] = {hist_size};          //Tamanho do vetor de cada dimensão do histograma
  int type = CV_HIST_ARRAY;           //Tipo de Histograma, utilizando padrão

  float xranges_a[] = { -127, 127 }; 
  float* ranges_a[] = { xranges_a };

  //////////////////////////////////////////////////////////////////////////
  // Criar histogramas
  CvHistogram* hist_a = cvCreateHist( dim, sizes, type, ranges_a, 1 ); 

  //////////////////////////////////////////////////////////////////////////
  // Calcular o histograma baseado no canal cinza da imagem
  IplImage* planes_a[] = { a };

  cvCalcHist( planes_a, hist_a, 0, NULL);

  //////////////////////////////////////////////////////////////////////////
  // Calcular o maior e o menor valor nos histogramas
  float max_a = 0, min_a = 0;
  int max_id_a = 0, min_id_a = 0;

  cvGetMinMaxHistValue( hist_a, &min_a, &max_a, &min_id_a, &max_id_a);

  //////////////////////////////////////////////////////////////////////////
  // Criar imagens de 8 bit para armazenar os desenhos da curva do histogramas L, A e B
  IplImage* histImg_a = cvCreateImage(cvSize(256, 50),8,3);

  cvRectangle(histImg_a, cvPoint(0,0), cvPoint(256,50), CV_RGB(255,255,255),-1);
  //////////////////////////////////////////////////////////////////////////
  // Desenhar Histograma
  float value1_a,  value2_a;
  int norm1_a;  int norm2_a;

  cvSet( histImg_a, cvScalarAll(255), 0 );

  //Calculando valores do histograma para L
  for(int i=0; i < hist_size; i++)
  {
    value1_a = cvQueryHistValue_1D( hist_a, i );
    norm1_a = cvRound(value1_a*50/max_a);

    //Hist Linha
    if(i<hist_size-1)
    {
      value2_a = cvQueryHistValue_1D( hist_a, i+1 );
      norm2_a = cvRound(value2_a*50/max_a);

      cvLine(histImg_a,cvPoint(i,50-norm1_a), cvPoint(i+1,50-norm2_a), CV_RGB(255,0,0)); 
    }
  }

  //////////////////////////////////////////////////////////////////////////
  // Exibir Curva do histograma
  cvNamedWindow("histogram-a",0);
  cvShowImage( "histogram-a", histImg_a );


  int keyCode = cvWaitKey();

  //////////////////////////////////////////////////////////////////////////
  //Limpar imagens
  cvReleaseImage(&bgr);
  cvReleaseImage(&lab);
  cvReleaseImage(&a);
  cvReleaseImage(&histImg_a);


  //////////////////////////////////////////////////////////////////////////
  //Remover Janelas
  cvDestroyWindow("bgr");
  cvDestroyWindow("histImg_a");;

  return 0;
}
