/*
  Open an image and save a copy using only ANSI C

  Developed by: Matheus Ricardo Uihara Zingarelli
  Creation date: November, 17th 2011
  Last modification: November, 17th 2011

  Usage:
        openSave <image.bmp>
*/

#include <stdio.h>
#include <stdlib.h>

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

int main(int argc, char* argv[]){
    BMPMAGICNUMBER bmpnum;
    BMPFILEHEADER fHeader;
    BMPINFOHEADER header;
    FILE *in, *out;
    int i;

    in = fopen(argv[1], "rb");

    if (in == NULL) {
        fprintf(stderr, "Failed to open bmp file %s\n ", argv[1]);
        return(-1);
    }

    //read BMP header
    fread(&bmpnum, 2, 1, in);
    fread(&fHeader, sizeof(BMPFILEHEADER), 1, in);

    //verify if it is BMP
    if (bmpnum.bfType != 19778) {
        fprintf(stderr, "Not a bitmap file.\n");
        return(-1);
    }
    fread(&header, sizeof (BMPINFOHEADER), 1, in);

    //copy header information to a new file
    out = fopen("new-image-ANSIC.bmp", "wb");
    fwrite(&bmpnum, 2, 1, out);
    fwrite(&fHeader, sizeof(BMPFILEHEADER), 1, out);
    fwrite(&header, sizeof(BMPINFOHEADER), 1, out);

    //memory allocation
    char *pixelData =  NULL;
    pixelData = (char*)malloc(header.biHeight*sizeof(char)*header.biWidth*3); //RGB data

    //reading pixel data (char)
    for (i = 0; i < 3 * header.biHeight * header.biWidth; i++) {
        pixelData[i] = fgetc(in);
    }
    fclose(in);

    //saving data in a new file
    fwrite(pixelData, sizeof(char),3*header.biHeight*header.biWidth, out);
    fclose(out);

    free(pixelData);
    printf("New image created!\n");
    return 0;
}
