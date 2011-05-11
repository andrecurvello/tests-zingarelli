/*
  ICMC - USP/S�o Carlos         Entrega: 12/05/2011
  SCC5900-1.2001 - Projeto de Algoritmos
  
  Integrantes:
              Matheus Ricardo Uihara Zingarelli - 5377855
              Raiza Tamae Sarkis Hanada - 7493723
  
  Programa que recebe um texto e faz a an�lise de um alfabeto, retornando:
           - o tamanho do alfabeto utilizado do texto (Considere apenas palavras
             que contenham apenas letras de A a Z, h�fen, e os d�gitos de 0 a 9.
             Ignore diferen�as entre letras min�sculas e mai�sculas)
           - o alfabeto utilizado, cada s�mbolo separado por espa�o
           - o n�mero de vezes que cada s�mbolo do alfabeto foi utilizado no texto
           - o n�mero total de palavras no texto (considere palavras com h�fen 
             como uma �nica palavra)
             
  OBSERVA��O[1]: ficou amb�guo nos requisitos se os s�mbolos que aparecem nas palavras
               consideradas inv�lidas seriam tamb�m contabilizados na frequ�ncia.
               Por isso, decidimos que, mesmo a palavra sendo inv�lida, os s�mbolos
               nela presentes que sejam v�lidos s�o adicionados ao alfabeto e 
               contabilizados na frequ�ncia.
  OBSERVA��O[2]: tamb�m devido a essa ambiguidade, n�o sab�amos se as palavras
                inv�lidas deveriam ser consideradas no total de palavras do texto.
                Com isso, optamos por considerar apenas as palavras v�lidas para
                serem calculadas no total.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Simbolo{
    int frequencia; //frequ�ncia de ocorr�ncia do s�mbolo em um texto
    char caracter; //s�mbolo do alfabeto
} simbolo;

typedef struct ShiftSimbolo{
    char caracter; //s�mbolo do alfabeto
    int shiftStep; //quantidade de s�mbolos que posso pular durante o string matching
} shiftSimbolo;

typedef struct Palavra{
  char* p; //palavra formada por s�mbolos do alfabeto presente no texto
  shiftSimbolo shiftTable[4000]; //tabela de shift para cada s�mbolo de cada palavra TODO: VERIFICAR TAMANHO
  int tamPalavra; //tamanho da palavra
} palavra;

/*
   Converte todas as letras de uma palavra para min�sculo
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
   Converte um caracter para min�sculo
   Entrada: simbolo - caracter a ser convertido
   Sa�da: caracter em min�sculo
*/
char lowerCaseChar (char simbolo){
    if ((simbolo>=65)&&(simbolo<=90)){
        simbolo = simbolo + 97 - 65;
    }
    return simbolo;
}

/* 
   Verifica se uma palavra � v�lida de acordo com os requisitos ([A-Z][a-z][-][0-9])
   Entradas: 
            string - palavra a ser verificada
            pa - recebe a palavra verificada. RAIZA: Serve pra mais alguma coisa?
   Sa�da: i - 0 caso a palavra n�o for v�lida ou o tamanho da palavra c.c.
*/
int verificaPalavraValida(char *string, char *pa){
    int i=0;
    strcpy(pa,"");
    while (string[i]!='\0'){
    	if ((!((string[i]>=48)&&(string[i]<=57)))&&
    	    (!((string[i]>=65)&&(string[i]<=90)))&&
    	    (!((string[i]>=97)&&(string[i]<=122)))&&
    	    (string[i]!=45)){
    	    break; //palavra possui caracteres inv�lidos
        }
    	pa[i] = string[i];
    	i++;	
    }
    pa[i]='\0';
    lowerCase(pa);
    return i;      
}

/* 
   Verifica se um s�mbolo � v�lido de acordo com os requisitos ([A-Z][a-z][-][0-9])
   Entrada: s�mbolo - s�mbolo a ser verificado
   Sa�da: 0 caso s�mbolo n�o seja v�lido ou 1 c.c.
*/
int verificaSimboloValido(char simbolo){
    if ((!((simbolo>=48)&&(simbolo<=57)))&&
    	(!((simbolo>=65)&&(simbolo<=90)))&&
    	(!((simbolo>=97)&&(simbolo<=122)))&&
    	(simbolo!=45)) //s�mbolo � inv�lido
    	return 0;
    return 1;
}

/*
   Verifica se uma dada palavra j� existe em uma lista de palavras
   Entradas:
            stringValida - palavra a ser verificada
            palavrasValidas - lista de palavras
            tam - n�mero de palavras existentes na lista
   Sa�da: 1 se a palavra j� existe. 0 c.c.
*/
int existePalavra(char *stringValida, palavra *palavrasValidas, int tam){
  int i;
  for (i=0;i<tam;i++){
      if (!strcmp(palavrasValidas[i].p,stringValida))
	  return 1;    
  }  
  return 0;
}

/*
   Insere uma palavra na lista de palavras.
   Entradas:
            stringValida - palavra a ser inserida
            palavrasValidas - lista de palavras
            qtde - n�mero de palavras existentes na lista
            tam - tamanho da palavra a ser inserida
*/
void inserePalavra(char *stringValida, palavra *palavrasValidas, int qtde, int tam){
    //TODO: definir o tamanho m�ximo da palavra
    palavrasValidas[qtde].p = (char*)malloc(sizeof(char) * 40);
    strcpy(palavrasValidas[qtde].p, stringValida);
    palavrasValidas[qtde].tamPalavra = tam;  
}

/*
  Fun��o que realiza a leitura de um arquivo de texto, contando o n�mero de 
  palavras v�lidas que este possui. De acordo com os requisitos, uma palavra � 
  v�lida se cont�m apenas letras de A a Z, h�fen, e os d�gitos de 0 a 9, ignorando
  diferen�as entre letras min�sculas e mai�sculas.
   Entradas: 
             texto - RAIZA: serve para que este??
             palavrasValidas - lista de palavras v�lidas do texto
*/
int leitura (char *texto, palavra *palavrasValidas){
  int qtde=0;
  int tam;
  FILE *pt;
  pt=fopen("input.txt","r");  
  //TODO: definir o tamanho m�ximo da palavra
  char *string = (char*)malloc(sizeof(char)*40);  
  char *stringValida = (char*)malloc(sizeof(char)*40);  
  while (!feof(pt)){
      fscanf(pt,"%s",string); //l� arquivo at� encontrar espa�o, newline ou tab
      strcat (texto,string);
      strcat (texto," ");
      tam = verificaPalavraValida(string, stringValida);
      if (tam>0){ //palavra v�lida, insere na lista caso ainda n�o exista
    	  if (!existePalavra(stringValida, palavrasValidas, qtde)){
    	      inserePalavra(stringValida, palavrasValidas, qtde, tam);
    	      qtde++;
    	  }
      }
  }
  fclose(pt); 
  free(string); 
  free(stringValida);
  return qtde;
}

/*
   Insere s�mbolos v�lidos do alfabeto em uma lista de s�mbolos, com a frequ�ncia
   inicialmente zerada.
   Entrada: alfabeto - lista de s�mbolos
*/
void inicializaAlfabeto(simbolo *alfabeto){
    int i;
    for (i=0;i<=126-32;i++){
	    alfabeto[i].frequencia=0;
	    alfabeto[i].caracter = i+32; //antes de 32 s�o os caracteres de controle na tabela ASCII
    }
}

/* 
   Incrementa a frequ�ncia de ocorr�ncia de um s�mbolo na lista de s�mbolos
   Entradas:
        caracter - s�mbolo cuja frequ�ncia ser� aumentada
        alfabeto - lista de s�mbolos
*/
void insereSimbolo(char caracter, simbolo *alfabeto){
    int pos = caracter - 32;
    alfabeto[pos].frequencia++;    
}

/*
   Verifica se determinado s�mbolo j� existe na lista de s�mbolos
   Entradas:
        caracter - s�mbolo a ser verificado
        alfabeto - lista de s�mbolos
   Sa�da: 0 se n�o existe, 1 c.c.
*/
int existeSimbolo(char caracter, simbolo *alfabeto){
    int pos = caracter - 32;
    if (alfabeto[pos].frequencia==0) return 0;
    return 1;
}

/*
   Percorre um texto e conta a frequ�ncia de ocorr�ncia de cada s�mbolo v�lido.
   Entrada: alfabeto - lista de s�mbolos v�lidos e suas frequ�ncias
   Sa�da: tam - quantidade de s�mbolos v�lidos (tamanho do alfabeto)
*/
int calculaAlfabeto(simbolo *alfabeto){
//int calculaAlfabeto(palavra *palavrasValidas, int qtde, simbolo *alfabeto){
    int tam=0;
    int i=0,j;
    inicializaAlfabeto(alfabeto);
    /* Do jeito que est� implementado aqui, estamos descartando quando os s�mbolos aparecem em palavras inv�lidas.
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
    
    //armazena o arquivo em mem�ria
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
                tam++; //atualiza o tamanho do alfabeto com o novo s�mbolo
            }
            insereSimbolo(simbolo,alfabeto);
        }
    }
    
    fclose(pt);
    free(buffer);
    return tam; 
}

/* 
   Imprime a lista de s�mbolos e respectiva frequ�ncia do alfabeto utilizado
   Entrada: alfabeto - lista de s�mbolos
*/
void imprimeAlfabeto(simbolo *alfabeto){
    int i;
    for (i=0;i<=126-32;i++)
	if (alfabeto[i].frequencia>0)
	    printf ("%c = %d\n",alfabeto[i].caracter,alfabeto[i].frequencia);
}

/*
   Gera um arquivo com os dados da an�lise do texto:
        - o tamanho do alfabeto utilizado do texto
        - o alfabeto utilizado, cada s�mbolo separado por espa�o
        - o n�mero de vezes que cada s�mbolo do alfabeto foi utilizado no texto
        - o n�mero total de palavras v�lidas no texto (palavras com h�fen s�o 
          consideradas como uma �nica palavra)
*/
void escreve(const int tamAlfabeto,simbolo *alfabeto, const int qtdePalavras){
    int i;
    FILE *pt;
    pt=fopen("output.txt","a");  
    //tamanho do alfabeto
    fprintf(pt,"\n%d\n",tamAlfabeto);
    //s�mbolos do alfabeto
    for (i=1;i<=126-32;i++){
        if (alfabeto[i].frequencia>0)
            fprintf(pt,"%c ",alfabeto[i].caracter);
    }
    //frequ�ncia de cada s�mbolo
    for (i=1;i<=126-32;i++){
        if (alfabeto[i].frequencia>0)
            fprintf(pt,"\n%c-%d",alfabeto[i].caracter,alfabeto[i].frequencia);  
    }
    //quantidade de palavras v�lidas
    fprintf(pt,"\n%d\n\n",qtdePalavras);
    fclose(pt);
}
/*
   Imprime a lista de palavras v�lidas.
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
    Constr�i a tabela de shift (quantidade de caracteres que podem ser pulados
    durante o string matching) para cada palavra da lista de palavras
    Entradas:
        palavrasValidas - lista de palavras v�lidas da leitura de um texto
        tam - quantidade de palavras na lista
*/
void constroiShiftTable(palavra *palavrasValidas, int tam){
    int i, j;
    for(i=0; i<tam; i++){ //para cada palavra
        //inicia a tabela de shift dessa palavra
        for (j=0;j<=126-32;j++){
	       palavrasValidas[i].shiftTable[j].caracter = j+32; //antes de 32 s�o os caracteres de controle na tabela ASCII
   	       palavrasValidas[i].shiftTable[j].shiftStep= palavrasValidas[i].tamPalavra;
        }
        //atualiza os simbolos que ter�o tamanho de shift diferenciado
        for(j=0; j<palavrasValidas[i].tamPalavra-1;j++){ //para cada s�mbolo da palavra menos a �ltima
            palavrasValidas[i].shiftTable[palavrasValidas[i].p[j]-32].shiftStep = palavrasValidas[i].tamPalavra - 1 - j;
        }        
    }
}

/*
    Imprime a tabela de shift para cada palavra.
    Entradas:
        palavrasValidas - lista de palavras v�lidas da leitura de um texto
        tam - quantidade de palavras na lista
*/
void imprimeShiftTable(palavra *palavrasValidas, int tam){
    int i, j;
    for(i=0; i<tam; i++){ //para cada palavra
        printf("\nPalavra %s\n", palavrasValidas[i].p);
        for(j=0; j<=126-32;j++){ //para cada s�mbolo do alfabeto
            printf("%c - %d | ", palavrasValidas[i].shiftTable[j].caracter, palavrasValidas[i].shiftTable[j].shiftStep);
        }        
    }
}

/* Chamada do Programa */
/*int main(){
    int i;
    
    char *texto = (char*)malloc(sizeof(char)*4000); 
    //TODO:decidir o tamanho m�ximo da lista de palavras  
    palavra *palavrasValidas = (palavra*)malloc(sizeof(palavra)*4000);     
    simbolo *alfabeto = (simbolo*)malloc(sizeof(simbolo)*126-32+1); //armazena os s�mbolos de 32 a 126 da tabela ASCII
    
    //C�lculo do n�mero total de palavras v�lidas no texto
    const int QTDE_PALAVRAS = leitura (texto,palavrasValidas);
    
    //debug -->
    //printf ("Qtde de palavras validas encontradas = %d\n", QTDE_PALAVRAS);  
    //imprimePalavrasValidas(palavrasValidas,QTDE_PALAVRAS);
    //<-- debug
    
    //An�lise do alfabeto e contagem de frequ�ncia de s�mbolos
    //const int TAM_ALFABETO = calculaAlfabeto(palavrasValidas, QTDE_PALAVRAS, alfabeto);
    const int TAM_ALFABETO = calculaAlfabeto(alfabeto);
    
    //debug -->
    //imprimeAlfabeto(alfabeto);
    //<-- debug
    
    escreve(TAM_ALFABETO,alfabeto,QTDE_PALAVRAS);
    
    constroiShiftTable(palavrasValidas, QTDE_PALAVRAS);
    
    //debug -->
    //imprimeShiftTable(palavrasValidas, QTDE_PALAVRAS);
    //<-- debug
    
    free(texto);
    free(palavrasValidas);
    free(alfabeto);
    return 0;
  
}*/
