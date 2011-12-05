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
        RevGlyph -d <anaglyph.dat> <cit.dat>

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

void createAnaglyph(IplImage* left,IplImage* right, char* type);
IplImage* loadAndVerify(char* image, char* type);
void prepareStereoImages(IplImage* stereoPair, char* type, IplImage** left,IplImage** right);
void splitImage(IplImage* stereoPair, char* type, IplImage** left, IplImage** right);
void anaglyphConversion(char* parameters[]);
void printHelp();
void verifyParameters(int argc, char* argv[]);

/*
  Create an anaglyph, given two images. The anaglyph to be created can be green-magenta,
  red-cyan or blue-yellow, according to the type entered
  Input: left - left image of the stereo pair
         right - right image of the stereo pair
         type - type of the anaglyph to be created (green-magenta, red-cyan or blue-yellow)
*/
void createAnaglyph(IplImage* left,IplImage* right, char* type){
    //create anaglyph container
    CvSize size = cvGetSize(left);
    IplImage* anaglyph = cvCreateImage(size, left->depth, left->nChannels);
    cvZero(anaglyph);

    if(!strcmp(type,"-gm")){//green-magenta anaglyph
        printf("Creating green-magenta anaglyph... ");
        for(int row = 0; row < left->height; row++){
            uchar* ptrR = (uchar*)(right->imageData + row * right->widthStep);
            uchar* ptrL = (uchar*)(left->imageData + row * left->widthStep);
            uchar* ptrA = (uchar*)(anaglyph->imageData + row * anaglyph->widthStep);
            for(int col = 0; col < left->width; col++){
                //copy green channel from the right image and red and blue channel from the left image
                ptrA[3*col+1] = ptrR[3*col+1]; //G
                ptrA[3*col] = ptrL[3*col]; //B
                ptrA[3*col+2] = ptrL[3*col+2]; //R
            }
        }
        printf("OK!\n");
    }
    else if(!strcmp(type,"-rc")){//red-cyan anaglyph
        printf("Creating red-cyan anaglyph... ");
        for(int row = 0; row < left->height; row++){
            uchar* ptrR = (uchar*)(right->imageData + row * right->widthStep);
            uchar* ptrL = (uchar*)(left->imageData + row * left->widthStep);
            uchar* ptrA = (uchar*)(anaglyph->imageData + row * anaglyph->widthStep);
            for(int col = 0; col < left->width; col++){
                //copy red channel from the right image and green and blue channel from the left image
                ptrA[3*col+2] = ptrR[3*col+2]; //R
                ptrA[3*col] = ptrL[3*col]; //B
                ptrA[3*col+1] = ptrL[3*col+1]; //G
            }
        }
        printf("OK!\n");
    }
    else if(!strcmp(type,"-by")){//blue-yellow anaglyph
        printf("Creating blue-yellow anaglyph... ");
        for(int row = 0; row < left->height; row++){
            uchar* ptrR = (uchar*)(right->imageData + row * right->widthStep);
            uchar* ptrL = (uchar*)(left->imageData + row * left->widthStep);
            uchar* ptrA = (uchar*)(anaglyph->imageData + row * anaglyph->widthStep);
            for(int col = 0; col < left->width; col++){
                //copy blue channel from the right image and red and green channel from the left image
                ptrA[3*col] = ptrR[3*col]; //B
                ptrA[3*col+1] = ptrL[3*col+1]; //G
                ptrA[3*col+2] = ptrL[3*col+2]; //R
            }
        }
        printf("OK!\n");
    }

    //compression


//---- UNIT TEST
    //cvSaveImage("anaglyph.bmp", anaglyph);

    cvReleaseImage(&anaglyph);
}

/*
  Open a stereo pair file and verify its dimensions, based on its type. If it is
  side-by-side, the image must be divisible by 2 on its width. If it is above-below,
  the image must be divisible by 2 on its heigth.
  Input: image - stereo pair
         type - side-by-side or above-below
*/
IplImage* loadAndVerify(char* image, char* type){
    //load image file
    printf("Loading original image... ");
    IplImage* stereoPair = cvLoadImage(image, 1);
    if(!stereoPair){
       printf("ERROR!\n\tError opening BMP file %s.", image);
       exit(-1);
    }

    //verify image size
    if(!strcmp(type,"-sbs") && stereoPair->width % 2 != 0){
          printf("ERROR!\n\tImage width is not divisible by 2. Please, resize it!");
          exit(-1);
    }
    if(!strcmp(type,"-ab") && stereoPair->height % 2 != 0){
          printf("ERROR!\n\tImage height is not divisible by 2. Please, resize it!");
          exit(-1);
    }
    printf("OK!\n");
    return(stereoPair);
}

/*
  Given an stereo pair, copy and adapt its properties to the two images
  Input: stereoPair - original image
         type - side-by-side or above-below
         left - left image container
         right - right image container
*/
void prepareStereoImages(IplImage* stereoPair, char* type, IplImage** left,IplImage** right){
    //get size of the stereo pair, based on its type
    CvSize size;
    if(!strcmp(type, "-sbs")){
        size = cvSize( stereoPair->width/2, stereoPair->height);
    }
    else if(!strcmp(type, "-ab")){
        size = cvSize( stereoPair->width, stereoPair->height/2);
    }

    //copy image properties
    *left = cvCreateImage(size, stereoPair->depth, stereoPair->nChannels);
    *right = cvCreateImage(size, stereoPair->depth, stereoPair->nChannels);
    cvZero(*left);
    cvZero(*right);
}

/*
  Separates a stereo pair image into two images. The original image can be either
  side-by-side or above-below. If it is side-by-side, splits image by its width.
  If its above-below, splits image by its height.
  Input: stereoPair - original image
         type - side-by-side or above-below
         left - left image container
         right - right image container
*/
void splitImage(IplImage* stereoPair, char* type, IplImage** left, IplImage** right){
    printf("Dividing the stereo pair into two images... ");
    if(!strcmp(type,"-sbs")){
        //set ROI to copy the first half of the image width (left image)
        cvSetImageROI(stereoPair, cvRect(0,0, stereoPair->width/2, stereoPair->height));
        cvCopy(stereoPair, *left);
        cvResetImageROI(stereoPair);

        //set ROI to copy the second half of the image width (right image)
        cvSetImageROI(stereoPair, cvRect(stereoPair->width/2, 0, stereoPair->width/2, stereoPair->height));
        cvCopy(stereoPair, *right);
        cvResetImageROI(stereoPair);
    }
    else if(!strcmp(type, "-ab")){
        //set ROI to copy the first half of the image height (left image)
        cvSetImageROI(stereoPair, cvRect(0,0, stereoPair->width, stereoPair->height/2));
        cvCopy(stereoPair, *left);
        cvResetImageROI(stereoPair);

        //set ROI to copy the second half of the image height (right image)
        cvSetImageROI(stereoPair, cvRect(0, stereoPair->height/2, stereoPair->width, stereoPair->height/2));
        cvCopy(stereoPair, *right);
        cvResetImageROI(stereoPair);
    }
    printf("OK!\n");
}

/*
  Given a stereo image, creates an anaglyph and stores the colors not used in the
  color index table file.
  Input: parameters - the parameters entered by user via command line
*/
void anaglyphConversion(char* parameters[]){
    IplImage *stereopair = NULL;
    IplImage *left = NULL;
    IplImage *right = NULL;

    //load de stereo pair
    stereopair = loadAndVerify(parameters[2],parameters[3]);

    //prepare stereo images with properties from the original image
    prepareStereoImages(stereopair, parameters[3], &left, &right);

    //split the stereo pair
    splitImage(stereopair, parameters[3], &left, &right);

    //create the anaglyph
    createAnaglyph(left, right, parameters[4]);

    //create the color index table

//------UNIT TEST
//cvSaveImage("left.bmp", left);
//cvSaveImage("right.bmp", right);

    cvReleaseImage(&stereopair);
    cvReleaseImage(&left);
    cvReleaseImage(&right);
    /*//green-magenta anaglyph
    CvSize size = cvGetSize(frameL);
    anaglyph = cvCreateImage(size, frameL->depth, frameL->nChannels);
    cvZero(anaglyph);

    //complementary anaglyph
    complement = cvCreateImage(size, frameL->depth, frameL->nChannels);
    cvZero(complement);

    //anaglyph transformation
    printf("Creating green magenta and complementary anaglyphs... ");
    for(int row = 0; row < frameL->height; row++){
        //set pointer to the correct position in each row
        uchar* ptrR = (uchar*)(frameR->imageData + row * frameR->widthStep);
        uchar* ptrL = (uchar*)(frameL->imageData + row * frameL->widthStep);
        uchar* ptrA = (uchar*)(anaglyph->imageData + row * anaglyph->widthStep);
        uchar* ptrC = (uchar*)(complement->imageData + row * complement->widthStep);
        for(int col = 0; col < frameL->width; col++){
            //green-magenta
            //copy green channel from the right image and red and blue channel from the left image
            ptrA[3*col+1] = ptrR[3*col+1]; //G
            ptrA[3*col] = ptrL[3*col]; //B
            ptrA[3*col+2] = ptrL[3*col+2]; //R
            //complement
            //copy red and blue channel from the right image and green channel from the left image
            ptrC[3*col+1] = ptrL[3*col+1]; //G
            ptrC[3*col] = ptrR[3*col]; //B
            ptrC[3*col+2] = ptrR[3*col+2]; //R
        }
    }
    printf("OK!\n");

    //debug anaglyphs RGB
    //cvSaveImage("green-magenta.bmp", anaglyph);
    //cvSaveImage("complementary.bmp", complement);

    //SUBSAMPLING COMPLEMENTARY ANAGLYPH AND CREATING COLOR INDEX TABLE
    printf("Complementary anaglyph subsampling\n");
    int imageSize = complement->width * complement->height;
    uchar *cit = (uchar*) malloc(imageSize*sizeof(uchar));
    cit = (uchar*)subsampling422(complement, WITHOUT_Y);

    //Saving CIT file
    char* fileNoExtension = strtok(file,".");
    char* newFile = (char*) malloc(sizeof(char)*(strlen(fileNoExtension)+18));
    strcpy(newFile, fileNoExtension);
    strcat(newFile,"-complementary.dat");
    FILE *fp = fopen(newFile,"wb");
    if(!fp){
        printf(" ERROR!\nError creating CIT file %s.\n", newFile);
        exit(-1);
    }
    fwrite(cit, sizeof(uchar),imageSize, fp);
    fclose(fp);
    free(cit);
    free(newFile);
    cvReleaseImage(&complement);
    //SUBSAMPLING MAIN ANAGLYPH
    printf("Main anaglyph subsampling\n");
    uchar *subsampleData = (uchar*)malloc(2*imageSize*sizeof(uchar));
    subsampleData = (uchar*)subsampling422(anaglyph,WITH_Y);
    //save junctioned images
    char* fileAnaglyph = (char*) malloc(sizeof(char)*(strlen(fileNoExtension)+9));
    strcpy(fileAnaglyph, fileNoExtension);
    strcat(fileAnaglyph,"-main.dat");
    fp = fopen(fileAnaglyph, "wb");
    if(!fp){
        printf(" ERROR!\nError creating green-magenta anaglyph file %s\n", fileAnaglyph);
        exit(-1);
    }
    fwrite(subsampleData, sizeof(uchar),2*imageSize, fp);
    fclose(fp);
    free(subsampleData);
    free(fileAnaglyph);
    cvReleaseImage(&anaglyph);*/
}

/* Prints usage method */
void printHelp(){
    printf("Usage:\n\n");
    printf("Encode:\n\tRevGlyph -e <image.bmp> {-sbs | -ab} {-gm | -rc | -by} {-422 | -440}\n\n");
    printf("Decode:\n\tRevGlyph -d <anaglyph.dat> <cit.dat>\n\n");
    printf("Parameters:\n -e: encode\n -d: decode\n -sbs: side-by-side\n -ab: above-below\n -gm: green-magenta\n -rc: red-cyan\n -by: blue-yellow");
}

/*
  Verifies if the parameters inputted by user are correct and valid.
  Input: argc - the number of parameteres entered by user via command line
         argv[] - the parameters entered by user via command line
*/
void verifyParameters(int argc, char* argv[]){
    //verify number of parameters
    if(argc != 6 && argc != 4){
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
    else if (argc == 4){//anaglyph reversion
        printf("--- ANAGLYPH REVERSION  ---\n");
        printf("--- ANAGLYPH REVERSION SUCCESSFULLY COMPLETED! ---\n");
    }
    return 0;
}
