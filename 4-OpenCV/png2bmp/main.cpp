/*
    Developed by: Matheus Ricardo Uihara Zingarelli
    Creation date: Mar 13th 2012

    This code creates a BMP copy of a given PNG image.
    It performs a batch process: given a directory, it will create a BMP copy of
    every PNG image inside that directory and subdirectories.

    Usage:
        png2bmp <directory_of_images>

    Changelog:


*/
#include <stdio.h>
#include <dirent.h>
#include <cv.h>
#include <highgui.h>

#define LOGFILE "logger.txt"
#define MESSAGELENGTH 500
#define FILENAMELENGTH 100

/*
  Create a BMP copy of the given PNG file

*/
void convert2BMP(char* filename, char* path){
    //copy full file name path
    //TODO: don't know why the .png is not present in the filename
    char pngImage[FILENAMELENGTH] = "";
    char bmpImage[FILENAMELENGTH] = "BMP\\";
    //png
    strcpy(pngImage,path);
    strcat(pngImage,"\\");
    strcat(pngImage,filename);
    strcat(pngImage,".png");
    //bmp
    strcat(bmpImage,filename);
    strcat(bmpImage,".bmp");

    IplImage *PNG = NULL;
    PNG = cvLoadImage(pngImage, 1);
    cvSaveImage(bmpImage,PNG);
}

/*
  Creates a new log entry in a txt file
  Input: message - message to be logged
*/
void logger(char message[]){
    FILE *fp;
    fp = fopen(LOGFILE,"a");
    if(fp == NULL){
        printf("ERROR!\n\tError opening file %s", LOGFILE);
        exit(-1);
    }
    fputs(message,fp);
    fclose(fp);
}

/*
  Verifies if a given image is PNG. Tt is just verified if the file
  has a .bmp extension
  Input:  filename - name of the file
  Output: 1 if it is PNG, 0 otherwise.
*/
int verifyFile(char* filename){
    char* verifier = strtok(filename,".");
    while(verifier!=NULL){
        if(!strcmp(verifier,"png")){
            return 1;
        }
        else{
            verifier = strtok(NULL,".");
        }
    }
    return 0;
}

/*
  Given a directory name, verify if it is valid and then process all the
  PNG images inside it and any other subdirectory. BMP images are saved in
  a BMP directory
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
            //verify if the file is a PNG image
            if(verifyFile(file->d_name)){
                char message[MESSAGELENGTH] = "------------\nCriando copia BMP da imagem: ";
                strcat(message,file->d_name);
                strcat(message,"\n");
                logger(message);
                convert2BMP(file->d_name, dirname);
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
        logger(message);
    }
}

/* Prints usage method */
void printHelp(){
    printf("Usage:\n\n");
    printf("png2bmp.exe <directory_of_images>\n\n");
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
