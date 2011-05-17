#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include "alphabet_analysis.c"

double tempo()
{
 struct timeval tv;
 gettimeofday(&tv,0);
 return (tv.tv_sec + tv.tv_usec/1e6)*1000;
}

void horspool(palavra *listaPalavras, int qtde, char *texto, int tamTexto){
    //Pre-processamento - Construção da tabela de shift
    constroiShiftTable(listaPalavras, qtde);
    
    FILE *pt;
    pt=fopen("parte2c.txt","a");
    fprintf (pt,"Horspool\n");
    
    double tempo1, tempo2;    
    //faz a busca de cada palavra no texto
    int i, j=0, ocorrencia,countPalavras=0;
    int posicoes[500000]; //índices em que a palavra ocorre no texto  
    for(i=0; i<qtde && countPalavras<100; i++){//para cada palavra
        if(listaPalavras[i].tamPalavra>=2){  
	    countPalavras++;
	    //Pega a hora inicial
	    tempo1 = tempo();
	    //Realiza a busca
	    ocorrencia = 0;
            int j = listaPalavras[i].tamPalavra-1; 	//guarda a posição para o último caracter da palavra
            while(j < tamTexto){ 			//enquanto não atingiu o fim do texto
                int k = 0; 				//número de caracteres que deram match
		//enquanto houver matching de cada caracter
                while(k < listaPalavras[i].tamPalavra && listaPalavras[i].p[listaPalavras[i].tamPalavra - k - 1] == lowerCaseChar(texto[j-k])){
                    k++;
                }
                if(k == listaPalavras[i].tamPalavra){	//matching de palavra
                    posicoes[ocorrencia] = j - k + 1;
                    ocorrencia++;                    
                    j++; 				//shift para continuar procurando mais ocorrências
                }	
                else{
                    if(texto[j]<32 || texto[j]>126)
                        j += listaPalavras[i].tamPalavra;
                    else
                        j += listaPalavras[i].shiftTable[lowerCaseChar(texto[j])-32].shiftStep;
                    
                }
            }
	    //Pega a hora atual
	    tempo2 = tempo();
            fprintf(pt,"%lf\n",tempo2-tempo1);
        }
    }
    fprintf (pt,"\n");
    fclose(pt);    
}

int maior(int num1, int num2){
    if (num1>num2)
	return num1;
    return num2;
}

void boyerMoore(palavra *listaPalavras, int qtde, char* texto, int tamTexto){    
    //Tratamento no texto
   int a,b,count=0;
   a=0; 
   for (b=0;b<tamTexto;b++){
	if ((texto[b]>=1)&&(texto[b]<=256)){
	    texto[a]=texto[b]; a++; //printf("%c",texto[a]); a++;    
	}
	if (b<tamTexto-2){
	    if (!((texto[b]>=1)&&(texto[b]<=256))){
		texto[a]=-1; a++;
	    }
	}
   }
   texto[a]='\0';
   tamTexto-=count;
      
    //cria tabelas
    constroiBadSymbolTable(listaPalavras, qtde);   
    constroiGoodSufixTable(listaPalavras, qtde);

    int posicoes[500000]; //índices em que a palavra ocorre no texto  
    int countPalavras=0;
    
    //faz a busca de cada palavra no texto
    FILE *pt;
    pt=fopen("parte2c.txt","a");
    
    fprintf (pt,"BoyerMoore\n");
    
    double tempo1, tempo2;
    char caracter;
    int i,j,t,p,d,tam,k,t1,d2,d1,qtde_achou;
    for (i=0;i<qtde && countPalavras<100;i++){
	tam =listaPalavras[i].tamPalavra;
	if(tam>=2){
	    countPalavras++;
	    //Pega a hora inicial
	    tempo1 = tempo();
	    //inicia algoritmo
	    t=tam-1;
	    qtde_achou=0;
	    while (t<tamTexto){
                int k = 0; //número de caracteres que deram match
                //enquanto houver matching de cada caracter
                while(k < listaPalavras[i].tamPalavra && listaPalavras[i].p[listaPalavras[i].tamPalavra - k - 1] == lowerCaseChar(texto[t-k])){
                    k++;
                }
                if(k == listaPalavras[i].tamPalavra){//matching de palavra
		    posicoes[qtde_achou]=t-k+1;
		    qtde_achou++; t++;
                }
                else{
                    if(texto[t-k]<32 || texto[t-k]>126)
			t1 = listaPalavras[i].tamPalavra;
		    else t1 = listaPalavras[i].badSymbolTable[lowerCaseChar(texto[t-k])-32];
		    d1 = maior(t1-k,1);
		    if (k>0){
			d2 = listaPalavras[i].goodSufixTable[k-1];
			d = maior(d1,d2);
		    }
		    else d=d1;
		    t=t+d;
                }
	    }	    //Pega a hora atual
	    tempo2 = tempo();
            fprintf(pt,"%lf\n",tempo2-tempo1);
	}
    }
    fclose(pt);    
}


int main(int argc, char*argv[]){
    //---------Análise do alfabeto
    palavra *palavrasValidas = (palavra*)malloc(sizeof(palavra)*1000000);     
    simbolo *alfabeto = (simbolo*)malloc(sizeof(simbolo)*126-32+1); //armazena os símbolos de 32 a 126 da tabela ASCII
    
    //Cálculo do número total de palavras válidas no texto
    const int QTDE_PALAVRAS = leitura (palavrasValidas);
    //contagem de frequência de símbolos
    const int TAM_ALFABETO = calculaAlfabeto(alfabeto);
    //escrita no arquivo da análise feita
    char *arquivo=(char*)malloc(sizeof(char)*45);
    strcpy(arquivo,"parte2c.txt");   

    ordenaFrequenciaPalavrasValidas(palavrasValidas,QTDE_PALAVRAS);
    imprime100MenosFrequentesPalavrasValidas(palavrasValidas,QTDE_PALAVRAS,arquivo);   
    
    //armazena arquivo de texto em memória
    FILE *pt;
    pt=fopen("input.txt","rb");
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
    
    horspool(palavrasValidas,QTDE_PALAVRAS,texto,tamTexto);
    boyerMoore(palavrasValidas,QTDE_PALAVRAS,texto,tamTexto);    
    
    free(arquivo);
    free(palavrasValidas);
    free(alfabeto);
    return 0;
}