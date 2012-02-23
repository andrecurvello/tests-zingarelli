/*
    Developed by: Matheus Ricardo Uihara Zingarelli
    Creation date: Feb 23rd 2012
    Last modification: Feb 23rd 2012

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
  Verifies if a file is a valid image. It must be BMP images
  It is just verified if the file has a .bmp extension
  Input:  filename - name of the file
  Output: 1 if it is a valid image, 0 otherwise.
*/
int verifyFile(char* filename){
    char* verifier = strtok(filename,".");
    while(verifier!=NULL){
        if(!strcmp(verifier,"bmp")){
            return 1;
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
uchar* subsampling440(IplImage* image){
    //YCbCr conversion
    cvCvtColor(image, image, CV_BGR2YCrCb);

    //placeholder for subsampled data
    int imageSize = image->width * image->height;
    uchar *dataStream = (uchar*) malloc(2*imageSize*sizeof(uchar));

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
    return dataStream;
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
    IplImage *original = cvLoadImage(image, 1);
    if(!original){
        char message[MESSAGELENGTH] = "\t***Ocorreu um erro ao abrir a imagem.\n";
        subsampleLog(message);
        return;
    }

    //apply subsampling
    subsampleLog("\tAplicando subamostragem 4:4:0...\n");
    uchar* subData = (uchar*) malloc(2*original->width*original->height*sizeof(uchar));
    subData = subsampling440(original);

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

    IplImage *original = cvLoadImage(image, 1);
    IplImage *processed = cvLoadImage(newImage,1);
    if(!original || !processed){
        char message[MESSAGELENGTH] = "\t***Ocorreu um erro ao abrir as imagens.\n";
        PSNRLog(message);
    }

    //apply PSNR
    double Y = 0.0;
    double Cb = 0.0;
    double Cr = 0.0;
    PSNRLog("\tCalculando o PSNR...\n");
    PSNR(original, processed, 255, &Y, &Cb, &Cr, "-yuv");
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

int main(int argc, char* argv[]){
    //simple parameter number verification
    if(argc != 2){
        printHelp();
        exit(-1);
    }

    //open directory
    DIR *dir;
    struct dirent *file;

    dir = opendir(argv[1]);
    if(dir != NULL){
        while((file = readdir(dir))){
            //verify if the file is an image
            if(verifyFile(file->d_name)){
                char message[MESSAGELENGTH] = "------------\nProcessando imagem: ";
                strcat(message,file->d_name);
                strcat(message,"\n");
                subsampleLog(message);
                processSubsampling(file->d_name, argv[1]);

                strcpy(message, "++++++++++++\nCalculando PSNR da imagem: ");
                strcat(message,file->d_name);
                strcat(message,"\n");
                PSNRLog(message);
                processPSNR(file->d_name, argv[1]);
            }
            else{
                //log error
                char message[MESSAGELENGTH] = "***Arquivo de imagem inválido: ";
                strcat(message,file->d_name);
                strcat(message,"\n");
                subsampleLog(message);
            }
        }
        (void)closedir(dir);
    }
    else{
        printf("Directory not found. Please give the full address of an existing directory.\n\n");
        printHelp();
    }

}
