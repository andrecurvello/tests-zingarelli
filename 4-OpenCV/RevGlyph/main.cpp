/*
  Developed by: Matheus Ricardo Uihara Zingarelli
  Creation date: November, 28th 2011
  Last modification: January, 24th 2012

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
#include "reversion.h"

/* --- GENERAL PURPOSE FUNCTIONS --- */
/* Prints usage method */
void printHelp();

/*
  Verifies if the parameters inputted by user are correct and valid.
  Input: argc - the number of parameteres entered by user via command line
         argv[] - the parameters entered by user via command line
*/
void verifyParameters(int argc, char* argv[]);

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

//Uncomment this if you only want to get only the anaglyphs - Diario-Mestrado.docx (Seção 8)
/*int main(int argc, char* argv[]){
    IplImage *stereopair = NULL;
    IplImage *left = NULL;
    IplImage *right = NULL;
    IplImage *mainAnaglyph = NULL;
    IplImage *complAnaglyph = NULL;

    //load the stereo pair
    stereopair = loadAndVerify(argv[2],argv[3]);

    //prepare images with properties from the original stereo image
    prepareImages(stereopair, argv[3], &left, &right, &mainAnaglyph, &complAnaglyph);

    //split the stereo pair
    splitImage(stereopair, argv[3], &left, &right);

    //create the anaglyphs
    createAnaglyph(left, right, mainAnaglyph, complAnaglyph, argv[4]);

    //save anaglyphs
    cvSaveImage("main-anaglyph.bmp", mainAnaglyph);
    cvSaveImage("complementary-anaglyph.bmp", complAnaglyph);
    cvSaveImage("left.bmp", left);
    cvSaveImage("right.bmp", right);

    cvReleaseImage(&stereopair);
    cvReleaseImage(&left);
    cvReleaseImage(&right);
    cvReleaseImage(&mainAnaglyph);
    cvReleaseImage(&complAnaglyph);
}*/
