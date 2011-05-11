//TODO: mostrar a posi��o da palavra no texto
/*
  ICMC - USP/S�o Carlos         Entrega: 12/05/2011
  SCC5900-1.2001 - Projeto de Algoritmos
  
  Integrantes:
              Matheus Ricardo Uihara Zingarelli - 5377855
              Raiza Tamae Sarkis Hanada - 7493723
  
  Programa que recebe um texto e, para cada palavra de tamanho >= 2 retorna:
            - em uma linha: a palavra
            - em uma linha: time of the day
            - em uma linha: o n�mero de ocorr�ncias dessa palavra no texto
            - em uma linha: a posi��o de todas as ocorrencias da palavra no texto
             (posi��o do primeiro caractere da palavra)
            - em uma linha: time of the day
            - linha em branco
  
  Utilizando o algoritmo de Horspool
*/
#include <stdio.h>
#include <time.h>
#include "alphabet_analysis.c"

/*
    Utiliza o algoritmo de Horspool para encontrar o n�mero de ocorr�ncias e a
    posi��o de cada ocorr�ncia de cada palavra na lista de palavras em rela��o
    a um texto.
    Entradas:
        listaPalavras - lista de palavras
        qtde - quantidade de palavras na lista
*/
void horspool(palavra *listaPalavras, int qtde){    
    //armazena arquivo de texto em mem�ria
    FILE *pt;
    pt=fopen("input.txt","r");
    fseek (pt , 0 , SEEK_END);
    long tamTexto = ftell (pt); //tamanho do arquivo
    rewind (pt);
    char *texto = (char*) malloc (sizeof(char)*tamTexto);
    if (texto == NULL) {
        printf("Problema de alocacao de memoria");
        exit (2);
    }
    fread (texto,1,tamTexto,pt);
    fclose(pt);
    //cria a tabela de shift
    constroiShiftTable(listaPalavras, qtde);
    
    //faz a busca de cada palavra no texto
    pt=fopen("output.txt","a");  //TODO: mudar o nome
    int i, ocorrencia;    
    for(i=0; i<qtde; i++){//para cada palavra
        if(listaPalavras[i].tamPalavra>=2){
            //palavra
            fprintf(pt,"%s\n",listaPalavras[i].p);
            //hora do dia
            time_t rawtime;
            struct tm * timeinfo;        
            time(&rawtime);
            timeinfo = localtime(&rawtime);
            fprintf (pt,"%s", asctime(timeinfo));
    
            ocorrencia = 0;
            int j = listaPalavras[i].tamPalavra-1; //guarda a posi��o para o �ltimo caracter da palavra
            while(j < tamTexto){ //enquanto n�o atingiu o fim do texto
                int k = 0; //n�mero de caracteres que deram match
                //enquanto houver matching de cada caracter
                while(k < listaPalavras[i].tamPalavra && listaPalavras[i].p[listaPalavras[i].tamPalavra - k - 1] == lowerCaseChar(texto[j-k])){
                    k++;
                }
                if(k == listaPalavras[i].tamPalavra){//matching de palavra
                    ocorrencia++;
                    j++; //shift para continuar procurando mais ocorr�ncias
                }
                else{
                    //Valida��o adicional!!
                    //estamos considerando apenas um subconjunto de caracteres da 
                    //tabela ASCII como sendo s�mbolos v�lidos. Qualquer outro s�mbolo 
                    //fora da tabela ASCII, tal como �, � dentre outros ser�o considerados
                    // inv�lidos e o shift ser� o tamanho da palavra
                    if(texto[j]<32 || texto[j]>126){
                        j += listaPalavras[i].tamPalavra;
                    }
                    else{
                        j += listaPalavras[i].shiftTable[lowerCaseChar(texto[j])-32].shiftStep;
                    }
                }
            }//while
            
            //n�mero de ocorr�ncias
            fprintf(pt,"%d\n", ocorrencia);
            
            //posi��o de cada ocorr�ncia no texto
            
            //hora do dia      
            time(&rawtime);
            timeinfo = localtime(&rawtime);
            fprintf (pt,"%s\n", asctime(timeinfo));
        }
    }//for
    fclose(pt);    
}

int main(int argc, char*argv[]){
    //---------An�lise do alfabeto
    char *texto = (char*)malloc(sizeof(char)*4000); 
    //TODO:decidir o tamanho m�ximo da lista de palavras  
    palavra *palavrasValidas = (palavra*)malloc(sizeof(palavra)*4000);     
    simbolo *alfabeto = (simbolo*)malloc(sizeof(simbolo)*126-32+1); //armazena os s�mbolos de 32 a 126 da tabela ASCII
    
    //C�lculo do n�mero total de palavras v�lidas no texto
    const int QTDE_PALAVRAS = leitura (texto,palavrasValidas);
    //contagem de frequ�ncia de s�mbolos
    const int TAM_ALFABETO = calculaAlfabeto(alfabeto);
    //escrita no arquivo da an�lise feita
    escreve(TAM_ALFABETO,alfabeto,QTDE_PALAVRAS);
    
    //---------String Matching
    horspool(palavrasValidas, QTDE_PALAVRAS);
    
    //debug-->
    //imprimeShiftTable(palavrasValidas, QTDE_PALAVRAS);
    //<--debug
    free(texto);
    free(palavrasValidas);
    free(alfabeto);
    return 0;
}
