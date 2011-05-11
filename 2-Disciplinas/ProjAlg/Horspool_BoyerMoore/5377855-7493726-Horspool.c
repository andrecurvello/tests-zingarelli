//TODO: mostrar a posição da palavra no texto
/*
  ICMC - USP/São Carlos         Entrega: 12/05/2011
  SCC5900-1.2001 - Projeto de Algoritmos
  
  Integrantes:
              Matheus Ricardo Uihara Zingarelli - 5377855
              Raiza Tamae Sarkis Hanada - 7493723
  
  Programa que recebe um texto e, para cada palavra de tamanho >= 2 retorna:
            - em uma linha: a palavra
            - em uma linha: time of the day
            - em uma linha: o número de ocorrências dessa palavra no texto
            - em uma linha: a posição de todas as ocorrencias da palavra no texto
             (posição do primeiro caractere da palavra)
            - em uma linha: time of the day
            - linha em branco
  
  Utilizando o algoritmo de Horspool
*/
#include <stdio.h>
#include <time.h>
#include "alphabet_analysis.c"

/*
    Utiliza o algoritmo de Horspool para encontrar o número de ocorrências e a
    posição de cada ocorrência de cada palavra na lista de palavras em relação
    a um texto.
    Entradas:
        listaPalavras - lista de palavras
        qtde - quantidade de palavras na lista
*/
void horspool(palavra *listaPalavras, int qtde){    
    //armazena arquivo de texto em memória
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
            int j = listaPalavras[i].tamPalavra-1; //guarda a posição para o último caracter da palavra
            while(j < tamTexto){ //enquanto não atingiu o fim do texto
                int k = 0; //número de caracteres que deram match
                //enquanto houver matching de cada caracter
                while(k < listaPalavras[i].tamPalavra && listaPalavras[i].p[listaPalavras[i].tamPalavra - k - 1] == lowerCaseChar(texto[j-k])){
                    k++;
                }
                if(k == listaPalavras[i].tamPalavra){//matching de palavra
                    ocorrencia++;
                    j++; //shift para continuar procurando mais ocorrências
                }
                else{
                    //Validação adicional!!
                    //estamos considerando apenas um subconjunto de caracteres da 
                    //tabela ASCII como sendo símbolos válidos. Qualquer outro símbolo 
                    //fora da tabela ASCII, tal como ç, ¬ dentre outros serão considerados
                    // inválidos e o shift será o tamanho da palavra
                    if(texto[j]<32 || texto[j]>126){
                        j += listaPalavras[i].tamPalavra;
                    }
                    else{
                        j += listaPalavras[i].shiftTable[lowerCaseChar(texto[j])-32].shiftStep;
                    }
                }
            }//while
            
            //número de ocorrências
            fprintf(pt,"%d\n", ocorrencia);
            
            //posição de cada ocorrência no texto
            
            //hora do dia      
            time(&rawtime);
            timeinfo = localtime(&rawtime);
            fprintf (pt,"%s\n", asctime(timeinfo));
        }
    }//for
    fclose(pt);    
}

int main(int argc, char*argv[]){
    //---------Análise do alfabeto
    char *texto = (char*)malloc(sizeof(char)*4000); 
    //TODO:decidir o tamanho máximo da lista de palavras  
    palavra *palavrasValidas = (palavra*)malloc(sizeof(palavra)*4000);     
    simbolo *alfabeto = (simbolo*)malloc(sizeof(simbolo)*126-32+1); //armazena os símbolos de 32 a 126 da tabela ASCII
    
    //Cálculo do número total de palavras válidas no texto
    const int QTDE_PALAVRAS = leitura (texto,palavrasValidas);
    //contagem de frequência de símbolos
    const int TAM_ALFABETO = calculaAlfabeto(alfabeto);
    //escrita no arquivo da análise feita
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
