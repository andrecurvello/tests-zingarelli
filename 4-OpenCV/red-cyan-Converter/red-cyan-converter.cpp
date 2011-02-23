/*
  Red-cyan image converter. Uses a pair of stereoscopic images and creates a 
  single anaglyph image for red-cyan glasses. 
  *** Attention *** we're considering the red to be on the left side and cyan
  to be on the right side of the glasses. This way, right image will have both
  blue and green channels and left image will have only red channel.
  
  Developed by: Matheus Ricardo Uihara Zingarelli
  Creation date: Feb, 22nd. 2011
  Last modification: Feb, 22nd. 2011
*/

//red-cyan-converter <right_image> <left_image>

#include <stdio.h>
#include <cv.h>
#include <highgui.h>

int main(int argc, char* argv[]){
    IplImage *rightImg, *leftImg, *anaglyph; 

    //images are loaded by openCV with colors in this order: BGR
    //open right image
    rightImg = cvLoadImage(argv[1], 1);    
    //open left image
    leftImg = cvLoadImage(argv[2], 1);
    
    //Simple error handling
    if((!rightImg)||(!leftImg)){
       printf("Erro ao abrir as imagens.");
       exit(-1);
    }
    
    //Verify if both images are of the same size
    if((rightImg->width != leftImg->width) ||
       (rightImg->height != leftImg->height)){
          printf("Imagens possuem tamanhos diferentes. Favor redimensionar!");
          exit(-1);
    }
        
    //remove channels
    for(int row = 0; row < rightImg->height; row++){
            //set pointer to the correct position in each row
            uchar* ptrR = (uchar*)(rightImg->imageData + row * rightImg->widthStep);
            uchar* ptrL = (uchar*)(leftImg->imageData + row * leftImg->widthStep);
            
            for(int col = 0; col < rightImg->width; col++){
                    //remove blue and green channel from the right image
                    ptrR[3*col] = 0;
                    ptrR[3*col+1] = 0;
                    //remove red channel from the left image
                    ptrL[3*col+2] = 0;
            }            
    }
           
    //prepare anaglyph image
    CvSize size = cvGetSize(rightImg);
    anaglyph = cvCreateImage(size, rightImg->depth, rightImg->nChannels);
    //make sure anaglyph image is empty ("black")
    cvZero(anaglyph);
    
    //junctions images
    cvAdd(rightImg, leftImg, anaglyph);
    
    //save junctioned image
    cvSaveImage("anaglyph.bmp", anaglyph);
    
    /*
    cvSaveImage("red-channel.bmp", rightImg);
    cvSaveImage("cian-channel.bmp", leftImg);
    */
    
    printf("\n\n\nImagem anaglifica criada com sucesso na pasta em que se encontra o programa!\n\n\n");
    
    //Free pointers
    cvReleaseImage(&rightImg);
    cvReleaseImage(&leftImg);
    cvReleaseImage(&anaglyph);
    
    return 0;
}
