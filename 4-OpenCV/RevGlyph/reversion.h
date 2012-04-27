/*
  Developed by: Matheus Ricardo Uihara Zingarelli
  Creation date: January, 4th 2012
  Last modification: January, 4th 2012

  This library contains procedures for anaglyph reversion of stereoscopic images.
  The reversion is enabled due to proper conversion using the Color Index Table.
*/

#include <cv.h>
#include <highgui.h>

/*
  The reversion process is enabled due to proper conversion using the Color Index
  Table. Compressed file is opened, the first byte is read (it contains metadata
  regarding which anaglyph was created, how the stereopair is represented and which
  subsampling was used), then data from both anaglyph and the color index table is
  retrieved. The complementar anaglyph is rebuilt and then subsampling is reversed
  to 4:4:4. Channels are rearranged and the stereo pair is obtained.
  Input: filename - name of the compressed file
*/
void anaglyphReversion(char* filename);

/*
  Reads control data e size regarding the image from the compressed file. Control data inside the
  first byte of the file. Image size (width, height and pixel depth) is inside the next three bytes
  Input: filename - name of the compressed file
         metadata - information regarding the type of anaglyph, stereopair representation and subsampling type used
         imageSize - width[0], height[1] and pixel depth[2] of the anaglyph image
*/
void readMetadata(char* filename, uchar *metadata, int imageSize[]);

/*
  Reads pixel data regarding the image from the compressed file. The first thirteen bytes are metadata, so
  pixel data starts after the 13th byte. Then we have the main anaglyph data (2*imageSize bytes), followed by
  the Color Index Table data (imageSize bytes).
  Input: filename - name of the compressed file
         anaglyph - container for the main anaglyph data
         cit - container for the color index table data
         imageSize - width[0], height[1] and pixel depth[2] of the anaglyph image
*/
void readPixelData(char* filename, uchar** anaglyph, uchar** cit, int* imageSize);

/*
  Rebuild the complementary anaglyph, by extracting the Y component from the main anaglyph.
  Input: filename - name of the compressed file
         mainAnaglyph - container for the main anaglyph image
         complAnaglyph - container for the complementary anaglyph image
         anaglyph - data from the main anaglyph
         cit - data from the color index table
         metadata - information regarding the type of anaglyph, stereopair representation and subsampling type used
         imageSize - width[0], height[1] and pixel depth[2] of the anaglyph image
*/
void rebuildAnaglyph(char* filename, IplImage* mainAnaglyph, IplImage* complAnaglyph, uchar* anaglyph, uchar* cit, uchar metadata, int* imageSize);

/*
  Extracts the Y component from the main anaglyph. The Y data is in the second half of the
  anaglyph data (after imageSize bytes), as exemplified below:
   ---------------------------------------------------------------
  |       Cb       |       Cr       |              Y              |
   ---------------------------------------------------------------
  /--imageSize/2---/--imageSize/2---/---------imageSize-----------/
  Input: anaglyph - main anaglyph data
         imageSize - size of the anaglyph image
  Output: uchar elements containing data from the Y component
*/
uchar* extractY(uchar* anaglyph, int imageSize);

/*
  The luminance from the complementary anaglyph is rebuilt by using the luminance
  from the main anaglyph and the luminance differences calculated during conversion
  Input: filename - name of the compressed file
         analyph - main anaglyph data
         imageSize - size of the anaglyph image
  Output: datastream with the luminance from the complementary anaglyph
*/
uchar* recoverComplY(char* filename, uchar* anaglyph, int imageSize);

/*
  Creates the complementary anaglyph using the Y component from the main anaglyph
  Input: cit - data from the color index table
         Y - luminance data from the main anaglyph
         imageSize - size of the anaglyph image
  Output: uchar elements containg the rebuilt complementary anaglyph
*/
uchar* buildComplementaryAnaglyph(uchar* cit, uchar* Y, int imageSize);

/*
  Converts an image from YCbCr to RGB. Image was previously subsampled, so it also has to
  be reverted to 4:4:4 prior to RGB conversion. It is possible to know which kind of chrominance
  subsampling was applied by using the metadata.
  Input: image - container for the image
         imageData - pixel data
         metadata - information regarding the type of anaglyph, stereopair representation and subsampling type used
         imageSize - width[0], height[1] and pixel depth[2] of the anaglyph image
*/
void convert2RGB(IplImage* image, uchar* imageData, uchar metadata, int* imageSize);

/*
  Given metadata on the compressed image, performs 3 left shift to find out which chrominance
  subsampling was used. Informations regarding chrominance subsampling is in the third bit of
  metadata.
  Input: metadata - information regarding the type of anaglyph, stereopair representation and subsampling type used
  Output: integer number (returns 128 for 4:2:2 subsampling and 0 for 4:4:0 subsampling)
*/
int extractSubsampling(uchar metadata);

/*
  Reverts chrominance subsampling from 4:4:0 to the original 4:4:4. 4:4:0 was based by calculating the
  average values of two consecutive rows. For the reversion, we will simply copy the same value for two
  consecutive rows. Image data is organized as below:
   ---------------------------------------------------------------------------------------------
  |         Cb          |           Cr          |                       Y                       |
   ---------------------------------------------------------------------------------------------
  /-----imageSize/2-----/-----imageSize/2-------/---------------imageSize-----------------------/
  Input: image - container for the image
         imageData - pixel Data
         imageSize - width[0], height[1] and pixel depth[2] of the anaglyph image
*/
void revertSubsampling440(IplImage* image, uchar* imageData, int* imageSize);

/*
  Given two anaglyphs, recreates the original stereo pair. Uses metadata to decide wether to create a
  side-by-side or above-below image.
  Input: mainAnaglyph - main anaglyph
         complAnaglyph - complementary anaglyph
         metadata - information regarding the type of anaglyph, stereopair representation and subsampling type used
*/
void rebuildStereoPair(IplImage* mainAnaglyph, IplImage* complAnaglyph, uchar metadata, char* filename);

/*
  Given metadata on the compressed image, performs 2 left shifts to find out wether the original stereo pair
  was represented by a side-by-side or an above-below image.
  Input: metadata - information regarding the type of anaglyph, stereopair representation and subsampling type used
  Output: integer number (returns 128 if above-below and 0 if side-by-side)
*/
int extractRepresentation(char metadata);

/*
  Creates a stereo pair with images displaced side by side. The stereo pair will
  have the double of image width compared to the anaglyphs.
  Input: mainAnaglyph - main anaglyph
         complAnaglyph - complementary anaglyph
*/
void createSBSImage(IplImage* mainAnaglyph, IplImage* complAnaglyph, char* filename);

/*
  Creates a stereo pair with images displaced above and below. The stereo pair will
  have the double of image height compared to the anaglyphs.
  Input: mainAnaglyph - main anaglyph
         complAnaglyph - complementary anaglyph
*/
void createABImage(IplImage* mainAnaglyph, IplImage* complAnaglyph, char* filename);
