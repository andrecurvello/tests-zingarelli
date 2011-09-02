//Programa para testar o PSNR ao realizar a conversão RGB - YCbCr - RGB
//lendo manualmente os dados das imagens, realizando a conversão, retornando novamente
//para RGB e escrevendo em arquivo.

//O PSNR não passou de 50dB

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

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
    FILE *entrada, *saida;

    entrada = fopen(argv[1], "rb");
    saida = fopen("saida.bmp", "wb");

    if (entrada == NULL) {
        fprintf(stderr, "Nao foi possivel abrir o arquivo %s\n ", argv[1]);
        exit(EXIT_FAILURE);
    } else {
        //leitura do header do BMP, escrita do mesmo em arquivo de saída
        fread(&bmpnum, 2, 1, entrada);
        fwrite(&bmpnum, 2, 1, saida);
        fread(&fHeader, sizeof(BMPFILEHEADER), 1, entrada);
        fwrite(&fHeader, sizeof(BMPFILEHEADER), 1, saida);
        // verifica se o arquivo eh um bitmap
        if (bmpnum.bfType != 19778) {
            fprintf(stderr, "Arquivo nao eh um bitmap.\n");
            exit(EXIT_FAILURE);
        }
        fread(&header, sizeof (BMPINFOHEADER), 1, entrada);
        fwrite(&header, sizeof(BMPINFOHEADER), 1, saida);

        //estruturas para leitura dos dados
        //RGB -> unsigned char (0-255)
        //YCbCr c/ level shift -> char (-128 a 127)
        unsigned char **R =  NULL;
        unsigned char **G = NULL;
        unsigned char **B = NULL;
        char **Y =  NULL;
        char **Cb = NULL;
        char **Cr = NULL;
        R = (unsigned char**)malloc(header.biHeight*sizeof(unsigned char*));
        G = (unsigned char**)malloc(header.biHeight*sizeof(unsigned char*));
        B = (unsigned char**)malloc(header.biHeight*sizeof(unsigned char*));
        Y = (char**)malloc(header.biHeight*sizeof(char*));
        Cb = (char**)malloc(header.biHeight*sizeof(char*));
        Cr = (char**)malloc(header.biHeight*sizeof(char*));        
        for(int i = 0; i < header.biWidth; i++){
            R[i] = (unsigned char*)malloc(header.biWidth*sizeof(unsigned char));
            G[i] = (unsigned char*)malloc(header.biWidth*sizeof(unsigned char));
            B[i] = (unsigned char*)malloc(header.biWidth*sizeof(unsigned char));
            Y[i] = (char*)malloc(header.biWidth*sizeof(char));
            Cb[i] = (char*)malloc(header.biWidth*sizeof(char));
            Cr[i] = (char*)malloc(header.biWidth*sizeof(char));
        }
        
        //leitura dos dados RGB e início das conversões
        int shift = 128;

        for (int i = 0; i < header.biHeight; i++) {
            for (int j = 0; j < header.biWidth; j++) {
                R[i][j] = (unsigned char)fgetc(entrada);
                G[i][j] = (unsigned char)fgetc(entrada);
                B[i][j] = (unsigned char)fgetc(entrada);    
                
                //conversao para YCbCr c/ level shift
                double a,b,c;
                a = (0.299f * (R[i][j] - shift) + 0.587f * (G[i][j] - shift) + 0.114f * (B[i][j] - shift));
                b = (0.565f * ((B[i][j] - shift) - a));
                c = (0.713f * ((R[i][j] - shift) - a));                
                Y[i][j] = (char)a;
                Cb[i][j] = (char)b;
                Cr[i][j] = (char)c;
                
                //reversão para RGB
                double d, e, f;
                d = Y[i][j] + 1.403f * Cr[i][j] + 128;
                e = Y[i][j] - 0.344f * Cb[i][j] - 0.714f * Cr[i][j] + 128;
                f = Y[i][j] + 1.77f * Cb[i][j] + 128;
                
                //eliminando dados fora do range 0-255
                if(d < 0) d = 0;
                if(d > 255) d = 255;
                if(e < 0) e = 0;
                if(e > 255) e = 255;
                if(f < 0) f = 0;
                if(f > 255) f = 255;
                
                R[i][j] = (unsigned char)(d);
                G[i][j] = (unsigned char)(e);
                B[i][j] = (unsigned char)(f);
                 
                //escrita no arquivo de saída                                
                fputc((int)R[i][j], saida);
                fputc((int)G[i][j], saida);
                fputc((int)B[i][j], saida);     
            }
        }
    }    
    fclose(entrada);
    fclose(saida);
    //TODO:liberar ponteiros
}
