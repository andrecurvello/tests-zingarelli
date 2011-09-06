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
        double **R =  NULL;
        double **G = NULL;
        double **B = NULL;
        double **Y =  NULL;
        double **Cb = NULL;
        double **Cr = NULL;
        R = (double**)malloc(header.biHeight*sizeof(double*));
        G = (double**)malloc(header.biHeight*sizeof(double*));
        B = (double**)malloc(header.biHeight*sizeof(double*));
        Y = (double**)malloc(header.biHeight*sizeof(double*));
        Cb = (double**)malloc(header.biHeight*sizeof(double*));
        Cr = (double**)malloc(header.biHeight*sizeof(double*));        
        for(int i = 0; i < header.biHeight; i++){
            R[i] = (double*)malloc(header.biWidth*sizeof(double));
            G[i] = (double*)malloc(header.biWidth*sizeof(double));
            B[i] = (double*)malloc(header.biWidth*sizeof(double));
            Y[i] = (double*)malloc(header.biWidth*sizeof(double));
            Cb[i] = (double*)malloc(header.biWidth*sizeof(double));
            Cr[i] = (double*)malloc(header.biWidth*sizeof(double));
        }
        
        //conversões RGB -> YCbCr -> RGB
        int shift = 128;

        /*
        |              |
        |--- DOUBLE ---|
        |              |
        */
        //RGB -> YCbCr
        for (int i = 0; i < header.biHeight; i++) {
            for (int j = 0; j < header.biWidth; j++) {
                R[i][j] = fgetc(entrada);
                G[i][j] = fgetc(entrada);
                B[i][j] = fgetc(entrada);    
                               
                //conversao para YCbCr c/ level shift
                double a,b,c;
                a = (0.299f * (R[i][j] - shift) + 0.587f * (G[i][j] - shift) + 0.114f * (B[i][j] - shift));
                b = (0.565f * ((B[i][j] - shift) - a));
                c = (0.713f * ((R[i][j] - shift) - a));                
                
                Y[i][j] = a;
                Cb[i][j] = b;
                Cr[i][j] = c;                         
            }
        }
        
        //utilizando fwrite para gravar os valores negativos
        double *dataStream = (double*)malloc(3*header.biHeight*header.biWidth*sizeof(double));
        int count = 0;
        for (int i = 0; i < header.biHeight; i++) {
            for (int j = 0; j < header.biWidth; j++) {
                dataStream[count++] = Y[i][j];
                dataStream[count++] = Cb[i][j];
                dataStream[count++] = Cr[i][j];
            }
        }        
        //escrita no arquivo de saída 
        fwrite(dataStream, sizeof(double),count, aux);        
        fclose(aux);
        
        //YCbCr -> RGB
        aux = fopen("data.dat","rb");
        double *dataStreamOut = (double*)malloc(3*header.biHeight*header.biWidth*sizeof(double));
        //lendo com fread para que seja lido os valores negativos
        fread(dataStreamOut,sizeof(double),count,aux);    
        count = 0;    
        for (int i = 0; i < header.biHeight; i++) {
            for (int j = 0; j < header.biWidth; j++) {
                Y[i][j] = dataStreamOut[count++];
                Cb[i][j] = dataStreamOut[count++];
                Cr[i][j] = dataStreamOut[count++];
                
                double d, e, f;
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
                
                fputc((unsigned char)R[i][j], saida);
                fputc((unsigned char)G[i][j], saida);
                fputc((unsigned char)B[i][j], saida);
            }
        }  
        
         /*
        |                                                  |
        |--- DOUBLE COM ROUND PARA ARMAZENAR EM ARQUIVO ---|
        | PSNR cai para ~50dB!!!                           |
        */
        //RGB -> YCbCr
        /*for (int i = 0; i < header.biHeight; i++) {
            for (int j = 0; j < header.biWidth; j++) {
                R[i][j] = fgetc(entrada);
                G[i][j] = fgetc(entrada);
                B[i][j] = fgetc(entrada);    
                               
                //conversao para YCbCr c/ level shift
                double a,b,c;
                a = (0.299f * (R[i][j] - shift) + 0.587f * (G[i][j] - shift) + 0.114f * (B[i][j] - shift));
                b = (0.565f * ((B[i][j] - shift) - a));
                c = (0.713f * ((R[i][j] - shift) - a));                
                Y[i][j] = a;
                Cb[i][j] = b;
                Cr[i][j] = c;                         
            }
        }        
        //utilizando fwrite para gravar os valores negativos e round. Isso irá diminuir o PSNR pela metade, mesmo armazenando em double!
        double *dataStream = (double*)malloc(3*header.biHeight*header.biWidth*sizeof(double));
        int count = 0;
        for (int i = 0; i < header.biHeight; i++) {
            for (int j = 0; j < header.biWidth; j++) {
                dataStream[count++] = round(Y[i][j]);
                dataStream[count++] = round(Cb[i][j]);
                dataStream[count++] = round(Cr[i][j]);
            }
        }
        fwrite(dataStream, sizeof(double),count, aux);        
        fclose(aux);
        
        //YCbCr -> RGB
        aux = fopen("data.dat","rb");
        double *dataStreamOut = (double*)malloc(3*header.biHeight*header.biWidth*sizeof(double));
        //lendo com fread para que seja lido os valores negativos
        fread(dataStreamOut,sizeof(double),count,aux);    
        count = 0;    
        for (int i = 0; i < header.biHeight; i++) {
            for (int j = 0; j < header.biWidth; j++) {
                Y[i][j] = dataStreamOut[count++];
                Cb[i][j] = dataStreamOut[count++];
                Cr[i][j] = dataStreamOut[count++];
                
                double d, e, f;
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
                
                fputc((unsigned char)R[i][j], saida);
                fputc((unsigned char)G[i][j], saida);
                fputc((unsigned char)B[i][j], saida);
            }
        }*/
        /*
        |                       |
        |---  INT COM ROUND --- |
        | PSNR cai para ~50dB!!!|
        */
        //RGB -> YCbCr
        /*for (int i = 0; i < header.biHeight; i++) {
            for (int j = 0; j < header.biWidth; j++) {
                R[i][j] = fgetc(entrada);
                G[i][j] = fgetc(entrada);
                B[i][j] = fgetc(entrada);    
                               
                //conversao para YCbCr c/ level shift
                double a,b,c;
                a = (0.299f * (R[i][j] - shift) + 0.587f * (G[i][j] - shift) + 0.114f * (B[i][j] - shift));
                b = (0.565f * ((B[i][j] - shift) - a));
                c = (0.713f * ((R[i][j] - shift) - a));                
                Y[i][j] = a;
                Cb[i][j] = b;
                Cr[i][j] = c;                         
            }
        }        
        //utilizando fwrite para gravar os valores negativos e round. Isso irá diminuir o PSNR pela metade, mesmo armazenando em double!
        int *dataStream = (int*)malloc(3*header.biHeight*header.biWidth*sizeof(int));
        int count = 0;
        for (int i = 0; i < header.biHeight; i++) {
            for (int j = 0; j < header.biWidth; j++) {
                dataStream[count++] = (int)round(Y[i][j]);
                dataStream[count++] = (int)round(Cb[i][j]);
                dataStream[count++] = (int)round(Cr[i][j]);
            }
        }
        fwrite(dataStream, sizeof(int),count, aux);        
        fclose(aux);
        
        //YCbCr -> RGB
        aux = fopen("data.dat","rb");
        int *dataStreamOut = (int*)malloc(3*header.biHeight*header.biWidth*sizeof(int));
        //lendo com fread para que seja lido os valores negativos
        fread(dataStreamOut,sizeof(int),count,aux);    
        count = 0;    
        for (int i = 0; i < header.biHeight; i++) {
            for (int j = 0; j < header.biWidth; j++) {
                Y[i][j] = dataStreamOut[count++];
                Cb[i][j] = dataStreamOut[count++];
                Cr[i][j] = dataStreamOut[count++];
                
                double d, e, f;
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
                
                fputc((unsigned char)R[i][j], saida);
                fputc((unsigned char)G[i][j], saida);
                fputc((unsigned char)B[i][j], saida);
            }
        }*/
        /*
        |                       |
        |---  INT SEM ROUND --- |
        | PSNR cai para ~46dB!!!|
        */
        //RGB -> YCbCr
        /*for (int i = 0; i < header.biHeight; i++) {
            for (int j = 0; j < header.biWidth; j++) {
                R[i][j] = fgetc(entrada);
                G[i][j] = fgetc(entrada);
                B[i][j] = fgetc(entrada);    
                               
                //conversao para YCbCr c/ level shift
                double a,b,c;
                a = (0.299f * (R[i][j] - shift) + 0.587f * (G[i][j] - shift) + 0.114f * (B[i][j] - shift));
                b = (0.565f * ((B[i][j] - shift) - a));
                c = (0.713f * ((R[i][j] - shift) - a));                
                Y[i][j] = a;
                Cb[i][j] = b;
                Cr[i][j] = c;                         
            }
        }        
        //utilizando fwrite para gravar os valores negativos e round. Isso irá diminuir o PSNR pela metade, mesmo armazenando em double!
        int *dataStream = (int*)malloc(3*header.biHeight*header.biWidth*sizeof(int));
        int count = 0;
        for (int i = 0; i < header.biHeight; i++) {
            for (int j = 0; j < header.biWidth; j++) {
                dataStream[count++] = (int)(Y[i][j]); //sem round
                dataStream[count++] = (int)(Cb[i][j]); //sem round
                dataStream[count++] = (int)(Cr[i][j]); //sem round
            }
        }
        fwrite(dataStream, sizeof(int),count, aux);        
        fclose(aux);
        
        //YCbCr -> RGB
        aux = fopen("data.dat","rb");
        int *dataStreamOut = (int*)malloc(3*header.biHeight*header.biWidth*sizeof(int));
        //lendo com fread para que seja lido os valores negativos
        fread(dataStreamOut,sizeof(int),count,aux);    
        count = 0;    
        for (int i = 0; i < header.biHeight; i++) {
            for (int j = 0; j < header.biWidth; j++) {
                Y[i][j] = dataStreamOut[count++];
                Cb[i][j] = dataStreamOut[count++];
                Cr[i][j] = dataStreamOut[count++];
                
                double d, e, f;
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
                
                fputc((unsigned char)R[i][j], saida);
                fputc((unsigned char)G[i][j], saida);
                fputc((unsigned char)B[i][j], saida);
            }
        }*/
        /*
        |                        |
        |---  CHAR COM ROUND --- |
        | PSNR cai para ~50dB!!! |
        */
        //RGB -> YCbCr
        /*for (int i = 0; i < header.biHeight; i++) {
            for (int j = 0; j < header.biWidth; j++) {
                R[i][j] = fgetc(entrada);
                G[i][j] = fgetc(entrada);
                B[i][j] = fgetc(entrada);    
                               
                //conversao para YCbCr c/ level shift
                double a,b,c;
                a = (0.299f * (R[i][j] - shift) + 0.587f * (G[i][j] - shift) + 0.114f * (B[i][j] - shift));
                b = (0.565f * ((B[i][j] - shift) - a));
                c = (0.713f * ((R[i][j] - shift) - a));                
                Y[i][j] = a;
                Cb[i][j] = b;
                Cr[i][j] = c;                         
            }
        }        
        //utilizando fwrite para gravar os valores negativos e round. Isso irá diminuir o PSNR pela metade, mesmo armazenando em double!
        char *dataStream = (char*)malloc(3*header.biHeight*header.biWidth*sizeof(char));
        int count = 0;
        for (int i = 0; i < header.biHeight; i++) {
            for (int j = 0; j < header.biWidth; j++) {
                dataStream[count++] = (char)round(Y[i][j]);
                dataStream[count++] = (char)round(Cb[i][j]);
                dataStream[count++] = (char)round(Cr[i][j]);
            }
        }
        fwrite(dataStream, sizeof(char),count, aux);        
        fclose(aux);
        
        //YCbCr -> RGB
        aux = fopen("data.dat","rb");
        char *dataStreamOut = (char*)malloc(3*header.biHeight*header.biWidth*sizeof(char));
        //lendo com fread para que seja lido os valores negativos
        fread(dataStreamOut,sizeof(char),count,aux);    
        count = 0;    
        for (int i = 0; i < header.biHeight; i++) {
            for (int j = 0; j < header.biWidth; j++) {
                Y[i][j] = dataStreamOut[count++];
                Cb[i][j] = dataStreamOut[count++];
                Cr[i][j] = dataStreamOut[count++];
                
                double d, e, f;
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
                
                fputc((unsigned char)R[i][j], saida);
                fputc((unsigned char)G[i][j], saida);
                fputc((unsigned char)B[i][j], saida);
            }
        }*/
        /*
        |                        |
        |---  CHAR SEM ROUND --- |
        | PSNR cai para ~46dB!!! |
        */
        //RGB -> YCbCr
        /*for (int i = 0; i < header.biHeight; i++) {
            for (int j = 0; j < header.biWidth; j++) {
                R[i][j] = fgetc(entrada);
                G[i][j] = fgetc(entrada);
                B[i][j] = fgetc(entrada);    
                               
                //conversao para YCbCr c/ level shift
                double a,b,c;
                a = (0.299f * (R[i][j] - shift) + 0.587f * (G[i][j] - shift) + 0.114f * (B[i][j] - shift));
                b = (0.565f * ((B[i][j] - shift) - a));
                c = (0.713f * ((R[i][j] - shift) - a));                
                Y[i][j] = a;
                Cb[i][j] = b;
                Cr[i][j] = c;                         
            }
        }        
        //utilizando fwrite para gravar os valores negativos e round. Isso irá diminuir o PSNR pela metade, mesmo armazenando em double!
        char *dataStream = (char*)malloc(3*header.biHeight*header.biWidth*sizeof(char));
        int count = 0;
        for (int i = 0; i < header.biHeight; i++) {
            for (int j = 0; j < header.biWidth; j++) {
                dataStream[count++] = (char)(Y[i][j]); //sem round
                dataStream[count++] = (char)(Cb[i][j]); //sem round
                dataStream[count++] = (char)(Cr[i][j]); //sem round
            }
        }
        fwrite(dataStream, sizeof(char),count, aux);        
        fclose(aux);
        
        //YCbCr -> RGB
        aux = fopen("data.dat","rb");
        char *dataStreamOut = (char*)malloc(3*header.biHeight*header.biWidth*sizeof(char));
        //lendo com fread para que seja lido os valores negativos
        fread(dataStreamOut,sizeof(char),count,aux);    
        count = 0;    
        for (int i = 0; i < header.biHeight; i++) {
            for (int j = 0; j < header.biWidth; j++) {
                Y[i][j] = dataStreamOut[count++];
                Cb[i][j] = dataStreamOut[count++];
                Cr[i][j] = dataStreamOut[count++];
                
                double d, e, f;
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
                
                fputc((unsigned char)R[i][j], saida);
                fputc((unsigned char)G[i][j], saida);
                fputc((unsigned char)B[i][j], saida);
            }
        }*/   
    }//else BMP    
    fclose(entrada);
    fclose(saida);
    fclose(aux);
    //TODO:liberar ponteiros
}
