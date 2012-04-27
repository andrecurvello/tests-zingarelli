/*
  Developed by: Matheus Ricardo Uihara Zingarelli
  Creation date: December, 14th 2011
  Last modification: December, 14th 2011

  This library contains procedures for anaglyph conversion of stereoscopic images.
  The conversion is implemented in a way that enables posterior reversion of the process,
  with the creation of what we called "Color Index Table".
*/

#include <cv.h>
#include <highgui.h>

//RLE struct for luminance differences
struct rle_struct{
    char value; //value of the difference between pixels of Ym and Yc
    uchar qty;  //number of times the value is repeated in a sequence
};

/*
  Creates a uchar value (1byte) based on the parameters to be recorded in the compressed file,
  observing the following requirements:
  - first two bits to describe which anaglyph was created:
    00 - green-magenta
    01 - red-cyan
    10 - blue-yellow
  - next bit to describe if the original image was either side-by-side or above-below
    0 - side-by-side
    1 - above-below
  - next bit to describe which subsampling was used:
    0 - 4:4:0
    1 - 4:2:2
  - The remainig 4 bits are reserved to future enhacements (more anaglyph or subsampling types to be added).
  Input: parameters: the parameters entered by user via command line
  Output: uchar value

*/
uchar createMetadata(char* parameters[]);

/*
  Stores data from the compressed anaglyph and the color index table in a single file. First byte of this file
  contains control parameters for the reversion process:
  - first two bits to describe which anaglyph was created:
    00 - green-magenta
    01 - red-cyan
    10 - blue-yellow
  - next bit to describe if the original image was either side-by-side or above-below
    0 - side-by-side
    1 - above-below
  - next bit to describe which subsampling was used:
    0 - 4:4:0
    1 - 4:2:2
  - The remainig 4 bits are reserved to future enhacements (more anaglyph or subsampling types to be added).
  Next 12 bytes contain two int with the size of the anaglyph (width and height), followed by another int with
  pixel depth. After that comes data from the main anaglyph, followed by data from the Color Index Table
  Input: anaglyph - data regarding the main anaglyph (subsampled)
         cit - data regarding the color index table
         parameters - the parameters entered by user via command line
         width - width of one of the images from the stereopair
         height - height of one of the images from the stereopair
         depth - pixel depth of one of the images from the stereopair
         rle_elements - array with the lumdiff elements created by applying rle to it
         rle_size - number of elements of rle_elements
*/
void saveData(uchar* anaglyph, uchar* cit, char* parameters[], int width, int height, int depth, rle_struct* rle_elements, int rle_size);

/*
  Calculates pixel differences between luminance components of main and complementary anaglyphs
  Input: mainData - main anaglyph subsampled data
         complData - complementary anaglyph subsampled data
         imageSize - width*height of one of the images from the stereopair
  Output: uchar datastream with the differences
*/
char* diffY(uchar* mainData, uchar* complData, int imageSize);

/*
  Creates the Color Index Table. The color index table is formed by crominance data (Cb and Cr) from the
  complementary anaglyph. This way, it is removed the Y component, returning only data from Cb and Cr.
  By removing the Y component, we are discarding "imageSize" bytes from the compressed data, requiring
  only "imageSize" bytes to store crominance data.
  Input: data - data from the subsampled complementary anaglyph
         imageSize - width*height of one of the images from the stereopair
  Output: uchar datastream, with data from Y removed
*/
uchar* createCIT(uchar* data, int imageSize);

/*
  Converts an RGB image to YCbCr and then apply 4:4:0 subsampling. An image is represented by three matrices,
  each holding data from one of the components (one for Y, one for Cb and one for Cr) and each with a lenght
  of width*height pixels, thus needing to store 3*imageSize pixels for a 4:4:4 image. Since we're applying
  4:4:0 subsampling, we're reducing data from Cb and Cr to a half, thus needing to store 1/2*imageSize pixels
  for Cb and 1/2*imageSize pixels for Cr, which means we only need to store data for 2*imageSize pixels.
  Input: image - image to be subsampled
  Output: uchar datastream, with data in the following order: (subsampled)Cb, (subsampled)Cr and Y
   ---------------------------------------------------------------------------------------------
  |         Cb          |           Cr          |                       Y                       |
   ---------------------------------------------------------------------------------------------
  /-----imageSize/2-----/-----imageSize/2-------/---------------imageSize-----------------------/
*/
uchar* subsample440(IplImage* image);

/*
  Create two anaglyphs, given two images. The anaglyph to be created can be green-magenta,
  red-cyan or blue-yellow, according to the type entered. Another anaglyph, complementar to
  the first one, is also created.
  Input: left - left image of the stereo pair
         right - right image of the stereo pair
         mainAnaglyph - green-magenta, red-cyan or blue-yellow anaglyph
         complAnaglyph - complementary anaglyph
         type - type of the anaglyph to be created (green-magenta, red-cyan or blue-yellow)
*/
void createAnaglyph(IplImage* left, IplImage* right, IplImage* mainAnaglyph, IplImage* complAnaglyph, char* type);

/*
  Open a stereo pair file and verify its dimensions, based on its type. If it is
  side-by-side, the image must be divisible by 2 on its width. If it is above-below,
  the image must be divisible by 2 on its heigth.
  Input: image - stereo pair
         type - side-by-side or above-below
*/
IplImage* loadAndVerify(char* image, char* type);

/*
  Given an stereo pair, copy and adapt its properties to the two images that composes it.
  Also copy properties to the two anaglyphs that will be created
  Input: stereoPair - original image
         type - side-by-side or above-below
         left - left image container
         right - right image container
         mainAnaglyph - main anaglyph
         complAnaglyph - complementary anaglyph
*/
void prepareImages(IplImage* stereoPair, char* type, IplImage** left,IplImage** right, IplImage** mainAnaglyph, IplImage** complAnaglyph);

/*
  Separates a stereo pair image into two images. The original image can be either
  side-by-side or above-below. If it is side-by-side, splits image by its width.
  If its above-below, splits image by its height.
  Input: stereoPair - original image
         type - side-by-side or above-below
         left - left image container
         right - right image container
*/
void splitImage(IplImage* stereoPair, char* type, IplImage** left, IplImage** right);

/*
  Given a stereo image, creates an anaglyph and stores the colors not used in the
  color index table file.
  Input: parameters - the parameters entered by user via command line
*/
void anaglyphConversion(char* parameters[]);
