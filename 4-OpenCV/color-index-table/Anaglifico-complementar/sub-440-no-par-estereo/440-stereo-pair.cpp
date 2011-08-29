/* 
  Developed by: Matheus Ricardo Uihara Zingarelli
  Creation date: August, 3rd 2011
  Last modification: August, 11th 2011
  
  Anaglyph conversion / reversion using YCbCr color space with 4:4:0 subsampling
  on both images of the stereo pair.
  
  Usage:
        440-stereo-pair <image.bmp> {-sbs | -ab} {-e | -d}
        
        -sbs: side-by-side image
        -ab: above/below image
        -e: encoding
        -d: decoding
*/

#include <stdio.h>
#include <cv.h>
#include <highgui.h>

#define WITHOUT_Y 0
#define WITH_Y 1

/*
  Apply 4:4:0 subsampling on an image.
  Input: 
        image - image source to be subsampled
        keepY - boolean variable to keep or not the luminance (Y) component
  Output: data subsampled
*/
char *subsampling440(IplImage *image, int keepY){
    //YCbCr conversion
    printf("\tConverting from RGB to YCbCr color space... ");
    //cvCvtColor(image, image, CV_BGR2YCrCb);
    char **Y =  NULL;
    char **Cb = NULL;
    char **Cr = NULL;
    Y =  (char**)malloc(image->height*sizeof(char*));
    Cb = (char**)malloc(image->height*sizeof(char*));
    Cr = (char**)malloc(image->height*sizeof(char*));
    for(int i = 0; i < image->width; i++){
        Y[i] =  (char*)malloc(image->width*sizeof(char));
        Cb[i] = (char*)malloc(image->width*sizeof(char));
        Cr[i] = (char*)malloc(image->width*sizeof(char));
    }
    int shift = 128;
    for(int row = 0; row < image->height; row++){
        //set pointer to the correct position in each row
        uchar* ptr = (uchar*)(image->imageData + row * image->widthStep);
        for(int col = 0; col < image->width; col++){
            float luminance = (0.299f * (ptr[3*col+2] - shift) + 0.587f * (ptr[3*col+1] - shift) + 0.114f * (ptr[3*col] - shift));
            Y[row][col] = (char)luminance;
            Cb[row][col] = (char)(0.565f * (ptr[3*col] - shift) - luminance);
            Cr[row][col] = (char)(0.713f * (ptr[3*col+2] - shift) - luminance);            
        }
    }
    printf("OK!\n");
    
    //container for subsampled data    
    int imageSize = image->width * image->height;
    /* 
      subData variable may or may not hold data the luminance component. For a normal
      image, we would have 3 matrices, each holding data from one of the components
      (one for Y, one for Cb and one for Cr) and each with a lenght of width*height
      pixels, thus needing to store 3*imageSize pixels. If the Y is not needed
      (for the color index table), this reduces into storing 2*imageSize pixels.
      And since we're applying a 4:4:0 subsampling, we're reducing data from Cb 
      and Cr to a half, thus needing to store 1/2*imageSize pixels for Cb and 1/2*imageSize
      pixels for Cr, which means we only need to store data for 1*imageSize pixels or
      2*imageSize pixels, depending on if we need to keep or not the Y component
    */
    char *subData = NULL;
    //subData iterators
    int countY = 0;
    int countCbCr = 0;
    
    printf("\tSubsampling 4:4:0... ");
    if(keepY){ //main anaglyph subsampling
        subData = (char*) malloc(2*imageSize*sizeof(char));
        for(int row = 0; row < image->height; row++){          
            if(row % 2 == 0){
                //copy Y, Cb and Cr values of even rows
                for(int col = 0; col < image->width; col++){
                    subData[countCbCr] =                    Cb[row][col]; //Cb
                    subData[countCbCr+imageSize/2] =        Cr[row][col]; //Cr
                    subData[countY+imageSize] =             Y[row][col];   //Y
                    countY++;
                    countCbCr++;                    
                }//column iteration
            }//if even rows
            else{
                //copy Y, backtrack width positions on Cb and Cr portions of CIT and 
                //calculate the average values including Cr and Cr of odd rows
                countCbCr -= image->width;
                for(int col = 0; col < image->width; col++){
                    subData[countCbCr] =                    (subData[countCbCr]+Cb[row][col])/2; //Cb
                    subData[countCbCr+imageSize/2] =        (subData[countCbCr+imageSize/2] + Cr[row][col])/2; //Cr
                    subData[countY+imageSize] =             Y[row][col]; //Y
                    countY++;
                    countCbCr++;
                }//column iteration
            }//if odd rows
        }//row iteration
    }//if sampling Y
    else{ //complementary anaglyph subsampling
        subData = (char*) malloc(imageSize*sizeof(char));
        for(int row = 0; row < image->height; row++){
            if(row % 2 == 0){
                for(int col = 0; col < image->width; col++){
                    subData[countCbCr]                   = Cb[row][col]; //Cb
                    subData[countCbCr+(imageSize/2)]     = Cr[row][col]; //Cr
                    countCbCr++;
                }//column iteration
            }//if even row
            //backtrack width positions on cit and calculate the average values
            //including Cr and Cr of odd rows
            else{
                countCbCr -= image->width;
                for(int col = 0; col < image->width; col++){
                    subData[countCbCr]                   = (subData[countCbCr]+ Cb[row][col])/2; //Cb
                    subData[countCbCr+(imageSize/2)]     = (subData[countCbCr+(imageSize/2)] + Cr[row][col])/2; //Cr
                    countCbCr++;
                }//column iteration
            }//if odd row
        }//row iteration
    }//if not sampling Y
    printf("OK\nLiberando ponteiros... ");    
    //TODO: verificar por que o free n�o funfa...
    /*for(int i = 0; i < image->width; i++){
        free(Y[i]);
        //free(Cb[i]);
        //free(Cr[i]);        
    }
    free(Y);
    free(Cb);
    free(Cr);    */
    printf("OK\n");  
    return subData;    
}

/*
  Apply 4:2:2 subsampling on an image (TODO: needs to be tested)
  Input: 
        image - image source to be subsampled
  Output: data subsampled
*/
//YCbCr conversion
uchar *subsampling422(IplImage * image){
    printf("Converting from RGB to YCbCr color space... ");
    cvCvtColor(image, image, CV_BGR2YCrCb);
    printf("OK!\n");
    
    //container for color index table data
    int imageSize = image->width * image->height;
    uchar *cit = (uchar*) malloc(imageSize*sizeof(uchar));    
    
    printf("Subsampling 4:2:2... ");
    int count = 0;
    for(int row = 0; row < image->height; row++){
        //set pointer to the correct position in each row
        uchar* ptrAn = (uchar*)(image->imageData + row * image->widthStep);
        //subsampling Cr and Cb
        int k = 0; //will control the subsampling
        for(int col = 0; col < (image->width)/2; col++){
            cit[count]                   = (ptrAn[3*(col+k)+1] + ptrAn[3*(col+k+1)+1])/2; //Cb
            cit[count+(imageSize/2)]     = (ptrAn[3*(col+k)+2] + ptrAn[3*(col+k+1)+2])/2; //Cr
            count++;
            k++;
        }
    }
}

/*
  Get an anaglyph image and create the stereo pair, using the color index table
  to get the missing color components from the anaglyph image.
  Input: anaglyph - anaglyph image
         imageType - -sbs to create a side-by-side image or -ab to create a above-below one
*/
void reverseAnaglyph(char *file, char *imageType){
    IplImage *stereoPair, *anaglyph, *complement, *original;
    
    //TODO: LOAD FROM A SINGLE DAT FILE
    //load anaglyph image
    original = cvLoadImage(file, 1);
    
    //new image will have double width or double height, depending on the image type
    CvSize size;
    if(!strcmp(imageType,"-sbs")){
        size = cvSize(original->width, original->height);
    }
    else if(!strcmp(imageType, "-ab")){
        size = cvSize(original->width/2, original->height*2);
    }
    
    //We will create a RGB 24bits image
    stereoPair = cvCreateImage(size, original->depth, original->nChannels);
    cvZero(stereoPair);
    
    //complementary and main anaglyphs
    size = cvSize(original->width/2, original->height);
    complement = cvCreateImage(size, original->depth, original->nChannels);
    cvZero(complement);
    anaglyph = cvCreateImage(size, original->depth, original->nChannels);
    cvZero(anaglyph);
        
    //Load main anaglyph
    printf("Loading main anaglyph... ");
    int imageSize = original->width/2 * original->height;
    char* fileNoExtension = strtok(file,".");
    char* fileAnaglyph = (char*) malloc(sizeof(char)*(strlen(fileNoExtension)+9));
    strcpy(fileAnaglyph, fileNoExtension);
    strcat(fileAnaglyph, "-main.dat");
    FILE *fp = fopen(fileAnaglyph, "rb");
    if(!fp){
        printf(" ERROR!\nError opening main anaglyph file %s.\n", fileAnaglyph);
        exit(-1);
    }    
    char *mainAnaglyphData = (char*) malloc(sizeof(char)*2*imageSize);
    fread(mainAnaglyphData, sizeof(char),2*imageSize,fp);
    fclose(fp);
    free(fileAnaglyph);
    printf("OK!\n");
    
    //Load CIT
    printf("Loading Color Index Table... ");
    char* newFile = (char*) malloc(sizeof(char)*(strlen(fileNoExtension)+18));
    strcpy(newFile, fileNoExtension);
    strcat(newFile,"-complementary.dat");
    fp = fopen(newFile, "rb");
    if(!fp){
        printf(" ERROR!\nError opening CIT file %s.\n", newFile);
        exit(-1);
    }    
    char *cit = (char*) malloc(imageSize*sizeof(char));
    fread(cit, sizeof(char), imageSize, fp);
    fclose(fp);
    free(newFile);
    printf("OK!\n");
    
    //Copy Y from main anaglyph
    printf("Copying luminance data from main anaglyph... ");
    int count = 0;    
    char *Y = (char*) malloc(sizeof(char)*imageSize);
    for(int i = 0; i < imageSize; i++){
        Y[count] = mainAnaglyphData[count+imageSize];            
        count++;
    }
    printf("OK!\n");
    
    //recreate complement, type 4:4:4
    printf("Recreating anaglyphs to subsampling type 4:4:4... ");
    int countY = 0; //iterate through Y data   
    int countCbCr = 0; //iterate through Cb and Cr data
    for(int row = 0; row < complement->height; row++){
        //set pointer to the correct position in each row
        uchar* ptrC = (uchar*)(complement->imageData + row * complement->widthStep); 
        uchar* ptrM = (uchar*)(anaglyph->imageData + row * anaglyph->widthStep);              
        //return to type 4:4:4
        //when in an odd row, we need to copy values from the last row due to 
        //4:4:0 subsampling. We achieve this by returning complement->width
        //positions in k's value.
        if(row % 2 != 0){
            countCbCr -= complement->width;
        }     
        for(int col = 0; col < complement->width; col++){
            //complementary
            ptrC[3*col]   = Y[countY]+128; //Y
            ptrC[3*col+1] = cit[countCbCr]+128; //Cb
            ptrC[3*col+2] = cit[countCbCr+(imageSize/2)]+128; //Cr
            //main
            ptrM[3*col]   = mainAnaglyphData[countY+imageSize]+128; //Y
            ptrM[3*col+1] = mainAnaglyphData[countCbCr]+128; //Cb
            ptrM[3*col+2] = mainAnaglyphData[countCbCr+(imageSize/2)]+128; //Cr
            countY++;
            countCbCr++;
        }
    }
    printf("OK!\n");
        
    //convert from YCbCr to RGB color space
    //TODO: converter na m�o
    //cvCvtColor(complement, complement, CV_YCrCb2BGR);
    //cvCvtColor(anaglyph, anaglyph, CV_YCrCb2BGR);
    
    //debug anaglyphs RGB
    //cvSaveImage("complement-reversed.bmp", complement);
    //cvSaveImage("green-magenta-reversed.bmp", anaglyph);
    
    //Reverting..
    //TODO: above-below
    printf("Reverting anaglyph image to stereo pair... ");
    if(!strcmp(imageType, "-sbs")){    
        for(int row = 0; row < anaglyph->height; row++){
            //set pointer to the correct position in each row
            uchar* ptrStp = (uchar*)(stereoPair->imageData + row * stereoPair->widthStep);                
            uchar* ptrAn = (uchar*)(anaglyph->imageData + row * anaglyph->widthStep);                
            uchar* ptrAc = (uchar*)(complement->imageData + row * complement->widthStep);                
            //return to type 4:4:4
            for(int col = 0; col < anaglyph->width; col++){
                ptrStp[3*col] = ptrAn[3*col]; //B left
                ptrStp[3*(col+anaglyph->width)] = ptrAc[3*col];; //B right
                ptrStp[3*col+1] = ptrAc[3*col+1]; //G left
                ptrStp[3*col+2] = ptrAn[3*col+2]; //R left
                ptrStp[3*(col+anaglyph->width)+1] = ptrAn[3*col+1]; //G right
                ptrStp[3*(col+anaglyph->width)+2] = ptrAc[3*col+2]; //R right
            }
        }
    }
    /* Above/Below
    else if(!strcmp(imageType, "-ab")){
        for(int row = 0; row < frame->height; row++){
             //set pointer to the correct position in each row
             uchar* ptrAn = (uchar*)(frame->imageData + row * frame->widthStep);
             uchar* ptrStp = (uchar*)(stereoPair->imageData + row * stereoPair->widthStep);                
             for(int col = 0; col < frame->width; col++){
                 // Copy values from frame (R and B channels) and vet (G 
                 // channel) to stereoPair for the first half of image height 
                 ptrStp[3*col] = ptrAn[3*col];
                 ptrStp[3*col+1] = vet[count];
                 ptrStp[3*col+2] = ptrAn[3*col+2];
                 
                 count++;
             }
        }
        count = 0;
        for(int row = 0; row < frame->height; row++){
             //set pointer to the correct position in each row
             uchar* ptrAn = (uchar*)(frame->imageData + row * frame->widthStep);
             uchar* ptrStp = (uchar*)(stereoPair->imageData + (row + frame->height) * stereoPair->widthStep);
             for(int col = 0; col < frame->width; col++){
                 // Copy values from frame (G channel) and vet (R and B 
                 // channels) to stereoPair for the rest of image height
                 ptrStp[3*col] = vet[count+(2*imageSize)]; //blue
                 ptrStp[3*col+2] = vet[count+imageSize]; //red
                 ptrStp[3*col+1] = ptrAn[3*col+1];
                 
                 count++;
             }
        }
    }*/
    
    //save reversed image     
    char* reversed = (char*) malloc(sizeof(char)*(strlen(fileNoExtension)+13));
    strcpy(reversed, fileNoExtension);
    strcat(reversed,"-reversed.bmp");
    cvSaveImage(reversed, stereoPair);
    printf("OK!\n");       
    cvReleaseImage(&stereoPair);
    cvReleaseImage(&anaglyph);
    cvReleaseImage(&complement);
    cvReleaseImage(&original);
    free(Y);
    free(reversed);
    free(cit);
}

/*
  Get the stereo pair and create the green-magenta anaglyph.
  Green-magenta anaglyph is saved as YCbCr 4:4:0
  Complementary anaglyph is converted to YCbCr 4:4:0 but only Cb and Cr are stored
  in the Color Index Table
  Input: file - filename of original image
         frameL, frameR - store left and right image, respectively
*/
void createAnaglyph(char *file, IplImage *frameL, IplImage *frameR){
    IplImage *anaglyph, *complement;
    
    //green-magenta anaglyph
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
    char *cit = (char*) malloc(imageSize*sizeof(char));     
    cit = (char*)subsampling440(complement, WITHOUT_Y);  
              
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
    fwrite(cit, sizeof(char),imageSize, fp);    
    fclose(fp);
    free(cit);    
    free(newFile);
    cvReleaseImage(&complement);
    //TODO: PUT ALL DATA IN A SINGLE FILE
    //SUBSAMPLING MAIN ANAGLYPH
    printf("Main anaglyph subsampling\n");
    char *subsampleData = (char*)malloc(2*imageSize*sizeof(char));
    subsampleData = (char*)subsampling440(anaglyph,WITH_Y);
    
    printf("\nDONE\n");
    //save junctioned image
    char* fileAnaglyph = (char*) malloc(sizeof(char)*(strlen(fileNoExtension)+9));
    strcpy(fileAnaglyph, fileNoExtension);
    strcat(fileAnaglyph,"-main.dat");
    fp = fopen(fileAnaglyph, "wb");
    if(!fp){
        printf(" ERROR!\nError creating green-magenta anaglyph file %s\n", fileAnaglyph);
        exit(-1);
    }
    fwrite(subsampleData, sizeof(char),2*imageSize, fp);
    fclose(fp);
    free(subsampleData);
    free(fileAnaglyph);
    cvReleaseImage(&anaglyph);
}

/*
  Separate images from a side-by-side or above/below image. Given the original
  image dimensions, set a ROI (region of interest) based on the image type. If
  it's side-by-side, the ROI will be half of the original image's width. If it's
  above-below, the ROI will be half of the original image's height. Only the pixels
  inside the ROI will be copied by cvCopy
  Input: frame - original image
         frameL, frameR - stores left and right image, respectively
         width, height - original image dimensions
         imageType - -sbs if the original image has side-by-side images, -ab
            if it has above-below images                
*/
void separateImages(IplImage *frame, IplImage **frameL, IplImage **frameR, int width, int height, char *imageType){    
    printf("Dividing the stereo pair into two images... ");
    if(!strcmp(imageType,"-sbs")){
        //set ROI to cut the image on the left (half of the original)
        cvSetImageROI(frame, cvRect(0,0, width/2, height));            
        //copy image portion
        cvCopy(frame, *frameL);            
        //reset ROI
        cvResetImageROI(frame);            
        //set ROI to cut the image on the right (second half of the original)
        cvSetImageROI(frame, cvRect(width/2, 0, width/2, height));            
        //copy image portion
        cvCopy(frame, *frameR);            
        //reset ROI
        cvResetImageROI(frame);
    }
    else if(!strcmp(imageType, "-ab")){
        //set ROI to cut the image on the top (half of the original)
        cvSetImageROI(frame, cvRect(0,0, width, height/2));            
        //copy image portion
        cvCopy(frame, *frameL);            
        //reset ROI
        cvResetImageROI(frame);            
        //set ROI to cut the image on the bottom (second half of the original)
        cvSetImageROI(frame, cvRect(0, height/2, width, height/2));            
        //copy image portion
        cvCopy(frame, *frameR);            
        //reset ROI
        cvResetImageROI(frame);
    }  
    printf("OK!\n");
}

/* Prints help message to user */
void printHelp(){
    printf("Program execution: 440-stereo-pair <BMP_file> {-sbs | -ab} {-e | -d}\n");
    printf("\n-sbs:side-by-side or -ab:above/below");
    printf("\n-e:encode (anaglyph conversion) or -d:decode (anaglyph to stereo pair reversion)");
}

int main(int argc, char* argv[]){
    if(argc < 4){
        printHelp();
        exit(-1);  
    }
    
    char *imageType = argv[2]; //-sbs:sid-by-side or -ab:above/below
    
    // ----- Anaglyph conversion
    //TODO: simplify code with only one function call
    if(!strcmp(argv[3],"-e")){    
        printf("--- GREEN-MAGENTA ANAGLYPH CONVERSION  ---\n");
        IplImage *frame, *frameL, *frameR;        
        //load image
        //images are loaded by openCV with color channels in this order: BGR
        frame = cvLoadImage(argv[1], 1);
        
        //TEST PSNR FROM OPENCV CVT COLOR TO AND FROM RGB YCBCR
        /*CvSize Size = cvSize( frame->width, frame->height);
        IplImage *RGBtoYCbCr = cvCreateImage(Size, frame->depth, frame->nChannels);
        IplImage *YCbCrtoRGB = cvCreateImage(Size, frame->depth, frame->nChannels);
        cvCvtColor(frame, RGBtoYCbCr, CV_BGR2YCrCb);
        cvCvtColor(RGBtoYCbCr, YCbCrtoRGB, CV_YCrCb2BGR);
        cvSaveImage("RGB-YCbCr-RGB.bmp",YCbCrtoRGB);
        cvReleaseImage(&RGBtoYCbCr);
        cvReleaseImage(&YCbCrtoRGB);*/
        
        //Simple error handling
        if(!frame){
           printf("Error opening BMP file.");
           exit(-1);
        }            
        //some verifications regarding the image
        if(frame->width % 2 != 0){
              printf("Image width is not divisible by 2. Please, resize it!");
              exit(-1);
        }            
        int width = frame->width;
        int height = frame->height;      
            
        //get size of the two images from the stereo pair, based on its type
        CvSize size;
        if(!strcmp(imageType, "-sbs")){
            size = cvSize( width/2, height);
        }
        else if(!strcmp(imageType, "-ab")){
            size = cvSize( width, height/2);
        }
        else{
            printHelp();
            exit(-1);                    
        }
        
        //copy image properties
        frameL = cvCreateImage(size, frame->depth, frame->nChannels);
        frameR = cvCreateImage(size, frame->depth, frame->nChannels);
        cvZero(frameL);
        cvZero(frameR);
        
        //divide the stereo pair into two images
        separateImages(frame, &frameL, &frameR, width, height, imageType);
                
        //create anaglyph
        createAnaglyph(argv[1],frameL, frameR);  
                
        cvReleaseImage(&frame);
        cvReleaseImage(&frameL);
        cvReleaseImage(&frameR);
        printf("--- ANAGLYPH CONVERSION SUCCESSFULLY COMPLETED! ---\n");
    }
    
    // ----- Anaglyph reversion
    else if(!strcmp(argv[3],"-d")){
        printf("--- ANAGLYPH TO STEREO PAIR REVERSION ---\n");
        reverseAnaglyph(argv[1], imageType);
        printf("--- ANAGLYPH REVERSION SUCCESSFULLY COMPLETED! ---\n");
    }
    else{
        printHelp();
        exit(-1);      
    }
    
    return 0;
}
