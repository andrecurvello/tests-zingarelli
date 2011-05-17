/*
  ICMC - USP/São Carlos         Entrega: 12/05/2011
  SCC5900-1.2001 - Projeto de Algoritmos
  
  Integrantes:
              Matheus Ricardo Uihara Zingarelli - 5377855
              Raiza Tamae Sarkis Hanada - 7493723
  
  Programa que recebe um texto e faz a análise de um alfabeto, retornando:
           - o tamanho do alfabeto utilizado do texto (Considere apenas palavras
             que contenham apenas letras de A a Z, hífen, e os dígitos de 0 a 9.
             Ignore diferenças entre letras minúsculas e maiúsculas)
           - o alfabeto utilizado, cada símbolo separado por espaço
           - o número de vezes que cada símbolo do alfabeto foi utilizado no texto
           - o número total de palavras no texto (considere palavras com hífen 
             como uma única palavra)
             
  OBSERVAÇÃO[1]: ficou ambíguo nos requisitos se os símbolos que aparecem nas palavras
               consideradas inválidas seriam também contabilizados na frequência.
               Por isso, decidimos que, mesmo a palavra sendo inválida, os símbolos
               nela presentes que sejam válidos são adicionados ao alfabeto e 
               contabilizados na frequência.
  OBSERVAÇÃO[2]: também devido a essa ambiguidade, não sabíamos se as palavras
                inválidas deveriam ser consideradas no total de palavras do texto.
                Com isso, optamos por considerar apenas as palavras válidas para
                serem calculadas no total, e descartamos repetições.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Simbolo{
    int frequencia; //frequência de ocorrência do símbolo em um texto
    char caracter; //símbolo do alfabeto
} simbolo;

typedef struct ShiftSimbolo{
    char caracter; //símbolo do alfabeto
    int shiftStep; //quantidade de símbolos que posso pular durante o string matching
} shiftSimbolo;

typedef struct Palavra{
  char* p; //palavra formada por símbolos do alfabeto presente no texto
  shiftSimbolo shiftTable[4000]; //tabela de shift para cada símbolo de cada palavra TODO: VERIFICAR TAMANHO
  int badSymbolTable [126-32+1];
  int *goodSufixTable;
  int qtdeOcorrencias;
  int tamPalavra; //tamanho da palavra
} palavra;

/*
   Converte todas as letras de uma palavra para minúsculo
   Entrada: palavraValida - palavra a ser convertida
*/
void lowerCase (char *palavraValida){
    int i=0;
    while (palavraValida[i]!='\0'){
    	if ((palavraValida[i]>=65)&&(palavraValida[i]<=90)){
    	    palavraValida[i]= palavraValida[i]+97-65;
        }
    	i++;
    }
}

/*
   Converte um caracter para minúsculo
   Entrada: simbolo - caracter a ser convertido
   Saída: caracter em minúsculo
*/
char lowerCaseChar (char simbolo){
    if ((simbolo>=65)&&(simbolo<=90)){
        simbolo = simbolo + 97 - 65;
    }
    return simbolo;
}

/* 
   Verifica se uma palavra é válida de acordo com os requisitos ([A-Z][a-z][-][0-9])
   Caso a palavra possua um símbolo que não pertença ao intervalo do requisito,
   a palavra não é considerada válida
   Entradas: 
            string - palavra a ser verificada
            pa - recebe a palavra verificada. RAIZA: Serve pra mais alguma coisa?
   Saída: i - 0 caso a palavra não for válida ou o tamanho da palavra c.c.
*/
int verificaPalavraValida(char *string, char *pa){
    int i=0;
    strcpy(pa,"");
    while (string[i]!='\0'){
    	if ((!((string[i]>=48)&&(string[i]<=57)))&&
    	    (!((string[i]>=65)&&(string[i]<=90)))&&
    	    (!((string[i]>=97)&&(string[i]<=122)))&&
    	    (string[i]!=45)){
    	    return 0; //palavra possui caracteres inválidos
        }
    	pa[i] = string[i];
    	i++;	
    }
    pa[i]='\0';
    lowerCase(pa);
    return i;      
}

/* 
   Verifica se um símbolo é válido de acordo com os requisitos ([A-Z][a-z][-][0-9])
   Entrada: símbolo - símbolo a ser verificado
   Saída: 0 caso símbolo não seja válido ou 1 c.c.
*/
int verificaSimboloValido(char simbolo){
    if ((!((simbolo>=48)&&(simbolo<=57)))&&
    	(!((simbolo>=65)&&(simbolo<=90)))&&
    	(!((simbolo>=97)&&(simbolo<=122)))&&
    	(simbolo!=45)) //símbolo é inválido
    	return 0;
    return 1;
}

/*
   Verifica se uma dada palavra já existe em uma lista de palavras
   Entradas:
            stringValida - palavra a ser verificada
            palavrasValidas - lista de palavras
            tam - número de palavras existentes na lista
   Saída: 1 se a palavra já existe. 0 c.c.
*/
int existePalavra(char *stringValida, palavra *palavrasValidas, int tam){
  int i;
  for (i=0;i<tam;i++){
      if (!strcmp(palavrasValidas[i].p,stringValida)){
	  palavrasValidas[i].qtdeOcorrencias++;
	  return 1;    
      } 
  }  
  return 0;
}

/*
   Insere uma palavra na lista de palavras.
   Entradas:
            stringValida - palavra a ser inserida
            palavrasValidas - lista de palavras
            qtde - número de palavras existentes na lista
            tam - tamanho da palavra a ser inserida
*/
void inserePalavra(char *stringValida, palavra *palavrasValidas, int qtde, int tam){
    palavrasValidas[qtde].p = (char*)malloc(sizeof(char) * 40);
    strcpy(palavrasValidas[qtde].p, stringValida);
    palavrasValidas[qtde].tamPalavra = tam;
    palavrasValidas[qtde].qtdeOcorrencias = 1;  
}

/*
  Função que realiza a leitura de um arquivo de texto, contando o número de 
  palavras válidas que este possui. De acordo com os requisitos, uma palavra é 
  válida se contém apenas letras de A a Z, hífen, e os dígitos de 0 a 9, ignorando
  diferenças entre letras minúsculas e maiúsculas.
   Entrada: palavrasValidas - lista de palavras válidas do texto
*/
int leitura (palavra *palavrasValidas){
  int qtde=0;
  int tam;
  FILE *pt;
  pt=fopen("input.txt","r");  
  char string[50],stringValida[50];  
  fscanf(pt,"%s",string); //lê arquivo até encontrar espaço, newline ou tab
  while (fscanf(pt,"%s",string)==1){
      tam = verificaPalavraValida(string, stringValida);
      if (tam>0){ //palavra válida, insere na lista caso ainda não exista
    	  if (!existePalavra(stringValida, palavrasValidas, qtde)){
    	      inserePalavra(stringValida, palavrasValidas, qtde, tam);
    	      qtde++;
    	  }
      }
  }
  fclose(pt); 
  return qtde;
}

/*
   Insere símbolos válidos do alfabeto em uma lista de símbolos, com a frequência
   inicialmente zerada.
   Entrada: alfabeto - lista de símbolos
*/
void inicializaAlfabeto(simbolo *alfabeto){
    int i;
    for (i=0;i<=126-32;i++){
	    alfabeto[i].frequencia=0;
	    alfabeto[i].caracter = i+32; //antes de 32 são os caracteres de controle na tabela ASCII
    }
}

/* 
   Incrementa a frequência de ocorrência de um símbolo na lista de símbolos
   Entradas:
        caracter - símbolo cuja frequência será aumentada
        alfabeto - lista de símbolos
*/
void insereSimbolo(char caracter, simbolo *alfabeto){
    int pos = caracter - 32;
    alfabeto[pos].frequencia++;    
}

/*
   Verifica se determinado símbolo já existe na lista de símbolos
   Entradas:
        caracter - símbolo a ser verificado
        alfabeto - lista de símbolos
   Saída: 0 se não existe, 1 c.c.
*/
int existeSimbolo(char caracter, simbolo *alfabeto){
    int pos = caracter - 32;
    if (alfabeto[pos].frequencia==0) return 0;
    return 1;
}

/*
   Percorre um texto e conta a frequência de ocorrência de cada símbolo válido.
   Entrada: alfabeto - lista de símbolos válidos e suas frequências
   Saída: tam - quantidade de símbolos válidos (tamanho do alfabeto)
*/
int calculaAlfabeto(simbolo *alfabeto){
//int calculaAlfabeto(palavra *palavrasValidas, int qtde, simbolo *alfabeto){
    int tam=0;
    int i=0,j;
    inicializaAlfabeto(alfabeto);
    /* Do jeito que está implementado aqui, estamos descartando quando os símbolos aparecem em palavras inválidas.
    for (j=0;j<qtde;j++){
     for (i=0;i<palavrasValidas[j].tamPalavra;i++) {
      if (!existeSimbolo(palavrasValidas[j].p[i],alfabeto))
          tam++;
      insereSimbolo(palavrasValidas[j].p[i],alfabeto);     
      }
    }*/
    FILE *pt;
    pt=fopen("input.txt","r");
    //tamanho do arquivo
    fseek (pt , 0 , SEEK_END);
    long lSize = ftell (pt);
    rewind (pt);
    
    //armazena o arquivo em memória
    char *buffer = (char*) malloc (sizeof(char)*lSize);
    if (buffer == NULL) {
        printf("Problema de alocacao de memoria");
        exit (2);
    }
    fread (buffer,1,lSize,pt);
    
    //contagem de frequencia de cada simbolo
    char simbolo;
    for(i = 0; i < lSize; i++){
        if(verificaSimboloValido(buffer[i])){
            simbolo = lowerCaseChar(buffer[i]);
            if(!existeSimbolo(simbolo,alfabeto)){
                tam++; //atualiza o tamanho do alfabeto com o novo símbolo
            }
            insereSimbolo(simbolo,alfabeto);
        }
    }
    
    fclose(pt);
    free(buffer);
    return tam; 
}

/* 
   Imprime a lista de símbolos e respectiva frequência do alfabeto utilizado
   Entrada: alfabeto - lista de símbolos
*/
void imprimeAlfabeto(simbolo *alfabeto){
    int i;
    for (i=0;i<=126-32;i++)
	if (alfabeto[i].frequencia>0)
	    printf ("%c = %d\n",alfabeto[i].caracter,alfabeto[i].frequencia);
}

/*
   Gera um arquivo com os dados da análise do texto:
        - o tamanho do alfabeto utilizado do texto
        - o alfabeto utilizado, cada símbolo separado por espaço
        - o número de vezes que cada símbolo do alfabeto foi utilizado no texto
        - o número total de palavras válidas no texto (palavras com hífen são 
          consideradas como uma única palavra)
*/
void escreve(const int tamAlfabeto,simbolo *alfabeto, const int qtdePalavras, char* arquivo){
    int i;
    FILE *pt;
    pt=fopen(arquivo,"w");  
    //tamanho do alfabeto
    fprintf(pt,"\n%d\n",tamAlfabeto);
    //símbolos do alfabeto
    for (i=1;i<=126-32;i++){
        if (alfabeto[i].frequencia>0)
            fprintf(pt,"%c ",alfabeto[i].caracter);
    }
    //frequência de cada símbolo
    for (i=1;i<=126-32;i++){
        if (alfabeto[i].frequencia>0)
            fprintf(pt,"\n%c-%d",alfabeto[i].caracter,alfabeto[i].frequencia);  
    }
    //quantidade de palavras válidas
    fprintf(pt,"\n%d\n\n",qtdePalavras);
    fclose(pt);
}
/*
   Imprime a lista de palavras válidas.
   Entradas:
        palavrasValidas - lista de palavras
        tam - quantidade de palavras na lista
*/
void imprimePalavrasValidas(palavra *palavrasValidas, const int tam){
  int i;
  for (i=0;i<tam;i++){
      printf ("%s - %d\n",palavrasValidas[i].p,palavrasValidas[i].tamPalavra);
  }  
}

/*
    Constrói a tabela de shift (quantidade de caracteres que podem ser pulados
    durante o string matching) para cada palavra da lista de palavras
    Entradas:
        palavrasValidas - lista de palavras válidas da leitura de um texto
        tam - quantidade de palavras na lista
*/
void constroiShiftTable(palavra *palavrasValidas, int tam){
    int i, j;
    for(i=0; i<tam; i++){ //para cada palavra
        //inicia a tabela de shift dessa palavra
        for (j=0;j<=126-32;j++){
	       palavrasValidas[i].shiftTable[j].caracter = j+32; //antes de 32 são os caracteres de controle na tabela ASCII
   	       palavrasValidas[i].shiftTable[j].shiftStep= palavrasValidas[i].tamPalavra;
        }
        //atualiza os simbolos que terão tamanho de shift diferenciado
        for(j=0; j<palavrasValidas[i].tamPalavra-1;j++){ //para cada símbolo da palavra menos a última
            palavrasValidas[i].shiftTable[palavrasValidas[i].p[j]-32].shiftStep = palavrasValidas[i].tamPalavra - 1 - j;
        }        
    }
}

/*
    Imprime a tabela de shift para cada palavra.
    Entradas:
        palavrasValidas - lista de palavras válidas da leitura de um texto
        tam - quantidade de palavras na lista
*/
void imprimeShiftTable(palavra *palavrasValidas, int tam){
    int i, j;
    for(i=0; i<tam; i++){ //para cada palavra
        printf("\nPalavra %s\n", palavrasValidas[i].p);
        for(j=0; j<=126-32;j++){ //para cada símbolo do alfabeto
            printf("%c - %d | ", palavrasValidas[i].shiftTable[j].caracter, palavrasValidas[i].shiftTable[j].shiftStep);
        }        
    }
}


void constroiBadSymbolTable(palavra *palavrasValidas, int qtde){
  int i,j,k;
  for (i=0;i<qtde;i++){
      for (j=0;j<126-32+1;j++)      
	  palavrasValidas[i].badSymbolTable[j]=palavrasValidas[i].tamPalavra;
      k=0;
      for (j=palavrasValidas[i].tamPalavra-1;j>0;j--){
	  palavrasValidas[i].badSymbolTable[palavrasValidas[i].p[k]-32]=j;
	  k++;
      }
  }
}

void imprimeBadSymbolTable(palavra *palavrasValidas, int qtde){
  int i,j,k;
  char simbolo;
  for (i=0;i<qtde;i++){
      for (j=0;j<126-32+1;j++)      {
	      simbolo = j;
	      if ((!((simbolo>=48)&&(simbolo<=57)))&&
		  (!((simbolo>=65)&&(simbolo<=90)))&&
		  (!((simbolo>=97)&&(simbolo<=122)))&&
		  (simbolo!=45)) //símbolo é inválido
		  ;
	      else printf("%c - %d\n",j,palavrasValidas[i].badSymbolTable[j]);
      }
  }
}

int substring(char* p, int pos, int tam){
  int i,j,k;
  for (i=pos-1+(tam-1)-pos;i>=0;i--){
      k=i;
      for (j=tam-1;j>=pos;j--){
	  if ((p[k]!=p[j])||(k<0)){
	     break;}
	  else k--;
      }      
      
      if ((j<pos)||(k<0))
	  return tam-1-i;	
  }
  return -1;  
}

void constroiGoodSufixTable(palavra *palavrasValidas, int qtde){
  int i,j,k;
  for (i=0;i<qtde;i++){	
      palavrasValidas[i].goodSufixTable = (int*)malloc(sizeof(int)*palavrasValidas[i].tamPalavra+1);
      for (j=0;j<palavrasValidas[i].tamPalavra-1;j++){
	   k= substring(palavrasValidas[i].p,palavrasValidas[i].tamPalavra-1-j,palavrasValidas[i].tamPalavra);
	   if (k==-1) 
	      palavrasValidas[i].goodSufixTable[j]=palavrasValidas[i].tamPalavra;
	   else 
	      palavrasValidas[i].goodSufixTable[j]=k;
      }  
  }
}

void imprimeGoodSufixTable(palavra *palavrasValidas, int qtde){
  int i,j,k;
  for (i=0;i<qtde;i++){	
      for (j=0;j<palavrasValidas[i].tamPalavra-1;j++){
	printf("%d ", palavrasValidas[i].goodSufixTable[j]);
      }  
      printf("\n");
  }
}

void imprimeTamPalavrasValidas(palavra *palavrasValidas,const int qtde, char* arquivo){
    int i;
    FILE *pt;
    pt=fopen(arquivo,"w");  
    //tamanho do alfabeto
    fprintf(pt,"tam\n");
    for (i=0;i<qtde;i++)
        if(palavrasValidas[i].tamPalavra>=2)
	    fprintf (pt,"%d\n",palavrasValidas[i].tamPalavra);
    fprintf(pt,"\n");
    fclose (pt);
}

void ordenaFrequenciaPalavrasValidas(palavra *palavrasValidas,const int qtde){
  int i, j, min;
  palavra aux;
  for (i = 0; i < qtde-1; i++) {    
    min=i;
    for (j = (i+1); j < qtde; j++) {
	if(palavrasValidas[j].qtdeOcorrencias < palavrasValidas[min].qtdeOcorrencias)
	    min = j;	
    }
    if (i != min) {
      aux = palavrasValidas[i];
      palavrasValidas[i] = palavrasValidas[min];
      palavrasValidas[min] = aux;
    }
  } 
}

void imprime100MaisFrequentesPalavrasValidas(palavra *palavrasValidas, int qtde, char* arquivo){
    int i,count=0;
    FILE *pt;
    pt=fopen(arquivo,"w");  
    //tamanho do alfabeto
    fprintf(pt,"qtdeOcorrencias\n");
    for (i=qtde-1;(i>=0) && (count<100);i--){
      if(palavrasValidas[i].tamPalavra>=2){
	    fprintf (pt,"%d\n",palavrasValidas[i].qtdeOcorrencias);
	    count++;
	}
    }
    fprintf(pt,"\n");
    fclose (pt);
}

void imprime100MenosFrequentesPalavrasValidas(palavra *palavrasValidas, int qtde, char* arquivo){
    int i,count=0;
    FILE *pt;
    pt=fopen(arquivo,"w");  
    //tamanho do alfabeto
    fprintf(pt,"qtdeOcorrencias\n");
    for (i=0;(i<qtde) && (count<100);i++){
        if(palavrasValidas[i].tamPalavra>=2){
	    fprintf (pt,"%d\n",palavrasValidas[i].qtdeOcorrencias);
	    count++;	
	}
    }
    fprintf(pt,"\n");
    fclose (pt);
}