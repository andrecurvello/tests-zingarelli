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
  Reads a compressed file and separates data inside it. The compressed file is formed
  by metadata (1 byte with control parameters and 4 bytes with the size of the anaglyph),
  followed by the main anaglyph data, followed by the Color Index Table data.
*/
void readCompressedFile(char* filename, uchar** anaglyph, uchar** cit, uchar* metadata, int* imageSize){
    printf("Reading compressed file... ");
    //open file
    FILE *fp = fopen(filename,"rb");
    if(fp == NULL){
        printf("ERROR!\n\tError opening file %s", filename);
        exit(-1);
    }

    //read data regarding anaglyph type, stereo pair disposition and subsampling type
    *metadata = fgetc(fp);

    //read the size of anaglyph
    int imgSize[1] = {0};
    fread(imgSize, sizeof(int), 1, fp);
    *imageSize = imgSize[0];
    //read main anaglyph data

    //read Color Index Table data

    printf("OK!\n");
    fclose(fp);
}

void anaglyphReversion(char* filename){
    //load and separate data from the compressed file
    uchar* anaglyph = NULL;
    uchar* cit = NULL;
    uchar metadata = NULL;
    int imageSize = 0;
    readCompressedFile(filename, &anaglyph, &cit, &metadata, &imageSize);

    //rebuild the complementary anaglyph

    //rearrange color channels

//---- UNIT TEST
printf("\nCONTROL DATA: %d\n\n", metadata);
printf("\nIMAGE SIZE: %d\n\n", imageSize);
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
