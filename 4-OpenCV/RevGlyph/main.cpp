/*
  Developed by: Matheus Ricardo Uihara Zingarelli
  Creation date: November, 28th 2011
  Last modification: November, 28th 2011

  RevGlyph Technique
  Anaglyph conversion & reversion of stereo images.
  In the anaglyph conversion, given a stereo image (one file containing a pair of images, either above-below or side-by-side)
  splits it into two images, creates an anaglyph version of it (green-magenta, red-cyan or blue-yellow) and a
  correspondent complementary anaglyph with the remaining colors. Images are converted to YCbCr color space
  and it is applied a color subsampling (4:2:2 or 4:4:0). At the end of the process, the Y component of the complementary
  anaglyph is removed, being only stored information regarding color in a structure called Color Index Table (CIT).
  In the anaglyph reversion, the Y component of the main anaglyph is copied and, together with the color information from
  the Color Index Table, the complementary anaglyph is recreated. Both anaglyphs are reverted to 4:4:4 color sampling and then
  to RGB. Colors are then reorganized to form the reverted stereo pair.

  Usage:
        Encode:
        RevGlyph -e <image.bmp> {-sbs | -ab} {-gm | -rc | -by} {-422 | -440}

        Decode:
        RevGlyph -d <compressed-file.dat>

        -e: encode
        -d: decode
        -sbs: side-by-side
        -ab: above-below
        -gm: green-magenta
        -rc: red-cyan
        -by: blue-yellow

*/

#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include "conversion.h"

/* --- GENERAL PURPOSE FUNCTIONS --- */
/* Prints usage method */
void printHelp();

/*
  Verifies if the parameters inputted by user are correct and valid.
  Input: argc - the number of parameteres entered by user via command line
         argv[] - the parameters entered by user via command line
*/
void verifyParameters(int argc, char* argv[]);

/* --- ANAGLYPH REVERSION FUNCTIONS --- */

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
  Input: mainAnaglyph - container for the main anaglyph image
         complAnaglyph - container for the complementary anaglyph image
         anaglyph - data from the main anaglyph
         cit - data from the color index table
         metadata - information regarding the type of anaglyph, stereopair representation and subsampling type used
         imageSize - width[0], height[1] and pixel depth[2] of the anaglyph image
*/
void rebuildAnaglyph(IplImage* mainAnaglyph, IplImage* complAnaglyph, uchar* anaglyph, uchar* cit, uchar metadata, int* imageSize);

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
void createSBSImage(IplImage* mainAnaglyph, IplImage* complAnaglyph, char* filename){
    printf("Creating the stereo pair... ");
    IplImage* stereoPair = cvCreateImage(cvSize((mainAnaglyph->width) * 2, mainAnaglyph->height), mainAnaglyph->depth, mainAnaglyph->nChannels);
    cvZero(stereoPair);
    for(int row = 0; row < mainAnaglyph->height; row++){
        //set pointer to the correct position in each row
        uchar* ptrStp = (uchar*)(stereoPair->imageData + row * stereoPair->widthStep);
        uchar* ptrAn = (uchar*)(mainAnaglyph->imageData + row * mainAnaglyph->widthStep);
        uchar* ptrAc = (uchar*)(complAnaglyph->imageData + row * complAnaglyph->widthStep);
        for(int col = 0; col < mainAnaglyph->width; col++){
            ptrStp[3*col] = ptrAn[3*col];                           //B left
            ptrStp[3*(col+mainAnaglyph->width)] = ptrAc[3*col];     //B right
            ptrStp[3*col+1] = ptrAc[3*col+1];                       //G left
            ptrStp[3*col+2] = ptrAn[3*col+2];                       //R left
            ptrStp[3*(col+mainAnaglyph->width)+1] = ptrAn[3*col+1]; //G right
            ptrStp[3*(col+mainAnaglyph->width)+2] = ptrAc[3*col+2]; //R right
        }
    }

    //save image
    char* imageName = strtok(filename,".");
    strcat(imageName,"reversed.bmp");
    //printf("\n\n%s\n\n", imageName);
    cvSaveImage(imageName, stereoPair);
    printf("OK!\n");

    cvReleaseImage(&stereoPair);
}

int extractRepresentation(char metadata){
    int result = metadata << 2;
    return (result & 128);
}

void rebuildStereoPair(IplImage* mainAnaglyph, IplImage* complAnaglyph, uchar metadata, char* filename){
    //find out if the original stereo pair was a side-by-side or above-below image
    int representation = extractRepresentation(metadata);

    //reorder color components
    switch(representation){
        case 0:
            createSBSImage(mainAnaglyph, complAnaglyph, filename);
            break;
        case 128:
            //TODO
            printf("\n\nCreate an above-below image... TBD\n\n");
    }

//----- UNIT TEST
//printf("\n\nRepresentation: %d\n\n", representation);
}

void revertSubsampling440(IplImage* image, uchar* imageData, int* imageSize){
    int countY = 0; //iterate through Y data
    int countCbCr = 0; //iterate through Cb and Cr data
    for(int row = 0; row < image->height; row++){
        uchar* ptr = (uchar*)(image->imageData + row * image->widthStep);
        //in an odd row, we need to copy values from the last row
        //We achieve this by returning image->width positions in imageData.
        if(row % 2 != 0){
            countCbCr -= image->width;
        }
        for(int col = 0; col < image->width; col++){
            ptr[3*col]   = imageData[countY+imageSize[0]*imageSize[1]]; //Y
            ptr[3*col+1] = imageData[countCbCr]; //Cb
            ptr[3*col+2] = imageData[countCbCr+((imageSize[0]*imageSize[1])/2)]; //Cr
            countY++;
            countCbCr++;
        }
    }
}

int extractSubsampling(uchar metadata){
    int result = metadata << 3;
    return (result & 128);
}

void convert2RGB(IplImage* image, uchar* imageData, uchar metadata, int* imageSize){
    //find out which chrominance subsampling was used
    int subsampling = extractSubsampling(metadata);

    //revert chrominance subsampling
    printf("Reverting image to 4:4:4... ");
    switch(subsampling){
        case 0://4:4:0
            revertSubsampling440(image, imageData, imageSize);
            break;
        case 128://4:2:2
            //TODO
            printf("\n\nRevert subsampling 4:2:2... TBD\n\n");
        break;
    }
    printf("OK!\n");

    //YCbCr to RGB conversion
    printf("Converting from YCbCr to RGB color space... ");
    cvCvtColor(image, image, CV_YCrCb2BGR);
    printf("OK!\n");

//---- UNIT TEST
//printf("\n\nSubsampling type: %d\n\n", subsampling);
}

uchar* buildComplementaryAnaglyph(uchar* cit, uchar* Y, int imageSize){
    printf("Rebuilding the complementary anaglyph... ");
    uchar* data = (uchar*) malloc(sizeof(uchar)*2*imageSize);
    for(int i=0; i<imageSize; i++){
        data[i] = cit[i];
        data[i+imageSize] = Y[i];
    }
    printf("OK!\n");
    return data;
}

uchar* extractY(uchar* anaglyph,int imageSize){
    printf("Extracting Y data from main anaglyph... ");
    uchar* data = (uchar*) malloc(sizeof(uchar)*imageSize);
    for(int i=0; i<imageSize; i++){
        data[i] = anaglyph[i+imageSize];
    }
    printf("OK!\n");
    return data;
}

void rebuildAnaglyph(IplImage* mainAnaglyph, IplImage* complAnaglyph, uchar* anaglyph, uchar* cit, uchar metadata, int* imageSize){
    int size = imageSize[0]*imageSize[1]; //width*height

    //extract Y from main anaglyph
    uchar* Y = extractY(anaglyph, size);

    //build complementary anaglyph
    uchar* complementary = buildComplementaryAnaglyph(cit, Y, size);

    //revert to 4:4:4 RGB
    convert2RGB(mainAnaglyph, anaglyph, metadata, imageSize);
    convert2RGB(complAnaglyph, complementary, metadata, imageSize);

    free(Y);
    free(complementary);

//-----UNIT TEST
/*cvSaveImage("main.bmp", mainAnaglyph);
cvSaveImage("compl.bmp", complAnaglyph);*/
}

void readPixelData(char* filename, uchar** anaglyph, uchar** cit, int* imageSize){
    //open file
    FILE *fp = fopen(filename,"rb");
    if(fp == NULL){
        printf("ERROR!\n\tError opening file %s", filename);
        exit(-1);
    }

    //set pointer to the position where pixel data starts (13th byte of the file)
    fseek(fp, 13, SEEK_SET);

    //read main anaglyph data
    int size = imageSize[0]*imageSize[1];
    uchar* mainData = (uchar*)malloc(2*size*sizeof(uchar));
    fread(mainData, sizeof(uchar), 2*size, fp);
    *anaglyph = mainData;

    //read Color Index Table data
    uchar* citData = (uchar*)malloc(size*sizeof(uchar));
    fread(citData, sizeof(uchar), size, fp);
    *cit = citData;

    fclose(fp);
}

void readMetadata(char* filename, uchar *metadata, int* imageSize){
    //open file
    FILE *fp = fopen(filename,"rb");
    if(fp == NULL){
        printf("ERROR!\n\tError opening file %s", filename);
        exit(-1);
    }

    //read data regarding anaglyph type, stereo pair disposition and subsampling type
    *metadata = fgetc(fp);

    //read the size of anaglyph (width, height and pixel depth)
    int imgSize[3] = {0};
    fread(imgSize, sizeof(int), 3, fp);
    imageSize[0] = imgSize[0];
    imageSize[1] = imgSize[1];
    imageSize[2] = imgSize[2];

    fclose(fp);

//----- UNIT TEST
//printf("\n**readMetadada**\n IMAGE SIZE: width - %d | height - %d | pixel depth - %d\n\n", imageSize[0], imageSize[1], imageSize[2]);
}

void anaglyphReversion(char* filename){
    //load and separate data from the compressed file
    uchar metadata = NULL;
    int imageSize[3] = {0};
    printf("Reading compressed file... ");
    readMetadata(filename, &metadata, imageSize);
    uchar* anaglyph = (uchar*)malloc(2*sizeof(uchar)*imageSize[0]*imageSize[1]);
    uchar* cit = (uchar*)malloc(sizeof(uchar)*imageSize[0]*imageSize[1]);
    readPixelData(filename, &anaglyph, &cit, imageSize);
    printf("OK!\n");

    //rebuild the complementary and main anaglyphs
    IplImage* mainAnaglyph = cvCreateImage(cvSize(imageSize[0], imageSize[1]), imageSize[2], 3);
    IplImage* complAnaglyph = cvCreateImage(cvSize(imageSize[0], imageSize[1]), imageSize[2], 3);
    cvZero(mainAnaglyph);
    cvZero(complAnaglyph);
    rebuildAnaglyph(mainAnaglyph, complAnaglyph, anaglyph, cit, metadata, imageSize);

    //recreate the stereo pair
    rebuildStereoPair(mainAnaglyph, complAnaglyph, metadata, filename);

    cvReleaseImage(&mainAnaglyph);
    cvReleaseImage(&complAnaglyph);
    free(anaglyph);
    free(cit);

//---- UNIT TEST
//printf("\nCONTROL DATA: %d\n\n", metadata);
//printf("\nIMAGE SIZE: width - %d | height - %d | pixel depth - %d\n\n", imageSize[0], imageSize[1], imageSize[2]);
//FILE* fileA = fopen("Anaglyph.dat", "wb");
//fwrite(anaglyph, sizeof(uchar), 2*imageSize, fileA);
//fclose(fileA);
//fileA = fopen("cit.dat", "wb");
//fwrite(cit, sizeof(uchar), imageSize, fileA);
//fclose(fileA);
}
/* --- ANAGLYPH REVERSION FUNCTIONS --- */

void printHelp(){
    printf("Usage:\n\n");
    printf("Encode:\n\tRevGlyph -e <image.bmp> {-sbs | -ab} {-gm | -rc | -by} {-422 | -440}\n\n");
    printf("Decode:\n\tRevGlyph -d <anaglyph.dat> <cit.dat>\n\n");
    printf("Parameters:\n -e: encode\n -d: decode\n -sbs: side-by-side\n -ab: above-below\n -gm: green-magenta\n -rc: red-cyan\n -by: blue-yellow");
}

void verifyParameters(int argc, char* argv[]){
    //verify number of parameters
    if(argc != 6 && argc != 3){
        printf("ERROR!\n\tInvalid number of parameters.\n");
        printHelp();
        exit(-1);
    }
    //verify parameters for encoding
    if(argc == 6){
        if(strcmp(argv[1],"-e")){
            printf("ERROR!\nInvalid parameter for codification '%s'.\n", argv[1]);
            printHelp();
            exit(-1);
        }
        if(strcmp(argv[3],"-sbs") && strcmp(argv[3],"-ab")){
            printf("ERROR!\nInvalid parameter '%s'.\n", argv[3]);
            printHelp();
            exit(-1);
        }
        if(strcmp(argv[4],"-gm") && strcmp(argv[4],"-rc") && strcmp(argv[4],"-by")){
            printf("ERROR!\nInvalid anaglyph type '%s'.\n", argv[4]);
            printHelp();
            exit(-1);
        }
        if(strcmp(argv[5],"-422") && strcmp(argv[5],"-440")){
            printf("ERROR!\nInvalid subsampling type '%s'.\n", argv[5]);
            printHelp();
            exit(-1);
        }
    }
    else{//verify parameter for decoding
        if(strcmp(argv[1],"-d")){
            printf("ERROR!\nInvalid parameter for decodification '%s'.\n", argv[1]);
            printHelp();
            exit(-1);
        }
    }
}

int main(int argc, char* argv[]){
    //verify parameters
    verifyParameters(argc, argv);

    if(argc == 6){//anaglyph conversion
        printf("--- ANAGLYPH CONVERSION  ---\n");
        anaglyphConversion(argv);
        printf("--- ANAGLYPH CONVERSION SUCCESSFULLY COMPLETED! ---\n");
    }
    else if (argc == 3){//anaglyph reversion
        printf("--- ANAGLYPH REVERSION  ---\n");
        anaglyphReversion(argv[2]);
        printf("--- ANAGLYPH REVERSION SUCCESSFULLY COMPLETED! ---\n");
    }
    return 0;
}
