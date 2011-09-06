//Programa para testar o PSNR ao realizar a conversão RGB - YCbCr - RGB
//lendo manualmente os dados das imagens, realizando a conversão, subamostragem 4:4:0
//armazenando em arquivo float
//retornando novamente para RGB e escrevendo em arquivo (round e cast para unsigned char).

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
    FILE *entrada, *saida, *aux;

    entrada = fopen(argv[1], "rb");
    saida = fopen("saida.bmp", "wb");
    aux = fopen("data.dat", "wb");

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
        //YCbCr c/ level shift -> float (-128 a 127)
        float **R =  NULL;
        float **G = NULL;
        float **B = NULL;
        float **Y =  NULL;
        float **Cb = NULL;
        float **Cr = NULL;
        R = (float**)malloc(header.biHeight*sizeof(float*));
        G = (float**)malloc(header.biHeight*sizeof(float*));
        B = (float**)malloc(header.biHeight*sizeof(float*));
        Y = (float**)malloc(header.biHeight*sizeof(float*));
        Cb = (float**)malloc(header.biHeight*sizeof(float*));
        Cr = (float**)malloc(header.biHeight*sizeof(float*));
        for(int i = 0; i < header.biHeight; i++){
            R[i] = (float*)malloc(header.biWidth*sizeof(float));
            G[i] = (float*)malloc(header.biWidth*sizeof(float));
            B[i] = (float*)malloc(header.biWidth*sizeof(float));
            Y[i] = (float*)malloc(header.biWidth*sizeof(float));
            Cb[i] = (float*)malloc(header.biWidth*sizeof(float));
            Cr[i] = (float*)malloc(header.biWidth*sizeof(float));
        }

        //conversões RGB -> YCbCr -> RGB
        int shift = 128;

        //RGB -> YCbCr
        for (int i = 0; i < header.biHeight; i++) {
            for (int j = 0; j < header.biWidth; j++) {
                B[i][j] = fgetc(entrada);
                G[i][j] = fgetc(entrada);
                R[i][j] = fgetc(entrada);

                //conversao para YCbCr c/ level shift
                float a,b,c;
                a = (0.299f * (R[i][j] - shift) + 0.587f * (G[i][j] - shift) + 0.114f * (B[i][j] - shift));
                b = (0.565f * ((B[i][j] - shift) - a));
                c = (0.713f * ((R[i][j] - shift) - a));

                Y[i][j] = a;
                Cb[i][j] = b;
                Cr[i][j] = c;
            }
        }

        //impressão de valores para verificar os resultados após conversão/subamostragem
        /*printf("RGB Lido:\n");
        for (int i = 0; i < header.biHeight; i++) {
            for (int j = 0; j < header.biWidth; j++) {
                printf("%f %f %f\n", R[i][j],G[i][j],B[i][j]);
            }
        }
        printf("YCbCr calculado - antes da subamostragem\n");
        for (int i = 0; i < header.biHeight; i++) {
            for (int j = 0; j < header.biWidth; j++) {
                printf("%f %f %f\n", Y[i][j],Cb[i][j],Cr[i][j]);
            }
        }*/

        //subamostragem 4:4:0
        int imageSize = header.biHeight*header.biWidth;
        int countY = 0;
        int countCbCr = 0;
        float *subData = (float*) malloc(2*imageSize*sizeof(float));
        for (int i = 0; i < header.biHeight; i++) {
            if(i % 2 == 0){ //copia valores Y Cb e Cr das linhas pares
                for (int j = 0; j < header.biWidth; j++) {
                    subData[countCbCr]              = Cb[i][j];
                    subData[countCbCr+imageSize/2]  = Cr[i][j];
                    subData[countY+imageSize]       = Y[i][j];
                    countY++;
                    countCbCr++;
                }
            }
            else{
                //nas linhas ímpares, copia Y, volta width posições nas regiões de Cb
                //e Cr portions do vertor subData e calcula a média dos valores já obtidos
                //com os valores de Cb e Cr desta linha
                countCbCr -= header.biWidth;
                for(int j = 0; j < header.biWidth; j++){
                    subData[countCbCr]              = (subData[countCbCr]+Cb[i][j])/2;
                    subData[countCbCr+imageSize/2]  = (subData[countCbCr+imageSize/2] + Cr[i][j])/2;
                    subData[countY+imageSize]       = Y[i][j];
                    countY++;
                    countCbCr++;
                }
            }
        }

        //escrita no arquivo de saída
        fwrite(subData, sizeof(float),2*imageSize, aux);
        fclose(aux);

        //YCbCr -> RGB
        aux = fopen("data.dat","rb");
        float *dataStreamOut = (float*)malloc(2*header.biHeight*header.biWidth*sizeof(float));
        fread(dataStreamOut,sizeof(float),2*imageSize,aux);
        //reversão para 4:4:4
        countY = 0;
        countCbCr = 0;
        for (int i = 0; i < header.biHeight; i++) {
            if(i % 2 != 0){
                countCbCr -= header.biWidth;
            }
            for (int j = 0; j < header.biWidth; j++) {
                //main
                Y[i][j]  = dataStreamOut[countY+imageSize];
                Cb[i][j] = dataStreamOut[countCbCr];
                Cr[i][j] = dataStreamOut[countCbCr+(imageSize/2)];
                countY++;
                countCbCr++;
            }
        }
        for (int i = 0; i < header.biHeight; i++) {
            for (int j = 0; j < header.biWidth; j++) {
                float d, e, f;
                d = Y[i][j] + 1.403f * Cr[i][j] + shift;
                e = Y[i][j] - 0.344f * Cb[i][j] - 0.714f * Cr[i][j] + shift;
                f = Y[i][j] + 1.77f * Cb[i][j] + shift;

                //eliminando dados fora do range 0-255
                if(d < 0) d = 0;
                if(d > 255) d = 255;
                if(e < 0) e = 0;
                if(e > 255) e = 255;
                if(f < 0) f = 0;
                if(f > 255) f = 255;

                R[i][j] = round(d);
                G[i][j] = round(e);
                B[i][j] = round(f);

                fputc((unsigned char)B[i][j], saida);
                fputc((unsigned char)G[i][j], saida);
                fputc((unsigned char)R[i][j], saida);
            }
        }

        //impressão de valores para verificar os resultados após conversão/subamostragem
        /*printf("YCbCr calculado - após subamostragem\n");
        for (int i = 0; i < header.biHeight; i++) {
            for (int j = 0; j < header.biWidth; j++) {
                printf("%f %f %f\n", round(Y[i][j]),round(Cb[i][j]),round(Cr[i][j]));
            }
        }
        printf("RBG Convertido:\n");
        for (int i = 0; i < header.biHeight; i++) {
            for (int j = 0; j < header.biWidth; j++) {
                printf("%f %f %f\n", R[i][j],G[i][j],B[i][j]);
            }
        }*/


        for(int i = 0; i < header.biHeight; i++){
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
        free(Cr);
        free(subData);
        free(dataStreamOut);
    }//else BMP
    fclose(entrada);
    fclose(saida);
    fclose(aux);
}
