//Programa para testar o PSNR ao realizar a conversão na reversão anaglífica sem nenhuma função do opencv
//lendo manualmente os dados das imagens, realizando a conversão, transformação anaglífica,
//subamostragem 4:4:0 em ambos os anáglifos
//armazenando em arquivo com char (round e cast)
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
    BMPFILEHEADER fHeaderAnaglyph;
    BMPINFOHEADER headerAnaglyph;
    FILE *entrada, *saida, *verdemagenta, *cit, *vm;

    if(!strcmp(argv[2],"-e")){
        printf("Conversao anaglifica\n");
        entrada = fopen(argv[1], "rb");
        cit = fopen("cit.dat", "wb");
        vm = fopen("verdemagenta.dat","wb");

        if (entrada == NULL) {
            fprintf(stderr, "Nao foi possivel abrir o arquivo %s\n ", argv[1]);
            exit(EXIT_FAILURE);
        } else {
            //leitura do header do BMP, escrita do mesmo em arquivo de saída
            fread(&bmpnum, 2, 1, entrada);
            fread(&fHeader, sizeof(BMPFILEHEADER), 1, entrada);
            // verifica se o arquivo eh um bitmap
            if (bmpnum.bfType != 19778) {
                fprintf(stderr, "Arquivo nao eh um bitmap.\n");
                exit(EXIT_FAILURE);
            }
            fread(&header, sizeof (BMPINFOHEADER), 1, entrada);
            fHeaderAnaglyph = fHeader;
            fHeaderAnaglyph.bfSize = header.biHeight * header.biWidth / 2;
            headerAnaglyph = header;
            headerAnaglyph.biWidth = header.biWidth / 2;

            //estruturas para leitura dos dados
            //RGB -> unsigned char (0-255)
            //YCbCr c/ level shift -> float (-128 a 127)
            float **R =  NULL;
            float **G = NULL;
            float **B = NULL;
            float **R1 =  NULL;
            float **G1 = NULL;
            float **B1 = NULL;
            float **R2 =  NULL;
            float **G2 = NULL;
            float **B2 = NULL;
            float **Y =  NULL;
            float **Cb = NULL;
            float **Cr = NULL;
            R = (float**)malloc(header.biHeight*sizeof(float*));
            G = (float**)malloc(header.biHeight*sizeof(float*));
            B = (float**)malloc(header.biHeight*sizeof(float*));
            R1 = (float**)malloc(header.biHeight*sizeof(float*));
            G1 = (float**)malloc(header.biHeight*sizeof(float*));
            B1 = (float**)malloc(header.biHeight*sizeof(float*));
            R2 = (float**)malloc(header.biHeight*sizeof(float*));
            G2 = (float**)malloc(header.biHeight*sizeof(float*));
            B2 = (float**)malloc(header.biHeight*sizeof(float*));
            Y = (float**)malloc(header.biHeight*sizeof(float*));
            Cb = (float**)malloc(header.biHeight*sizeof(float*));
            Cr = (float**)malloc(header.biHeight*sizeof(float*));
            for(int i = 0; i < header.biHeight; i++){
                R[i] = (float*)malloc(header.biWidth*sizeof(float));
                G[i] = (float*)malloc(header.biWidth*sizeof(float));
                B[i] = (float*)malloc(header.biWidth*sizeof(float));
                R1[i] = (float*)malloc(header.biWidth/2*sizeof(float));
                G1[i] = (float*)malloc(header.biWidth/2*sizeof(float));
                B1[i] = (float*)malloc(header.biWidth/2*sizeof(float));
                R2[i] = (float*)malloc(header.biWidth/2*sizeof(float));
                G2[i] = (float*)malloc(header.biWidth/2*sizeof(float));
                B2[i] = (float*)malloc(header.biWidth/2*sizeof(float));
                Y[i] = (float*)malloc(header.biWidth*sizeof(float));
                Cb[i] = (float*)malloc(header.biWidth*sizeof(float));
                Cr[i] = (float*)malloc(header.biWidth*sizeof(float));
            }

            //leitura de dados do par estéreo
            for (int i = 0; i < header.biHeight; i++) {
                for (int j = 0; j < header.biWidth; j++) {
                    B[i][j] = fgetc(entrada);
                    G[i][j] = fgetc(entrada);
                    R[i][j] = fgetc(entrada);
                }
            }

            /*-------------
                Conversão anaglífica. Mantém o verde-magenta em RGB
                e armazena Cb e Cr do complementar
            --------------*/
            //separação do par estéreo
            for (int i = 0; i < header.biHeight; i++) {
                for (int j = 0; j < header.biWidth/2; j++) {
                    B1[i][j] = B[i][j];
                    G1[i][j] = G[i][j];
                    R1[i][j] = R[i][j];
                    B2[i][j] = B[i][j+header.biWidth/2];
                    G2[i][j] = G[i][j+header.biWidth/2];
                    R2[i][j] = R[i][j+header.biWidth/2];
                }
            }

            //conversão RGB -> YCbCr do complementar (B2G1R2)
            int shift = 128;
            for (int i = 0; i < headerAnaglyph.biHeight; i++) {
                for (int j = 0; j < headerAnaglyph.biWidth; j++) {
                    //conversao para YCbCr c/ level shift
                    float a,b,c;
                    a = (0.299f * (R2[i][j] - shift) + 0.587f * (G1[i][j] - shift) + 0.114f * (B2[i][j] - shift));
                    b = (0.565f * ((B2[i][j] - shift) - a));
                    c = (0.713f * ((R2[i][j] - shift) - a));
                    Cb[i][j] = b;
                    Cr[i][j] = c;
                }
            }
            //subamostragem 4:4:0 do Complementar, eliminando Y
            int imageSize = headerAnaglyph.biHeight*headerAnaglyph.biWidth;
            int countCbCr = 0;
            char *subDataCompl = (char*) malloc(imageSize*sizeof(char));
            for (int i = 0; i < headerAnaglyph.biHeight; i++) {
                if(i % 2 == 0){ //copia valores Y Cb e Cr das linhas pares
                    for (int j = 0; j < headerAnaglyph.biWidth; j++) {
                        subDataCompl[countCbCr]              = (char)round(Cb[i][j]);
                        subDataCompl[countCbCr+imageSize/2]  = (char)round(Cr[i][j]);
                        countCbCr++;
                    }
                }
                else{
                    //nas linhas ímpares, copia Y, volta width posições nas regiões de Cb
                    //e Cr portions do vertor subData e calcula a média dos valores já obtidos
                    //com os valores de Cb e Cr desta linha
                    countCbCr -= headerAnaglyph.biWidth;
                    for(int j = 0; j < headerAnaglyph.biWidth; j++){
                        subDataCompl[countCbCr]              = (char)round((subDataCompl[countCbCr]+Cb[i][j])/2);
                        subDataCompl[countCbCr+imageSize/2]  = (char)round((subDataCompl[countCbCr+imageSize/2] + Cr[i][j])/2);
                        countCbCr++;
                    }
                }
            }
            //escrita no arquivo de saída
            fwrite(subDataCompl, sizeof(char),imageSize, cit);
            fclose(cit);
            
            //conversão RGB -> YCbCr do verde-magenta (B1G2R1)
            for (int i = 0; i < headerAnaglyph.biHeight; i++) {
                for (int j = 0; j < headerAnaglyph.biWidth; j++) {
                    //conversao para YCbCr c/ level shift
                    float a,b,c;
                    a = (0.299f * (R1[i][j] - shift) + 0.587f * (G2[i][j] - shift) + 0.114f * (B1[i][j] - shift));
                    b = (0.565f * ((B1[i][j] - shift) - a));
                    c = (0.713f * ((R1[i][j] - shift) - a));
                    Y[i][j] = a;
                    Cb[i][j] = b;
                    Cr[i][j] = c;
                }
            }
            //subamostragem 4:4:0 do verde-magenta
            int countY = 0;
            countCbCr = 0;
            char *subDataVM = (char*) malloc(2*imageSize*sizeof(char));
            for (int i = 0; i < headerAnaglyph.biHeight; i++) {
                if(i % 2 == 0){ //copia valores Y Cb e Cr das linhas pares
                    for (int j = 0; j < headerAnaglyph.biWidth; j++) {
                        subDataVM[countCbCr]              = (char)round(Cb[i][j]);
                        subDataVM[countCbCr+imageSize/2]  = (char)round(Cr[i][j]);
                        subDataVM[countY+imageSize]       = (char)round(Y[i][j]);
                        countY++;
                        countCbCr++;
                    }
                }
                else{
                    //nas linhas ímpares, copia Y, volta width posições nas regiões de Cb
                    //e Cr portions do vertor subData e calcula a média dos valores já obtidos
                    //com os valores de Cb e Cr desta linha
                    countCbCr -= headerAnaglyph.biWidth;
                    for(int j = 0; j < headerAnaglyph.biWidth; j++){
                        subDataVM[countCbCr]              = (char)round((subDataVM[countCbCr]+Cb[i][j])/2);
                        subDataVM[countCbCr+imageSize/2]  = (char)round((subDataVM[countCbCr+imageSize/2] + Cr[i][j])/2);
                        subDataVM[countY+imageSize]       = (char)round(Y[i][j]);
                        countY++;
                        countCbCr++;
                    }
                }
            }
            //escrita no arquivo de saída
            fwrite(subDataVM, sizeof(char),2*imageSize, vm);
            fclose(vm);

            for(int i = 0; i < header.biHeight; i++){
                free(R[i]);
                free(G[i]);
                free(B[i]);
                free(R1[i]);
                free(G1[i]);
                free(B1[i]);
                free(R2[i]);
                free(G2[i]);
                free(B2[i]);
                free(Y[i]);
                free(Cb[i]);
                free(Cr[i]);
            }
            free(R);
            free(G);
            free(B);
            free(R1);
            free(G1);
            free(B1);
            free(R2);
            free(G2);
            free(B2);
            free(Y);
            free(Cb);
            free(Cr);
            free(subDataVM);
            free(subDataCompl);
            fclose(entrada);
            printf("Ok!");
        }
    }
    if(!strcmp(argv[2],"-d")){
        printf("Reversao anaglifica\n");
        entrada = fopen(argv[1], "rb");
        saida = fopen("revertido.bmp", "wb");
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
            fHeaderAnaglyph = fHeader;
            fHeaderAnaglyph.bfSize = header.biHeight * header.biWidth / 2;
            headerAnaglyph = header;
            headerAnaglyph.biWidth = header.biWidth / 2;

            //estruturas para leitura dos dados
            //RGB -> unsigned char (0-255)
            //YCbCr c/ level shift -> float (-128 a 127)
            float **R =  NULL;
            float **G = NULL;
            float **B = NULL;
            float **R1 =  NULL;
            float **G1 = NULL;
            float **B1 = NULL;
            float **R2 =  NULL;
            float **G2 = NULL;
            float **B2 = NULL;
            float **Y =  NULL;
            float **Cb = NULL;
            float **Cr = NULL;
            R =  (float**)malloc(header.biHeight*sizeof(float*));
            G =  (float**)malloc(header.biHeight*sizeof(float*));
            B =  (float**)malloc(header.biHeight*sizeof(float*));
            R1 = (float**)malloc(header.biHeight*sizeof(float*));
            G1 = (float**)malloc(header.biHeight*sizeof(float*));
            B1 = (float**)malloc(header.biHeight*sizeof(float*));
            R2 = (float**)malloc(header.biHeight*sizeof(float*));
            G2 = (float**)malloc(header.biHeight*sizeof(float*));
            B2 = (float**)malloc(header.biHeight*sizeof(float*));
            Y =  (float**)malloc(header.biHeight*sizeof(float*));
            Cb = (float**)malloc(header.biHeight*sizeof(float*));
            Cr = (float**)malloc(header.biHeight*sizeof(float*));
            for(int i = 0; i < header.biHeight; i++){
                //par estéreo
                R[i]  = (float*)malloc(header.biWidth*sizeof(float));
                G[i]  = (float*)malloc(header.biWidth*sizeof(float));
                B[i]  = (float*)malloc(header.biWidth*sizeof(float));                
                Y[i]  = (float*)malloc(header.biWidth*sizeof(float));
                Cb[i] = (float*)malloc(header.biWidth*sizeof(float));
                Cr[i] = (float*)malloc(header.biWidth*sizeof(float));
                //anáglifos
                R1[i] = (float*)malloc(header.biWidth/2*sizeof(float));
                G1[i] = (float*)malloc(header.biWidth/2*sizeof(float));
                B1[i] = (float*)malloc(header.biWidth/2*sizeof(float));
                R2[i] = (float*)malloc(header.biWidth/2*sizeof(float));
                G2[i] = (float*)malloc(header.biWidth/2*sizeof(float));
                B2[i] = (float*)malloc(header.biWidth/2*sizeof(float));
            }

            /*-------------
                Reversão anaglífica
            --------------*/
            int shift = 128;
            int imageSize = headerAnaglyph.biHeight*headerAnaglyph.biWidth;
            //leitura da tabela de índice de cores
            cit = fopen("cit.dat","rb");
            char *dataStreamCIT = (char*)malloc(imageSize*sizeof(char));
            fread(dataStreamCIT,sizeof(char),imageSize,cit);
            //leitura do anáglifo verde-magenta
            vm = fopen("verdemagenta.dat","rb");
            char *dataStreamVM = (char*)malloc(2*imageSize*sizeof(char));
            fread(dataStreamVM,sizeof(char),2*imageSize,vm);

            //extração de Y
            int countY = 0;
            for (int i = 0; i < headerAnaglyph.biHeight; i++) {
                for (int j = 0; j < headerAnaglyph.biWidth; j++) {
                    Y[i][j] = dataStreamVM[countY+imageSize];
                    countY++;
                }
            }
            
            //reversão Cb e Cr para 4:4:4 do complementar
            int countCbCr = 0;
            for (int i = 0; i < headerAnaglyph.biHeight; i++) {
                if(i % 2 != 0){
                    countCbCr -= headerAnaglyph.biWidth;
                }
                for (int j = 0; j < headerAnaglyph.biWidth; j++) {
                    Cb[i][j] = dataStreamCIT[countCbCr];
                    Cr[i][j] = dataStreamCIT[countCbCr+(imageSize/2)];
                    countCbCr++;
                }
            }
            //YCbCr -> RGB do complementar (B2G1R2)
            for (int i = 0; i < headerAnaglyph.biHeight; i++) {
                for (int j = 0; j < headerAnaglyph.biWidth; j++) {
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
                    R2[i][j] = round(d);
                    G1[i][j] = round(e);
                    B2[i][j] = round(f);
                }
            }
            
            //reversão do verde-magenta para 4:4:4
            countCbCr = 0;
            for (int i = 0; i < headerAnaglyph.biHeight; i++) {
                if(i % 2 != 0){
                    countCbCr -= headerAnaglyph.biWidth;
                }
                for (int j = 0; j < headerAnaglyph.biWidth; j++) {
                    Cb[i][j] = dataStreamVM[countCbCr];
                    Cr[i][j] = dataStreamVM[countCbCr+(imageSize/2)];
                    countCbCr++;
                }
            }
            //YCbCr -> RGB do verde-magenta (B1G2R1)
            for (int i = 0; i < headerAnaglyph.biHeight; i++) {
                for (int j = 0; j < headerAnaglyph.biWidth; j++) {
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
                    R1[i][j] = round(d);
                    G2[i][j] = round(e);
                    B1[i][j] = round(f);
                }
            }

            //Reconstrução do par estéreo
            for (int i = 0; i < headerAnaglyph.biHeight; i++) {
                for (int j = 0; j < headerAnaglyph.biWidth; j++) {
                    B[i][j] = B1[i][j];
                    G[i][j] = G1[i][j];
                    R[i][j] = R1[i][j];
                    B[i][j+header.biWidth/2] = B2[i][j];
                    G[i][j+header.biWidth/2] = G2[i][j];
                    R[i][j+header.biWidth/2] = R2[i][j];
                }
            }
            for (int i = 0; i < header.biHeight; i++) {
                for (int j = 0; j < header.biWidth; j++) {
                    fputc((unsigned char)B[i][j], saida);
                    fputc((unsigned char)G[i][j], saida);
                    fputc((unsigned char)R[i][j], saida);
                }
            }

            for(int i = 0; i < header.biHeight; i++){
                free(R[i]);
                free(G[i]);
                free(B[i]);
                free(R1[i]);
                free(G1[i]);
                free(B1[i]);
                free(R2[i]);
                free(G2[i]);
                free(B2[i]);
                free(Y[i]);
                free(Cb[i]);
                free(Cr[i]);
            }
            free(R);
            free(G);
            free(B);
            free(R1);
            free(G1);
            free(B1);
            free(R2);
            free(G2);
            free(B2);
            free(Y);
            free(Cb);
            free(Cr);
            fclose(entrada);
            fclose(saida);
            printf("Ok!");
        }
    }//else BMP
    return 0;
}
