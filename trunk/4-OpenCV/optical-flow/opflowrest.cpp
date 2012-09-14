#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <sstream>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
      const char *file1, *file2, *file3, *file4;
      int i, option = 0;

      Mat im1, im2;
      
      im1 = imread(argv[1]);
      if (!im1.data) {
	  printf("Nao foi possivel carregar imagem de entrada 1\n");
	  return -1;
      }
      im2 = imread(argv[2]);
      if (!im1.data) {
	  printf("Nao foi possivel carregar imagem de entrada 2\n");
	  return -1;
      }
      
      // create vector of 3 images to split channels
      // true images
      vector<Mat> RGBtrue1;
      RGBtrue1.push_back( Mat(im1.rows, im1.cols, CV_8UC1));
      RGBtrue1.push_back( Mat(im1.rows, im1.cols, CV_8UC1));
      RGBtrue1.push_back( Mat(im1.rows, im1.cols, CV_8UC1));

      vector<Mat> RGBtrue2;
      RGBtrue2.push_back( Mat(im1.rows, im1.cols, CV_8UC1));
      RGBtrue2.push_back( Mat(im1.rows, im1.cols, CV_8UC1));
      RGBtrue2.push_back( Mat(im1.rows, im1.cols, CV_8UC1));

      // split image 1
      split(im1, RGBtrue1);
      // split image 2
      split(im2, RGBtrue2);

      // images to estimate channels
      vector<Mat> RGBest1;
      RGBest1.push_back( RGBtrue1.at(0)); // B1
      RGBest1.push_back( Mat::zeros(im1.rows, im1.cols, CV_8UC1)); // G (empty)
      RGBest1.push_back( RGBtrue1.at(2)); // R1
      
      Mat imRB, grayRB1, grayG2;
      merge(RGBest1, imRB);
      cvtColor(imRB, grayRB1, CV_RGB2GRAY); // gray image composed by R1 and B1 channels
      grayG2 = Mat(RGBtrue2.at(1));
      
      // image just to compare
      vector<Mat> RGBest2;
      RGBest2.push_back( RGBtrue1.at(0)); // B1
      RGBest2.push_back( RGBtrue2.at(1)); // G2
      RGBest2.push_back( RGBtrue1.at(2)); // R1
      // flow matrix (velocity in x and y directions)
      Mat flow;

      // blur before computing optical flow 
      medianBlur(grayRB1, grayRB1, 3);
      medianBlur(grayG2, grayG2, 3);
      
      // InputArray prev, InputArray next, InputOutputArray flow, double pyr_scale, int levels, int winsize, int iterations, int poly_n, double poly_sigma, int flags 
      // 		 prev            next               flow    pscale  lev wins  it   polyn polys flags
      // 		const cv::Mat&, const cv::Mat&,   cv::Mat&, double, int, int, int, int, double, int
      //calcOpticalFlowFarneback(RGBtrue2.at(1), RGBest1.at(2), flow, 0.5,     1,    1,  10, 2 , 1.5,  0 );
      calcOpticalFlowFarneback(grayRB1, grayG2, flow, 0.5,     1,    1,  10,  2 , 1.5,  0 );

      // separate x and y velocitys
      Mat xy[2]; //X,Y
      split(flow, xy);

      int y, x;
      
      // compute the displacement of each pixel from G_2 to R_1
      for( x = 0; x < flow.rows; x++)  {
	  for( y = 0; y < flow.cols; y++) {
	      //grab flow for X and Y
	      float vx = xy[0].at<float>(x,y);
	      float vy = xy[1].at<float>(x,y);
	      
	      //calculate displacement
	      int dx = (int)x+vx;
	      int dy = (int)y+vy;
	      // check for out of bounds values
	      dx = (dx > flow.rows || dx < 0) ? 0 : dx;
	      dy = (dy > flow.cols || dy < 0) ? 0 : dy;
	      
	      // copy value from G_2 to G_1
	      RGBest1.at(1).at<uchar>(x,y) = RGBtrue2.at(1).at<uchar>(dx,dy);
	  }
      }
      
      // median blur to remove small noise
      medianBlur(RGBest1.at(1), RGBest1.at(1), 3);
      
      // merge the channels to form the final restored image

      // image with new estimated G channel
      Mat im1est;
      merge(RGBest1, im1est);

      // image with G2 channel
      Mat im2est;
      merge(RGBest2, im2est);

//       //calculate angle and magnitude
//       Mat magnitude, angle;
//       cartToPolar(xy[0], xy[1], magnitude, angle, true);
// 
//       //translate magnitude to range [0;1]
//       double mag_max;
//       minMaxLoc(magnitude, NULL, &mag_max, NULL, NULL, Mat());
//       magnitude.convertTo(magnitude, -1, 1.0/mag_max);

      // show restored image
      namedWindow("G recovery", CV_WINDOW_AUTOSIZE );
      imshow("G recovery", im1est);
      
      // write image in the disk
      imwrite("out_recover.png", im1est);
      imwrite("out_displaced.png", im2est);
      waitKey(0);
      return 0;
}
