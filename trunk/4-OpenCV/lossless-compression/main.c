#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "easyzlib.h"

int main(int argc, char* argv[])
{
    if(strcmp("-c",argv[2])==0){
        FILE *inputFile = fopen(argv[1],"rb");
        fseek(inputFile, 0, SEEK_END);
        size_t inputSize = ftell(inputFile);
        fseek(inputFile, 0, SEEK_SET);

        unsigned char *inputBuffer = (unsigned char *)malloc(inputSize);

        fread(inputBuffer, 1, inputSize, inputFile);

        long int outputSize = EZ_COMPRESSMAXDESTLENGTH(inputSize);
        unsigned char *outputBuffer = (unsigned char *)malloc(outputSize);
        FILE *saida = fopen("saida.dat","wb");
        int result = ezcompress(outputBuffer, &outputSize, inputBuffer, inputSize);

        if (result != EZ_BUF_ERROR) {
            // Do stuff with outputBuffer which has length outputSize
            fwrite(outputBuffer,sizeof(char),outputSize,saida);
            printf("Comprimido\n");
        } else {
            // Something went wrong
            printf("erro na compressao\n");
        }
    }
    else{
        FILE *inputFile = fopen(argv[1],"rb");
        fseek(inputFile, 0, SEEK_END);
        size_t inputSize = ftell(inputFile);
        fseek(inputFile, 0, SEEK_SET);

        unsigned char *inputBuffer = (unsigned char *)malloc(inputSize);

printf("%d inputSize", inputSize);

        fread(inputBuffer, 1, inputSize, inputFile);

        long int outputSize = EZ_COMPRESSMAXDESTLENGTH(inputSize);
        unsigned char *outputBuffer = (unsigned char *)malloc(999999);
        long int outputSizeUncomp;
        FILE *saidaRevertida = fopen("saida-reverso.dat","wb");

        int result = ezuncompress(outputBuffer, &outputSizeUncomp, inputBuffer, inputSize);

        if (result != EZ_BUF_ERROR) {
            // Do stuff with outputBuffer which has length outputSize
            fwrite(outputBuffer,sizeof(char),outputSizeUncomp,saidaRevertida);
            printf("Descomprimido");
        } else {
            // Something went wrong
            printf("erro na descompressao %d", result);
        }
    }

    return 0;
}
