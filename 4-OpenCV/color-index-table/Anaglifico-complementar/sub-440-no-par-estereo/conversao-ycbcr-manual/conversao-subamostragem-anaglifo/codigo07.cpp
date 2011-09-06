//Programa para testar o PSNR ao realizar a conversão na reversão anaglífica sem nenhuma função do opencv
//lendo manualmente os dados das imagens, realizando a conversão, transformação anaglífica,
//subamostragem 4:4:0 no anaglifo complementar
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
    FILE *entrada, *saida, *verdemagenta, *complementar, *aux;

    if(!strcmp(argv[2],"-e")){
        printf("Conversao anaglifica\n");
        entrada = fopen(argv[1], "rb");
        //saida = fopen("revertido.bmp", "wb");
        aux = fopen("data.dat", "wb");
        verdemagenta = fopen("verdemagenta.bmp","wb");
        //complementar = fopen("complementar.bmp","wb");

        if (entrada == NULL) {
            fprintf(stderr, "Nao foi possivel abrir o arquivo %s\n ", argv[1]);
            exit(EXIT_FAILURE);
        } else {
            //leitura do header do BMP, escrita do mesmo em arquivo de saída
            fread(&bmpnum, 2, 1, entrada);
            //fwrite(&bmpnum, 2, 1, saida);
            fwrite(&bmpnum, 2, 1, verdemagenta);
            //fwrite(&bmpnum, 2, 1, complementar);
            fread(&fHeader, sizeof(BMPFILEHEADER), 1, entrada);
            //fwrite(&fHeader, sizeof(BMPFILEHEADER), 1, saida);
            // verifica se o arquivo eh um bitmap
            if (bmpnum.bfType != 19778) {
                fprintf(stderr, "Arquivo nao eh um bitmap.\n");
                exit(EXIT_FAILURE);
            }
            fread(&header, sizeof (BMPINFOHEADER), 1, entrada);
            //fwrite(&header, sizeof(BMPINFOHEADER), 1, saida);
            fHeaderAnaglyph = fHeader;
            fHeaderAnaglyph.bfSize = header.biHeight * header.biWidth / 2;
            headerAnaglyph = header;
            headerAnaglyph.biWidth = header.biWidth / 2;
            fwrite(&fHeaderAnaglyph, sizeof(BMPFILEHEADER), 1, verdemagenta);
            //fwrite(&fHeaderAnaglyph, sizeof(BMPFILEHEADER), 1, complementar);
            fwrite(&headerAnaglyph, sizeof(BMPINFOHEADER), 1, verdemagenta);
            //fwrite(&headerAnaglyph, sizeof(BMPINFOHEADER), 1, complementar);

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

            //separação do par estéreo
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
            for (int i = 0; i < header.biHeight; i++) {
                for (int j = 0; j < header.biWidth/2; j++) {
                    B1[i][j] = B[i][j];
                    G1[i][j] = G[i][j];
                    R1[i][j] = R[i][j];
                    B2[i][j] = B[i][j+header.biWidth/2];
                    G2[i][j] = G[i][j+header.biWidth/2];
                    R2[i][j] = R[i][j+header.biWidth/2];
                    //save anaglyphs
                    fputc(B1[i][j],verdemagenta);
                    fputc(G2[i][j],verdemagenta);
                    fputc(R1[i][j],verdemagenta);
                    printf("%f %f %f\n",B1[i][j],G2[i][j],R1[i][j]);
                    /*fputc(B2[i][j],complementar);
                    fputc(G1[i][j],complementar);
                    fputc(R2[i][j],complementar);*/
                }
            }

            //conversões RGB -> YCbCr do complementar (B2G1R2)
            int shift = 128;
            for (int i = 0; i < headerAnaglyph.biHeight; i++) {
                for (int j = 0; j < headerAnaglyph.biWidth; j++) {
                    //conversao para YCbCr c/ level shift
                    float a,b,c;
                    a = (0.299f * (R2[i][j] - shift) + 0.587f * (G1[i][j] - shift) + 0.114f * (B2[i][j] - shift));
                    b = (0.565f * ((B2[i][j] - shift) - a));
                    c = (0.713f * ((R2[i][j] - shift) - a));
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

            //subamostragem 4:4:0 do Complementar, eliminando Y
            int imageSize = headerAnaglyph.biHeight*headerAnaglyph.biWidth;
            int countCbCr = 0;
            char *subData = (char*) malloc(imageSize*sizeof(char));
            for (int i = 0; i < headerAnaglyph.biHeight; i++) {
                if(i % 2 == 0){ //copia valores Y Cb e Cr das linhas pares
                    for (int j = 0; j < headerAnaglyph.biWidth; j++) {
                        subData[countCbCr]              = (char)round(Cb[i][j]);
                        subData[countCbCr+imageSize/2]  = (char)round(Cr[i][j]);
                        countCbCr++;
                    }
                }
                else{
                    //nas linhas ímpares, copia Y, volta width posições nas regiões de Cb
                    //e Cr portions do vertor subData e calcula a média dos valores já obtidos
                    //com os valores de Cb e Cr desta linha
                    countCbCr -= headerAnaglyph.biWidth;
                    for(int j = 0; j < headerAnaglyph.biWidth; j++){
                        subData[countCbCr]              = (char)round((subData[countCbCr]+Cb[i][j])/2);
                        subData[countCbCr+imageSize/2]  = (char)round((subData[countCbCr+imageSize/2] + Cr[i][j])/2);
                        countCbCr++;
                    }
                }
            }

            //escrita no arquivo de saída
            fwrite(subData, sizeof(char),imageSize, aux);
            fclose(aux);


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
            free(subData);
            fclose(entrada);
            fclose(aux);
            fclose(verdemagenta);
            printf("Ok!");
        }
    }
    if(!strcmp(argv[2],"-d")){
        printf("Reversao anaglifica\n");
        entrada = fopen(argv[1], "rb");
        saida = fopen("revertido.bmp", "wb");
        FILE *teste = fopen("teste.bmp","wb");
        if (entrada == NULL) {
            fprintf(stderr, "Nao foi possivel abrir o arquivo %s\n ", argv[1]);
            exit(EXIT_FAILURE);
        } else {
            //leitura do header do BMP, escrita do mesmo em arquivo de saída
            fread(&bmpnum, 2, 1, entrada);
            fwrite(&bmpnum, 2, 1, saida);
            fwrite(&bmpnum, 2, 1, teste);
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
            fwrite(&fHeaderAnaglyph, sizeof(BMPFILEHEADER), 1, teste);
            fwrite(&headerAnaglyph, sizeof(BMPINFOHEADER), 1, teste);
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

            //Leitura do anáglifo verde-magenta
            //TODO: VERIFICAR POR QUE NÃO DÁ CERTO.
            verdemagenta = fopen("verdemagenta.bmp","rb");
            for (int i = 0; i < header.biHeight; i++) {
                for (int j = 0; j < header.biWidth/2; j++) {
                    B1[i][j] = fgetc(verdemagenta);
                    G2[i][j] = fgetc(verdemagenta);
                    R1[i][j] = fgetc(verdemagenta);

                    fputc(B1[i][j],teste);
                    fputc(G2[i][j],teste);
                    fputc(R1[i][j],teste);

                    printf("%f %f %f\n",B1[i][j],G2[i][j],R1[i][j]);
                }
            }

            /*-------------
                Reversão anaglífica
            --------------*/
            int shift = 128;
            int imageSize = headerAnaglyph.biHeight*headerAnaglyph.biWidth;
            aux = fopen("data.dat","rb");
            char *dataStreamOut = (char*)malloc(imageSize*sizeof(char));
            fread(dataStreamOut,sizeof(char),imageSize,aux);

            //RGB -> YCbCr do verdemagenta (R1G2B1) para extração de Y
            for (int i = 0; i < headerAnaglyph.biHeight; i++) {
                for (int j = 0; j < headerAnaglyph.biWidth; j++) {
                    //conversao para YCbCr c/ level shift
                    float a = (0.299f * (R1[i][j] - shift) + 0.587f * (G2[i][j] - shift) + 0.114f * (B1[i][j] - shift));
                    Y[i][j] = a;
                }
            }

            //reversão para 4:4:4 do complementar
            int countCbCr = 0;
            for (int i = 0; i < headerAnaglyph.biHeight; i++) {
                if(i % 2 != 0){
                    countCbCr -= headerAnaglyph.biWidth;
                }
                for (int j = 0; j < headerAnaglyph.biWidth; j++) {
                    Cb[i][j] = dataStreamOut[countCbCr];
                    Cr[i][j] = dataStreamOut[countCbCr+(imageSize/2)];
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
            free(dataStreamOut);
            fclose(entrada);
            fclose(saida);
            fclose(aux);
            printf("Ok!");
        }
    }//else BMP
}
