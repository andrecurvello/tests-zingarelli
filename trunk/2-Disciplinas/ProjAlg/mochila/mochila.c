/*--------------------
  SCC5900 - Projeto de algoritmos       05-abril-2011
  Matheus Ricardo Uihara Zingarelli nUSP: 5377855
  Algoritmos de for�a bruta
  
  Descri��o:
    Problema da mochila. Dados n itens, cada qual com seu respectivo peso e valor,
    maximizar a quantidade de itens valiosos que cabem em uma mochila que suporta
    um peso especificado. Cada item pode ser colocado apenas uma vez
   
  Complexidade: O(2^n)
  
  Entrada: arquivo texto no formato:
           peso_mochila
           numero_itens
           id_item_1 peso_item_1 valor_item_1
           .
           .
           .
           id_item_n peso_item_n valor_item_n
           
  Sa�da: sa�da na linha de comando com os ids dos itens que maximizam o que cabe 
         na mochila, um por linha
--------------------*/

#include <stdio.h>
#include <math.h>

typedef struct item{
           int ID;
           int peso;
           int valor;
    } Item;

/*
Converte um n�mero decimal em seu respectivo valor bin�rio. Armazena o valor
em um vetor, em que cada posi��o pode ser 0 ou 1, formando o bin�rio em sua
ordem inversa, ou seja, o bin�rio correspondente ao n�mero � formado indo do
fim ao come�o do vetor.
Entradas: vet - vetor contendo uma combina��o de itens
          decimal
*/
void dec2bin(int *vet, int decimal){
     int i = 0;
     int result = 0;
     while(decimal / 2 != 0){
         vet[i] = decimal % 2;
         i++;
         decimal /= 2;
     }
     vet[i] = decimal % 2;
}

/*
Dada uma combina��o de itens a serem colocados na mochila, calcula o peso e o 
valor total. Um item se encontra na mochila se ele tiver valor 1 no vetor vet
Entradas: itens - vetor que cont�m os pesos e valores de cada item
          vet - vetor contendo uma combina��o de itens
          peso - armazena o peso total desta combina��o
          valor - armazena o valor total desta combina��o
          nItens - n�mero de itens a serem combinados
*/
void calculaCombinacao(Item *itens, int *vet, int *peso, int *valor, int nItens){
     int i;
     for(i = 0; i < nItens; i++){
           if(vet[i] == 1){
               *peso += itens[i].peso;
               *valor += itens[i].valor;               
           }
     }
}

int main(int argc, char *argv[]){
    int pMochila, nItens;    
    int totalPeso = 0;
    int valorParcial = 0;
    int valorTotal = 0;
    
    /* ler dados do arquivo e criar matriz de adjac�ncias */
    FILE *fp;
    fp = fopen(argv[1], "r");
    fscanf(fp, "%d", &pMochila);
    fscanf(fp, "%d", &nItens);
    
    int i;
    int vet[nItens]; //armazena uma combinacao de itens (0: item ausente; 1: item presente)
    int result[nItens]; //armazena melhor combina��o
    Item itens[nItens];
    
    for(i = 0; i < nItens; i++){
        vet[i] = 0; //inicia vetor
        //l� dados dos itens
        fscanf(fp, "%d %d %d", &itens[i].ID, &itens[i].peso, &itens[i].valor);
    }
    
    fclose(fp);
        
    /*for�a bruta, gera todas as combina��es de itens poss�veis*/
    for(i = 0; i < pow(2, nItens); i++){    
        /* dec2bin armazena o valor bin�rio de i em vet. Cada posi��o do vetor 
        est� sendo utilizada para simular se um item se encontra (vet[i]==1) ou
        n�o (vet[i]==0) na mochila.
        */
        dec2bin(vet, i);
        calculaCombinacao(itens, vet, &totalPeso, &valorParcial, nItens);
        if(totalPeso <= pMochila){
           if(valorParcial > valorTotal){
              valorTotal = valorParcial;
              int j;
              for(j = 0; j < nItens; j++){
                  result[j] = vet[j];
              }
           }             
        }                
        /*reseta valores*/
        int j;
        for(j = 0; j < nItens; j++){
              vet[j] = 0;
        }
        totalPeso = 0;
        valorParcial = 0;
    }
    
    for(i = 0; i < nItens;i++){
       if(result[i]==1){
          printf("%d\n", itens[i].ID);
       }
    }
    
    //debug
    printf("\n\nValor: %d", valorTotal);
        
    return 0;
}
