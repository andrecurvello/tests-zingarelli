/**
* OpenCV: Working with RGB channels
* Using openCV library to open an image and create new images from it,
* playing with its RGB channels and also the grayscale of each channel.
*
* @author: Matheus Ricardo Uihara Zingarelli
* @date: 10/28/2010
*
*/

#include <cv.h>
#include <cvaux.h>
#include <highgui.h>
#include <stdio.h>

int main(int argc, char **argv){
    /* Load image */
    IplImage* img_original = cvLoadImage("img\\carro.jpg");  
    
    /* Split image into each RGB channel */         
    IplImage* img_R = cvCreateImage( cvGetSize( img_original ), IPL_DEPTH_8U, 1 );
    IplImage* img_G = cvCreateImage( cvGetSize( img_original ), IPL_DEPTH_8U, 1 );
    IplImage* img_B = cvCreateImage( cvGetSize( img_original ), IPL_DEPTH_8U, 1 );
    
    /* ATTENTION, image is loaded as BGR not RGB*/
    cvSplit( img_original, img_B, img_G, img_R, 0 );
    
//    /* View images */
//    cvNamedWindow( "img_original", 0 );
//    cvNamedWindow( "img_R", 0 );
//    cvNamedWindow( "img_G", 0 );
//    cvNamedWindow( "img_B", 0 );
//
//  
//    cvShowImage( "img_original", img_original );
//    cvShowImage( "img_R", img_R );
//    cvShowImage( "img_G", img_G );
//    cvShowImage( "img_B", img_B );
//
//    /* Manter o programa ativo enquanto nenhuma tecla for pressionada */
//    int keyCode = cvWaitKey(0);
    
    /* Save the new images*/
    cvSaveImage("img\\carro_R.jpg", img_R);
    cvSaveImage("img\\carro_G.jpg", img_G);    
    cvSaveImage("img\\carro_B.jpg", img_B);
    
    /* Desalocar imagens */
    cvReleaseImage( &img_original );
    cvReleaseImage( &img_R );
    cvReleaseImage( &img_G);
    cvReleaseImage( &img_B );
    
//    /* Desalocar janelas*/
//    cvDestroyWindow( "img_original" );
//    cvDestroyWindow( "img_R" );
//    cvDestroyWindow( "img_G" );
//    cvDestroyWindow( "img_B" );
    
    return 0;     
        
}
