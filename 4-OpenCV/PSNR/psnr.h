/*
  Given two images, img_A (modified) and img_B (original),
  calculates the PSNR between them

  Developed by: Matheus Ricardo Uihara Zingarelli
  Adapted from: Rodolfo Ribeiro Silva
  Creation date: February, 16th 2011
  Last modification: February, 16th 2011

  Usage:
        psnr <img_A.bmp> <img_B.bmp> -TYPE

        TYPE: rgb or ycbcr
*/

#include <cv.h>
#include <highgui.h>
#include <math.h>

/*
  Calculates de Mean Square Error between two images. Both images must be
  1-channel, and have the same dimensions and pixel depth
  input:  original - original image
          processed - processed image to be compared with the original one
  output: a double value with the squared difference metric
*/
double MSE(IplImage* original, IplImage* processed);

/*
  Given two 1-channel images, performs the PSNR formula and return the result.
  input:  original - original image
          processed - processed image to be compared with the original one
          maxError - maximum value a pixel can have (255 for 8-bit pixel)
  output: PSNR value
*/
double getPSNR(IplImage* original, IplImage* processed, int maxError);

/*
  Calculates de Peak Signal-to-Noise Ratio between two 3-channel images. Images must
  have the same dimensions and pixel depth. There's an option in which images are
  converted to YCbCr prior to the PSNR measurement.
  input:  original - original image
          processed - processed image to be compared with the original one
          maxError - maximum value a pixel can have (255 for 8-bit pixel)
          res1, res2, res3 - PSNR values for each channel
          type - 0 for RGB PSNR; 1 for YCbCr PSNR
  output: returns PSNR values for each channel
*/
void PSNR(IplImage* original, IplImage* processed, int maxError, double *res1, double *res2, double *res3, char* type);
