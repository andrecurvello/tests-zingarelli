/*
    Developed by: Matheus Ricardo Uihara Zingarelli
    Creation date: Feb 23rd 2012
    Last modification: Mar 13th 2012

    Code created to evaluate how much objective quality is lost when applying
    chrominance sampling to an image. The first chrominance sampling to be tested
    will be 4:4:0.
    Given a directory of images, it is applied 4:4:0 subsampling and saved a
    new image. Then it is calculated the YCbCr PSNR. PSNR values are stored in
    a CSV file called PSNR.csv.
    ATTENTION: due to some naming problems, file names must NOT have a dot, only
    the one from the file extesion (e.g.: image.house.bmp will crash the system).

    Usage:
        subsampling.exe <directory_of_images>

    Changelog:
    (03/03/12) - modified function subsampling440() to solve a memory leak problem
    Nothing is allocated inside the function now, everything is passed as a parameter
    (12/03/12)
      - added recursion to process subdirectories.
      - modified so PNG files can also be loaded and saved as bmp
    (13/03/12)
      - removed functionality to converts PNG to BMP. It was created another program for it.

*/

#include <stdio.h>
#include <dirent.h>
#include <cv.h>
#include <highgui.h>
#include "psnr.h"

#define SUBLOG "subamostragem.txt"
#define PSNRLOG "psnr-log.txt"
#define PSNRCSV "psnr.csv"
#define MESSAGELENGTH 500
#define FILENAMELENGTH 100

/* Prints usage method */
void printHelp(){
    printf("Usage:\n\n");
    printf("subsampling.exe <directory_of_images>\n\n");
}

/*
  Verifies if a file is a valid image. It must be BMP or PNG images
  It is just verified if the file has a .bmp extension
  Input:  filename - name of the file
  Output: 1 if it is a BMP, 2 if it is a PNG, 0 otherwise.
*/
int verifyFile(char* filename){
    char* verifier = strtok(filename,".");
    while(verifier!=NULL){
        if(!strcmp(verifier,"bmp")){
            return 1;
        }
        else if(!strcmp(verifier,"png")){
            return 2;
        }
        else{
            verifier = strtok(NULL,".");
        }
    }
    return 0;
}

/*
  Creates a new log entry for the subsampling process.
*/
void subsampleLog(char message[]){
    FILE *fp;
    fp = fopen(SUBLOG,"a");
    if(fp == NULL){
        printf("ERROR!\n\tError opening file %s", SUBLOG);
        exit(-1);
    }
    fputs(message,fp);
    fclose(fp);
}

/*
  Creates a new log entry for the PSNR process.
*/
void PSNRLog(char message[]){
    FILE *fp;
    fp = fopen(PSNRLOG,"a");
    if(fp == NULL){
        printf("ERROR!\n\tError opening file %s", SUBLOG);
        exit(-1);
    }
    fputs(message,fp);
    fclose(fp);
}

/*
  Converts an RGB image to YCbCr and then apply 4:4:0 subsampling. An image is represented by three matrices,
  each holding data from one of the components (one for Y, one for Cb and one for Cr) and each with a lenght
  of width*height pixels, thus needing to store 3*imageSize pixels for a 4:4:4 image. Since we're applying
  4:4:0 subsampling, we're reducing data from Cb and Cr to a half, thus needing to store 1/2*imageSize pixels
  for Cb and 1/2*imageSize pixels for Cr, which means we only need to store data for 2*imageSize pixels.
  Input: image - image to be subsampled
  Output: uchar datastream, with data in the following order: (subsampled)Cb, (subsampled)Cr and Y
*/
void subsampling440(IplImage* image, uchar* dataStream){
    //YCbCr conversion
    cvCvtColor(image, image, CV_BGR2YCrCb);

    //placeholder for subsampled data
    int imageSize = image->width * image->height;

    //4:4:0 works through copying the average color values from even and odd rows
    //this way, it is a horizontal spatial reduction
    int countCbCr = 0;
    int countY = 0;
    for(int row = 0; row < image->height; row++){
        uchar* ptr = (uchar*)(image->imageData + row * image->widthStep);
        if(row % 2 == 0){
            //copy Y, Cb and Cr values of even rows
            for(int col = 0; col < image->width; col++){
                dataStream[countCbCr] =             ptr[3*col+1]; //Cb
                dataStream[countCbCr+imageSize/2] = ptr[3*col+2]; //Cr
                dataStream[countY+imageSize] =      ptr[3*col];   //Y
                countY++;
                countCbCr++;
            }//column iteration
        }//if even rows
        else{
            //copy Y, backtrack width positions on dataStream (that holds Cb and Cr data)
            //and calculate the average values
            countCbCr -= image->width;
            for(int col = 0; col < image->width; col++){
                dataStream[countCbCr] =             (dataStream[countCbCr]+ptr[3*col+1])/2; //Cb
                dataStream[countCbCr+imageSize/2] = (dataStream[countCbCr+imageSize/2] + ptr[3*col+2])/2; //Cr
                dataStream[countY+imageSize] =      ptr[3*col]; //Y
                countY++;
                countCbCr++;
            }//column iteration
        }//if odd rows
    }//row iteration
}

/*
  Revert data from a subsampled image to a 4:4:4 image. For the reversion,
  we will simply copy the same value of Cb and Cr for two consecutive rows
  Input: image - container for the new image with the reverted subsampled data
         imageData - subsampled image data from original image
*/
void revertSubsampling440(IplImage* image, uchar* imageData){
    int countY = 0; //iterate through Y data
    int countCbCr = 0; //iterate through Cb and Cr data
    int imageSize = image->width*image->height;
    for(int row = 0; row < image->height; row++){
        uchar* ptr = (uchar*)(image->imageData + row * image->widthStep);
        //in an odd row, we need to copy values from the last row
        //We achieve this by returning image->width positions in imageData.
        if(row % 2 != 0){
            countCbCr -= image->width;
        }
        for(int col = 0; col < image->width; col++){
            ptr[3*col]   = imageData[countY+imageSize]; //Y
            ptr[3*col+1] = imageData[countCbCr]; //Cb
            ptr[3*col+2] = imageData[countCbCr+(imageSize/2)]; //Cr
            countY++;
            countCbCr++;
        }
    }

    //YCbCr to RGB conversion
    cvCvtColor(image, image, CV_YCrCb2BGR);
}

/*
  Prepare image for the subsampling and resampling process, creating
  a new subsampled image.
  Input: filename - name of the image to be processed
         path - physical path of the directory
         type - 1 means it is a BMP file, 2 means it is a PNG file
*/
void processSubsampling(char* filename, char* path){
    //copy full file name path
    //TODO: don't know why the .bmp is not present in the filename
    char image[FILENAMELENGTH] = "";
    char newImage[FILENAMELENGTH] = "";
    strcpy(image,path);
    strcat(image,"\\");
    strcat(image,filename);
    strcpy(newImage,image);
    strcat(image,".bmp");

    //load image
    subsampleLog("\tCarregando imagem do disco...\n");
    IplImage *original = NULL;
    original = cvLoadImage(image, 1);
    if(!original){
        char message[MESSAGELENGTH] = "\t***Ocorreu um erro ao abrir a imagem.\n";
        subsampleLog(message);
        return;
    }

    //use these codes if you only want a RGB->YCbCr conversion without subsampling
    //comment "apply subsampling" and "revert to 4:4:4" and free(subdata)

    //uncomment here if you want to use the OpenCV formula
    /*cvCvtColor(original, original, CV_BGR2YCrCb);
    IplImage *processed = NULL;
    processed = cvCreateImage(cvSize(original->width,original->height),original->depth,original->nChannels);
    cvZero(processed);
    cvCvtColor(original, processed, CV_YCrCb2BGR);*/

    //RGB->YCbCR->RGB using REC.601
    /*BGR2YUV(original,original);
    IplImage *processed = cvCreateImage(cvSize(original->width,original->height),original->depth,original->nChannels);
    cvZero(processed);
    for(int row = 0; row < original->height; row++){
            uchar* ptrSrc = (uchar*)(original->imageData + row * original->widthStep);
            uchar* ptrDst = (uchar*)(processed->imageData + row * processed->widthStep);
            for(int col=0; col < original->width; col++){
                //REC.601
                double R = 1.164 * (ptrSrc[3*col] - 16) + 1.596 * (ptrSrc[3*col+2] - 128);
                double G = 1.164 * (ptrSrc[3*col] - 16) - 0.391 * (ptrSrc[3*col+1] - 128) - 0.813 * (ptrSrc[3*col+2] - 128);
                double B = 1.164 * (ptrSrc[3*col] - 16) + 2.018 * (ptrSrc[3*col+1] - 128);

                //eliminando dados fora do range 0-255
                if(R < 0) R = 0;
                if(R > 255) R = 255;
                if(G < 0) G = 0;
                if(G > 255) G = 255;
                if(B < 0) B = 0;
                if(B > 255) B = 255;

                ptrDst[3*col] = (uchar)round(B);
                ptrDst[3*col+1] = (uchar)round(G);
                ptrDst[3*col+2] = (uchar)round(R);
            }
    }*/

    //uncomment here if you want the manual and laborous way to test convertion with float
    /*int shift = 128;
    double **R =  NULL;
    double **G = NULL;
    double **B = NULL;
    double **Y =  NULL;
    double **Cb = NULL;
    double **Cr = NULL;
    R = (double**)malloc(original->height*sizeof(double*));
    G = (double**)malloc(original->height*sizeof(double*));
    B = (double**)malloc(original->height*sizeof(double*));
    Y = (double**)malloc(original->height*sizeof(double*));
    Cb = (double**)malloc(original->height*sizeof(double*));
    Cr = (double**)malloc(original->height*sizeof(double*));
    for(int i = 0; i < original->height; i++){
        R[i] = (double*)malloc(original->width*sizeof(double));
        G[i] = (double*)malloc(original->width*sizeof(double));
        B[i] = (double*)malloc(original->width*sizeof(double));
        Y[i] = (double*)malloc(original->width*sizeof(double));
        Cb[i] = (double*)malloc(original->width*sizeof(double));
        Cr[i] = (double*)malloc(original->width*sizeof(double));
    }
    //rgb->ycbcr com double
    for(int row = 0; row < original->height; row++){
        uchar* ptr = (uchar*)(original->imageData + row * original->widthStep);
        for(int col = 0; col < original->width; col++){
            //conversao para YCbCr c/ level shift
            double a,b,c;
            a = (0.299f * (ptr[3*col+2] - shift) + 0.587f * (ptr[3*col+1] - shift) + 0.114f * (ptr[3*col] - shift));
            b = (0.565f * ((ptr[3*col] - shift) - a));
            c = (0.713f * ((ptr[3*col+2] - shift) - a));

            Y[row][col] = a;
            Cb[row][col] = b;
            Cr[row][col] = c;
        }
    }
    //ycbcr->rgb com double
    IplImage *processed = cvCreateImage(cvSize(original->width,original->height),original->depth,original->nChannels);
    cvZero(processed);
    for(int row = 0; row < original->height; row++){
        uchar* ptr = (uchar*)(original->imageData + row * original->widthStep);
        uchar* ptrP = (uchar*)(processed->imageData + row * processed->widthStep);
        for(int col = 0; col < original->width; col++){
            double d, e, f;
            d = Y[row][col] + 1.403f * Cr[row][col] + shift;
            e = Y[row][col] - 0.344f * Cb[row][col] - 0.714f * Cr[row][col] + shift;
            f = Y[row][col] + 1.77f * Cb[row][col] + shift;

            //eliminando dados fora do range 0-255
            if(d < 0) d = 0;
            if(d > 255) d = 255;
            if(e < 0) e = 0;
            if(e > 255) e = 255;
            if(f < 0) f = 0;
            if(f > 255) f = 255;

            R[row][col] = round(d);
            G[row][col] = round(e);
            B[row][col] = round(f);

            ptrP[3*col] = (uchar)B[row][col] ;//B
            ptrP[3*col+1] = (uchar)G[row][col] ;//G
            ptrP[3*col+2] = (uchar)R[row][col] ;//R
        }
    }
    for(int i = 0; i < original->height; i++){
        free(R[i]);
        free(G[i]);
        free(B[i]);
        free(Y[i]);
        free(Cb[i]);
        free(Cr[i]);
    }
    free(R);
    free(G);
    free(B);
    free(Y);
    free(Cb);
    free(Cr);*/


    //apply subsampling
    subsampleLog("\tAplicando subamostragem 4:4:0...\n");
    uchar* subData = (uchar*) malloc(2*original->width*original->height*sizeof(uchar));
    subsampling440(original, subData);

    //revert to 4:4:4
    subsampleLog("\tRevertendo para 4:4:4...\n");
    IplImage *processed = cvCreateImage(cvSize(original->width,original->height),original->depth,original->nChannels);
    cvZero(processed);
    revertSubsampling440(processed,subData);

    //save processed image
    subsampleLog("\tSalvando imagem revertida...\n");
    strcat(newImage,"-subsampled.bmp");
    cvSaveImage(newImage, processed);

    char message[MESSAGELENGTH] = "Imagem ";
    strcat(message,newImage);
    strcat(message," criada com sucesso!\n------------\n");
    subsampleLog(message);

    free(subData);
    cvReleaseImage(&original);
    cvReleaseImage(&processed);
}

/*
  Calculates the YCbCr PSNR. Values are logged in a CSV file.
  Input: filename - name of the original image. The processed image filename is <original_image>-subsampled
         path - physical path of the directory
         type - 1 means it is a BMP file, 2 means it is a PNG file
*/
void processPSNR(char* filename, char* path){
    //load images
    char image[FILENAMELENGTH] = "";
    char newImage[FILENAMELENGTH] = "";
    strcpy(image,path);
    strcat(image,"\\");
    strcat(image,filename);
    strcpy(newImage,image);
    strcat(newImage,"-subsampled.bmp");
    strcat(image,".bmp");
    PSNRLog("\tCarregando imagens do disco...\n");

    //printf("\nOriginal %s\nProcessada %s\n",image,newImage);exit(0);

    IplImage *original = NULL;
    IplImage *processed = NULL;
    original = cvLoadImage(image, 1);
    processed = cvLoadImage(newImage,1);
    if(!original || !processed){
        char message[MESSAGELENGTH] = "\t***Ocorreu um erro ao abrir as imagens.\n";
        PSNRLog(message);
    }

    //apply PSNR
    double Y = 0.0;
    double Cb = 0.0;
    double Cr = 0.0;
    PSNRLog("\tCalculando o PSNR...\n");
    PSNR(original, processed, 255, &Y, &Cb, &Cr, "-ycbcr");
    double avg = (Y+Cb+Cr)/3.0;

    //save PSNR values in a CSV file
    PSNRLog("\tArmazenando os valores de PSNR obtidos...\n");
    FILE *fp = fopen(PSNRCSV,"a");
    fprintf(fp, "%s-%s,%lf,%lf,%lf,%lf\n", image, newImage, Y, Cb, Cr, avg);
    fclose(fp);
    PSNRLog("Dados armazenados com sucesso!\n++++++++++++\n");

    cvReleaseImage(&original);
    cvReleaseImage(&processed);
}

/*
  Given a directory name, verify if it is valid and then process all the valid images
  inside it. Recursively process any other directory inside it
  Input: dirname - directory name to be processed.
*/
void processDir(char* dirname){
    //open directory
    DIR *dir;
    struct dirent *file;
    dir = opendir(dirname);
    if(dir != NULL){
        //process each file inside dir
        while((file = readdir(dir))){
            //verify if the file is a BMP image
            int verifier = verifyFile(file->d_name);
            if(verifier){
                char message[MESSAGELENGTH] = "------------\nProcessando imagem: ";
                strcat(message,file->d_name);
                strcat(message,"\n");
                subsampleLog(message);
                processSubsampling(file->d_name, dirname);

                strcpy(message, "++++++++++++\nCalculando PSNR da imagem: ");
                strcat(message,file->d_name);
                strcat(message,"\n");
                PSNRLog(message);
                processPSNR(file->d_name, dirname);
            }
            else if((!strcmp(file->d_name, ".")) || (!strcmp(file->d_name, ".."))){
                //do nothing
            }
            else{
                //it may be a directory or an file with different file type, try to open it
                //attach current path
                char currentFile[FILENAMELENGTH] = ".\\";
                strcat(currentFile,dirname);
                strcat(currentFile,"\\");
                strcat(currentFile,file->d_name);
                processDir(currentFile);
            }
        }
        (void)closedir(dir);
    }
    else{
        //log error
        char message[MESSAGELENGTH] = "***Arquivo invalido: ";
        strcat(message,dirname);
        strcat(message,"\n");
        subsampleLog(message);
    }
}

int main(int argc, char* argv[]){
    //simple parameter number verification
    if(argc != 2){
        printHelp();
        exit(-1);
    }

    //process images from a given directory
    processDir(argv[1]);

}
