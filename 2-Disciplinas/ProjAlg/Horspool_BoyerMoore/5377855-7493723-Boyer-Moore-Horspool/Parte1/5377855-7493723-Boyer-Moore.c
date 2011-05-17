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
  
  OBSERVAÇÕES:
        - Copie o texto e cole em um editor de textos simples, como o Notepad
    do Windows, sem quebra de linha. Foi observado que textos extraídos de .zip
    ou de editores que fazem quebra de linha adicionam caracteres a mais que irão
    atrapalhar na hora de dizer onde começa cada ocorrência da palavra no texto.
        - O programa possui uma limitação de entrada de no máximo 60000 palavras
    válidas possíveis, bem como no máximo 500000 ocorrências de cada uma delas.
*/
#include <stdio.h>
#include <time.h>
#include "alphabet_analysis.c"

int maior(int num1, int num2){
    if (num1>num2)
	return num1;
    return num2;
}

/*
    Utiliza o algoritmo de Boyer Moore para encontrar o número de ocorrências e a
    posição de cada ocorrência de cada palavra na lista de palavras em relação
    a um texto.
    Entradas:
        listaPalavras - lista de palavras
        qtde - quantidade de palavras na lista
*/
void boyerMoore(palavra *listaPalavras, int qtde){    
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
    
    //Tratamento no texto
   int a,b,count=0;
   a=0; 
   for (b=0;b<tamTexto;b++){
	if ((texto[b]>=1)&&(texto[b]<=256)){
	    texto[a]=texto[b]; a++; //printf("%c",texto[a]); a++;    
	}
	if (b<tamTexto-2){
	    //if (!((texto[b]>=1)&&(texto[b]<=256))&&!((texto[b+1]>=1)&&(texto[b+1]<=256))){
	    if (!((texto[b]>=1)&&(texto[b]<=256))){
		//texto[a]=texto[b]; printf("%c.",texto[b]); a++;count++; 		
		texto[a]=-1; a++; //printf ("%c",texto[a]); a++;
	    }
	}
   }
   texto[a]='\0';
   tamTexto-=count;
   
   
       //cria tabelas
    constroiBadSymbolTable(listaPalavras, qtde);   
    constroiGoodSufixTable(listaPalavras, qtde);

    int posicoes[500000]; //índices em que a palavra ocorre no texto  
    
    
    //faz a busca de cada palavra no texto
    pt=fopen("5377855-7493726-output-Boyer-Moore.txt","a");

    char caracter;
    int i,j,t,p,d,tam,k,t1,d2,d1,qtde_achou;
    for (i=0;i<qtde;i++){
	tam =listaPalavras[i].tamPalavra;
	if(tam>=2){
            //grava no arquivo a palavra
            fprintf(pt,"%s\n",listaPalavras[i].p);
            //grava no arquivo a hora do dia
            time_t rawtime;
            struct tm * timeinfo;        
            time(&rawtime);
            timeinfo = localtime(&rawtime);
            fprintf (pt,"%s", asctime(timeinfo));
				//inicia algoritmo
				t=tam-1;
				qtde_achou=0;
				while (t<tamTexto){
				p=tam-1; k=t;
				while (lowerCaseChar(texto[k])==listaPalavras[i].p[p] && (p>=0)){
					p--;k--;
				}
				if (p<0){
					posicoes[qtde_achou]=t-(tam-1);
					qtde_achou++; d=1;
				}
				else {	
						k=tam-1-p;   //qntde de acerto	= posicao de t menos a posicao do q deu o miss

						caracter = texto[t-k];
						if ((caracter>=65)&&(caracter<=90))
							  caracter = caracter+32;				
		
						if ((!((caracter>=48)&&(caracter<=57)))&& (!((caracter>=65)&&(caracter<=90)))&&
							(!((caracter>=97)&&(caracter<=122)))&& (caracter!=45))
							  t1 = listaPalavras[i].tamPalavra;
						else t1 = listaPalavras[i].badSymbolTable[caracter-32];
		
						d1 = maior(t1-k,1);
						if (k>0){
							d2 = listaPalavras[i].goodSufixTable[k-1];
							d = maior(d1,d2);
						}
						else d=d1;
						/*if (!(strcmp(listaPalavras[i].p,"da")))
								printf ("p:%s, t:%d, caracter:%c, d1:%d, d2:%d, t:%d, d:%d, k:%d\n",listaPalavras[i].p,t,caracter,d1,d2,t,d,k);*/
					}
				t=t+d;
				}
				//fim do algoritmo
				    //grava no arquivo quantas vezes encontrou a palavra	    
				    fprintf(pt,"%d\n",qtde_achou);
				    //grava no arquivo as posicoes em que a palavra foi encontrada
				for (j=0;j<qtde_achou;j++){
				fprintf(pt,"%d ",posicoes[j]);	      
				}
				    //grava no arquivo a hora do dia
				    time(&rawtime);
				    timeinfo = localtime(&rawtime);
				    fprintf (pt,"\n%s\n", asctime(timeinfo));

			}
    } 

    free(texto);
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
    strcpy(arquivo,"5377855-7493726-output-Boyer-Moore.txt");   
    escreve(TAM_ALFABETO,alfabeto,QTDE_PALAVRAS,arquivo);
     
    //---------String Matching
    boyerMoore(palavrasValidas, QTDE_PALAVRAS);
    
    free(palavrasValidas);
    free(alfabeto);
    return 0;
}
