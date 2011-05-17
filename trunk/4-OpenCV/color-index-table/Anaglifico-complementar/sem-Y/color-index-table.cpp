/* BASELINE
  Developed by: Matheus Ricardo Uihara Zingarelli
  Creation date: May, 13th 2011
  Last modification: May, 13th 2011
  
  Usage:
        color-index-table <image.bmp> {-sbs | -ab} {-e | -d}
        
        -sbs: side-by-side image
        -ab: above/below image
        -e: encoding
        -d: decoding
*/

#include <stdio.h>
#include <cv.h>
#include <highgui.h>

/*
  Get an anaglyph image and create the stereo pair, using the color index table
  to get the missing color components from the anaglyph image.
  Input: anaglyph - anaglyph image
         imageType - -sbs to create a side-by-side image or -ab to create a above-below one
*/
void revertAnaglyph(char *file, char *imageType){
    IplImage *stereoPair, *anaglyph, *anaglyphYCbCr, *complement;
    
    //load anaglyph image
    anaglyph = cvLoadImage(file, 1);
    
    //new image will have double width or double height, depending on the image type
    CvSize size;
    if(!strcmp(imageType,"-sbs")){
        size = cvSize( anaglyph->width*2, anaglyph->height);
    }
    else if(!strcmp(imageType, "-ab")){
        size = cvSize( anaglyph->width, anaglyph->height*2);
    }
    
    //We will create a RGB 24bits image
    stereoPair = cvCreateImage(size, anaglyph->depth, anaglyph->nChannels);
    cvZero(stereoPair);
    
    //image to store anaglyph in YCbCr format
    size = cvSize( anaglyph->width, anaglyph->height);
    anaglyphYCbCr = cvCreateImage(size, anaglyph->depth, anaglyph->nChannels);
    cvZero(anaglyphYCbCr);
    complement = cvCreateImage(size, anaglyph->depth, anaglyph->nChannels);
    cvZero(complement);
    
    //Convert anaglyph from RGB to YCbCr
    cvCvtColor(anaglyph, anaglyphYCbCr, CV_BGR2YCrCb);
    
    //Get luminance (Y) from anaglyph image
    printf("Calculating luminance Y from anaglyph... ");
    int imageSize = anaglyph->width * anaglyph->height;
    int count = 0;
    uchar *Y = (uchar*) malloc(sizeof(uchar)*imageSize);
    for(int row = 0; row < anaglyphYCbCr->height; row++){
        //set pointer to the correct position in each row
        uchar* ptrAn = (uchar*)(anaglyphYCbCr->imageData + row * anaglyphYCbCr->widthStep);        
        for(int col = 0; col < anaglyphYCbCr->width; col++){
            //Copy luminance values
            Y[count] = ptrAn[3*col];            
            count++;
        }
    }
    printf("OK!\n");
    
    //Load CIT
    printf("Loading Color Index Table... ");
    char* newFile = (char*) malloc(sizeof(char)*(strlen(file)));
    strcpy(newFile, file);
    newFile[strlen(file)-1] = 't';
    newFile[strlen(file)-2] = 'a';
    newFile[strlen(file)-3] = 'd';
    FILE *fp = fopen(newFile, "rb");
    if(!fp){
        printf(" ERROR!\nError opening CIT file.\n");
        exit(-1);
    }
    
    uchar *cit = (uchar*) malloc(imageSize*sizeof(uchar));
    fread(cit, sizeof(uchar), imageSize, fp);
    fclose(fp);
    free(newFile);
    printf("OK!\n");
    
    //recreate complement, type 4:4:4
    printf("Recreating complement... ");
    count = 0;
    int k = -1; //iterate through the CIT
    if(!strcmp(imageType, "-sbs")){    
        for(int row = 0; row < complement->height; row++){
            //set pointer to the correct position in each row
            uchar* ptrC = (uchar*)(complement->imageData + row * complement->widthStep);                
            //return to type 4:4:4
            for(int col = 0; col < complement->width; col++){
                ptrC[3*col] = Y[count]; //luminance left
                if(col%2 == 0){//even columns receive current Cb and Cr from CIT
                    k++;
                    ptrC[3*col+1] = cit[k]; //Cb
                    ptrC[3*col+2] = cit[k+(imageSize/2)]; //Cr              
                }
                else{//odd columns receive previous Cb and Cr from CIT
                    ptrC[3*col+1] = cit[k]; //Cb
                    ptrC[3*col+2] = cit[k+(imageSize/2)]; //Cr  
                }
                count++;
            }
        }
    }
    printf("OK!\n");
    
    //convert from YCbCr to RGB
    cvCvtColor(complement, complement, CV_YCrCb2BGR);
    //cvSaveImage("complement-reversed.bmp", complement);
    
    //Reverting..
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
    char* reversed = (char*) malloc(sizeof(char)*(strlen(file)+9));
    strcpy(reversed, file);
    int i;
    //shift file extension: .bmp
    for (i = strlen(file)+8; i > strlen(file)+8-4; i--){
        reversed[i] = reversed[i-9];
    }
    reversed[strlen(file)+8-3]  = '.';
    reversed[strlen(file)+8-4]  = 'd';
    reversed[strlen(file)+8-5]  = 'e';
    reversed[strlen(file)+8-6]  = 's';
    reversed[strlen(file)+8-7]  = 'r';
    reversed[strlen(file)+8-8]  = 'e';
    reversed[strlen(file)+8-9]  = 'v';
    reversed[strlen(file)+8-10] = 'e';
    reversed[strlen(file)+8-11] = 'r';
    reversed[strlen(file)+8-12] = '-';
    cvSaveImage(reversed, stereoPair);
    printf("Ok!\n");       
    cvReleaseImage(&stereoPair);
    cvReleaseImage(&anaglyph);
    free(Y);
    free(reversed);
    free(cit);
}

/*
  Get the stereo pair and create the green-magenta anaglyph.
  Input: file - filename of original image
         frameL, frameR - store left and right image, respectively
*/
void createAnaglyph(char *file, IplImage *frameL, IplImage *frameR){
    IplImage *anaglyph, *complement;
    
    //prepare anaglyph image
    CvSize size = cvGetSize(frameL);
    anaglyph = cvCreateImage(size, frameL->depth, frameL->nChannels);
    cvZero(anaglyph);
    
    //anaglyph complement (holds discarded components)
    complement = cvCreateImage(size, frameL->depth, frameL->nChannels);
    cvZero(complement);
    
    //anaglyph transformation
    printf("Creating anaglyph image... ");
    for(int row = 0; row < frameL->height; row++){
        //set pointer to the correct position in each row
        uchar* ptrR = (uchar*)(frameR->imageData + row * frameR->widthStep);
        uchar* ptrL = (uchar*)(frameL->imageData + row * frameL->widthStep);
        uchar* ptrA = (uchar*)(anaglyph->imageData + row * anaglyph->widthStep);
        uchar* ptrC = (uchar*)(complement->imageData + row * complement->widthStep);
        for(int col = 0; col < frameL->width; col++){
            //copy green channel from the right image and red and blue channel from the left image
            ptrA[3*col+1] = ptrR[3*col+1]; //G
            ptrA[3*col] = ptrL[3*col]; //B
            ptrA[3*col+2] = ptrL[3*col+2]; //R
            //complement
            ptrC[3*col+1] = ptrL[3*col+1]; //G
            ptrC[3*col] = ptrR[3*col]; //B
            ptrC[3*col+2] = ptrR[3*col+2]; //R
        }            
    }
    
    //save junctioned image
    char* fileAnaglyph = (char*) malloc(sizeof(char)*(strlen(file)+9));
    strcpy(fileAnaglyph, file);
    int i;
    //shift file extension: .bmp
    for (i = strlen(file)+8; i > strlen(file)+8-4; i--){
        fileAnaglyph[i] = fileAnaglyph[i-9];
    }
    fileAnaglyph[strlen(file)+8-3]  = '.';
    fileAnaglyph[strlen(file)+8-4]  = 'h';
    fileAnaglyph[strlen(file)+8-5]  = 'p';
    fileAnaglyph[strlen(file)+8-6]  = 'y';
    fileAnaglyph[strlen(file)+8-7]  = 'l';
    fileAnaglyph[strlen(file)+8-8]  = 'g';
    fileAnaglyph[strlen(file)+8-9]  = 'a';
    fileAnaglyph[strlen(file)+8-10] = 'n';
    fileAnaglyph[strlen(file)+8-11] = 'a';
    fileAnaglyph[strlen(file)+8-12] = '-';
    cvSaveImage(fileAnaglyph, anaglyph);    
    //cvSaveImage("complement.bmp", complement);
    printf("OK!\n");
    
    free(fileAnaglyph);
    cvReleaseImage(&anaglyph);
    
    //----- Color Index Table
    
    //convert complement to YCbCr subsampled
    int imageSize = frameL->width * frameL->height;
    
    //CIT will hold chrominance subsampled from complement.
    uchar *cit = (uchar*) malloc(imageSize*sizeof(uchar));
    
    //convert from BGR to YCbCr
    printf("Converting complement from RGB to YCbCr... ");
    cvCvtColor(complement, complement, CV_BGR2YCrCb);
    printf("OK!\n");
    
    //Copy Y, Cb and Cr and store it on CIT
    printf("Subsampling 4:2:2... ");
    int count = 0;
    for(int row = 0; row < complement->height; row++){
        //set pointer to the correct position in each row
        uchar* ptrAn = (uchar*)(complement->imageData + row * complement->widthStep);
        //subsampling Cr and Cb
        int k = 0; //will control the subsampling
        for(int col = 0; col < (complement->width)/2; col++){
            cit[count]                   = (ptrAn[3*(col+k)+1] + ptrAn[3*(col+k+1)+1])/2; //Cb
            cit[count+(imageSize/2)]     = (ptrAn[3*(col+k)+2] + ptrAn[3*(col+k+1)+2])/2; //Cr
            count++;
            k++;
        }
    }    
    
    //Saving CIT file
    char* newFile = (char*) malloc(sizeof(char)*(strlen(file)+9));
    strcpy(newFile, file);
    newFile[strlen(file)+8]    = 't';
    newFile[strlen(file)+8-1]  = 'a';
    newFile[strlen(file)+8-2]  = 'd';
    newFile[strlen(file)+8-3]  = '.';
    newFile[strlen(file)+8-4]  = 'h';
    newFile[strlen(file)+8-5]  = 'p';
    newFile[strlen(file)+8-6]  = 'y';
    newFile[strlen(file)+8-7]  = 'l';
    newFile[strlen(file)+8-8]  = 'g';
    newFile[strlen(file)+8-9]  = 'a';
    newFile[strlen(file)+8-10] = 'n';
    newFile[strlen(file)+8-11] = 'a';
    newFile[strlen(file)+8-12] = '-';
    FILE *fp = fopen(newFile,"wb");
    if(!fp){
        printf(" ERROR!\nError creating CIT file.\n");
        exit(-1);
    }
    fwrite(cit, sizeof(uchar),imageSize, fp);    
    fclose(fp);
    free(cit);    
    free(newFile);
    free(fileAnaglyph);
    cvReleaseImage(&complement);
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
    printf("Program execution: color-index-table <BMP_file> {-sbs | -ab} {-e | -d}\n");
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
    }
    
    // ----- Anaglyph reversion
    else if(!strcmp(argv[3],"-d")){
        printf("--- ANAGLYPH TO STEREO PAIR REVERSION ---\n");
        revertAnaglyph(argv[1], imageType);
    }
    else{
        printHelp();
        exit(-1);      
    }
    
    return 0;
}
