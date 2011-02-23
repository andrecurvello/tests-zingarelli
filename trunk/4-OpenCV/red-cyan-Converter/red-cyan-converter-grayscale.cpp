/*
  Red-cyan image converter (grayscale). Uses a pair of stereoscopic images (in
  grayscale mode) and creates a single anaglyph image for red-cyan glasses.
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
    IplImage *rightImg, *leftImg, *anaglyph, *grayR, *grayL;

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
    
    CvSize size = cvGetSize(rightImg);
    grayR = cvCreateImage(size, rightImg->depth, 1);
    grayL = cvCreateImage(size, rightImg->depth, 1);
    
    //convert images to grayscale
    cvCvtColor(rightImg, grayR, CV_BGR2GRAY);
    cvCvtColor(leftImg, grayL, CV_BGR2GRAY);
    
    //revert to RGB (grayscale with 3 channels, all have the same pixel value)
    cvCvtColor(grayR, rightImg, CV_GRAY2BGR);
    cvCvtColor(grayL ,leftImg , CV_GRAY2BGR);
    
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
    anaglyph = cvCreateImage(size, rightImg->depth, rightImg->nChannels);
    //make sure anaglyph image is empty ("black")
    cvZero(anaglyph);
    
    //junctions images
    cvAdd(rightImg, leftImg, anaglyph);
    
    //save junctioned image
    cvSaveImage("anaglyph-grayscale.bmp", anaglyph);
    
    /*
    cvSaveImage("red-channel-grayscale.bmp", rightImg);
    cvSaveImage("cian-channel-grayscale.bmp", leftImg);
    */
    
    printf("\n\n\nImagem anaglifica criada com sucesso na pasta em que se encontra o programa!\n\n\n");
    
    //Free pointers
    cvReleaseImage(&rightImg);
    cvReleaseImage(&leftImg);
    cvReleaseImage(&anaglyph);
    
    return 0;
}
