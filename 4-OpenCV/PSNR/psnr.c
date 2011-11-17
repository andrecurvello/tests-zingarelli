/*
  Given two images, img_A (modified) and img_B (original),
  calculates the PSNR between them

  Developed by: Matheus Ricardo Uihara Zingarelli
  Adapted from: Rodolfo Ribeiro Silva
  Creation date: November, 17th 2011
  Last modification: November, 17th 2011

  Usage:
        psnr <img_A.bmp> <img_B.bmp>
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/**** BMP file header structure ****/
typedef struct {
    unsigned short bfType;       /* Magic number for file */
} BMPMAGICNUMBER;

typedef struct {
    unsigned int   bfSize;       /* Size of file */
    unsigned short bfReserved1;  /* Reserved */
    unsigned short bfReserved2;  /* ... */
    unsigned int   bfOffBits;    /* Offset to bitmap data */
} BMPFILEHEADER;

/**** BMP file info structure ****/
typedef struct {
    unsigned int   biSize;          /* Size of info header */
    int            biWidth;         /* Width of image */
    int            biHeight;        /* Height of image */
    unsigned short biPlanes;        /* Number of color planes */
    unsigned short biBitCount;      /* Number of bits per pixel */
    unsigned int   biCompression;   /* Type of compression to use */
    unsigned int   biSizeImage;     /* Size of image data */
    int            biXPelsPerMeter; /* X pixels per meter */
    int            biYPelsPerMeter; /* Y pixels per meter */
    unsigned int   biClrUsed;       /* Number of colors used */
    unsigned int   biClrImportant;  /* Number of important colors */
} BMPINFOHEADER;

double PSNR_Matrix( char **img1_data, // changed image
                    char **img2_data, // original image
                    long width,
                    long height)
{
    long i, j;
    double error, pix, x, psnr;

    /* calculating distortion */
    error = 0.0;
    pix = (double)width*(double)height;
    for(i=0;i<height;i++)
    {
        for(j=0;j<width;j++)
        {
            x = (double)img1_data[i][j] - (double)img2_data[i][j];
            //debug
            //printf("Modified:\t%f\tOriginal:\t%f\n",(double)img1_data[i][j],(double)img2_data[i][j]);
            error += ((x * x) / pix);
        }
    }
    //debug
    //printf("\n\nError:%f\n\n", error);
    if(error == 0){ //identical images
        return(100);
    }
    psnr = 10.0 * log10((255.0*255.0)/error);

    return (psnr);
}

int main(int argc, char* argv[]){
    BMPMAGICNUMBER bmpnum;
    BMPFILEHEADER fHeader;
    BMPINFOHEADER header;
    FILE *original, *modified;
    int i, j;
    double PSNR_R, PSNR_G, PSNR_B, PSNR;

    //open Images
    printf("Loading images...\t");
    modified = fopen(argv[1], "rb");
    original = fopen(argv[2], "rb");
    if (original == NULL || modified == NULL) {
        fprintf(stderr, "Failed to open bmp file %s\n ", argv[1]);
        return(-1);
    }

    //read BMP header and verify
    fread(&bmpnum, 2, 1, original);
    fread(&fHeader, sizeof(BMPFILEHEADER), 1, original);
    if (bmpnum.bfType != 19778) {
        fprintf(stderr, "Not a bitmap file.\n");
        return(-1);
    }
    fread(&header, sizeof (BMPINFOHEADER), 1, original);
    fread(&bmpnum, 2, 1, modified);
    fread(&fHeader, sizeof(BMPFILEHEADER), 1, modified);
    if (bmpnum.bfType != 19778) {
        fprintf(stderr, "Not a bitmap file.\n");
        return(-1);
    }
    fread(&header, sizeof (BMPINFOHEADER), 1, modified);
    printf("OK\n");

    //memory allocation of each R, G and B matrix
    char **Ro =  NULL;  /*                  */
    char **Go = NULL;   /* Original Image   */
    char **Bo = NULL;   /* ---------------- */
    char **Rm =  NULL;  /*                  */
    char **Gm = NULL;   /* Modified Image   */
    char **Bm = NULL;   /*                  */
    Ro = (char**)malloc(header.biHeight*sizeof(char*));
    Go = (char**)malloc(header.biHeight*sizeof(char*));
    Bo = (char**)malloc(header.biHeight*sizeof(char*));
    Rm = (char**)malloc(header.biHeight*sizeof(char*));
    Gm = (char**)malloc(header.biHeight*sizeof(char*));
    Bm = (char**)malloc(header.biHeight*sizeof(char*));
    for(i = 0; i < header.biHeight; i++){
        Ro[i] = (char*)malloc(header.biWidth*sizeof(char));
        Go[i] = (char*)malloc(header.biWidth*sizeof(char));
        Bo[i] = (char*)malloc(header.biWidth*sizeof(char));
        Rm[i] = (char*)malloc(header.biWidth*sizeof(char));
        Gm[i] = (char*)malloc(header.biWidth*sizeof(char));
        Bm[i] = (char*)malloc(header.biWidth*sizeof(char));
    }

    //read pixel data
    printf("Reading images...\t");
    for (i = 0; i < header.biHeight; i++) {
        for (j = 0; j < header.biWidth; j++) {
            Bo[i][j] = fgetc(original);
            Go[i][j] = fgetc(original);
            Ro[i][j] = fgetc(original);
            Bm[i][j] = fgetc(modified);
            Gm[i][j] = fgetc(modified);
            Rm[i][j] = fgetc(modified);
        }
    }
    printf("OK\n");

    //Calculate PSNR
    printf("Calculating PSNR...\t");
    PSNR_R = PSNR_Matrix(Rm, Ro, header.biWidth, header.biHeight);
    PSNR_G = PSNR_Matrix(Gm, Go, header.biWidth, header.biHeight);
    PSNR_B =  PSNR_Matrix(Bm, Bo, header.biWidth, header.biHeight);
    printf("OK\n\n");
    printf("PSNR:\nRed:\t%f\nGreen:\t%f\nBlue:\t%f\nOverall:\t%f", PSNR_R, PSNR_G, PSNR_B, (PSNR_R+PSNR_G+PSNR_B)/3);

    fclose(original);
    fclose(modified);
    free(Ro);
    free(Go);
    free(Bo);
    free(Rm);
    free(Gm);
    free(Bm);
    return 0;
}
