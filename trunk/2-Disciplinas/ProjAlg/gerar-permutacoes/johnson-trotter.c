/*--------------------
  SCC5900 - Projeto de algoritmos       31-mar�o-2011
  Matheus Ricardo Uihara Zingarelli nUSP: 5377855
  Algoritmos do tipo Decrease & Conquer
  
  Descri��o:
  Dado um n�mero inteiro positivo n, gerar todas as permuta��es envolvendo os
  n�meros de 1 a n. Utilizar o algoritmo de Johnson-Trotter.
  
  Algoritmo: o algoritmo utiliza o conceito de d�gitos m�veis. O d�gito � m�vel
  se ele possui uma flecha acima dele que aponta para um n�mero menor que ele e
  adjacente.
     se n = 1 imprima 1 e termine o programa.
     imprima a permuta��o de 1...n, sendo que todos s�o d�gitos m�veis
     enquanto houver d�gito m�vel
         encontre maior d�gito m�vel k
         troque k com o elemento adjacente a ele para o qual aponta sua seta
         inverta a dire��o da flecha de todos os d�gitos maiores que k
         imprima esta permuta��o
     fim
     
  Entrada: n�mero inteiro positivo na linha de comando. 
           
  Sa�da: todas as permuta��es envolvendo os n�meros de 1 a n. Uma permuta��o por
  linha.
  
  Complexidade: O(n!)
--------------------*/
#include <stdio.h>

/* Estrutura para armazenar um d�gito m�vel, com seu valor e o valor do d�gito
para o qual ele aponta */
typedef struct digito{
        int valor;
        int aponta_para;
} Digito;

/* Imprime os valores contidos na lista de d�gitos (valor de uma permuta��o) 
   Entrada: vet - vetor de d�gitos
            n - tamanho da lista
   Sa�da: valor dos d�gitos na linha de comando */
void imprime(Digito *vet, int n){
    int i;
    for(i = 0; i < n; i++){
        printf("%d ", vet[i].valor);
    }
    printf("\n");
}

/* Dado uma lista de d�gitos, verificar se existem d�gitos m�veis e retornar o
maior deles
   Entrada: vet - vetor de d�gitos
            n - tamanho da lista
   Sa�da: �ndice do maior d�gito m�vel da lista. -1 se n�o houver d�gito m�vel */
int MaiorDigitoMovel(Digito *vet, int n){
    int i;
    int maior = -1;
    for(i = 0; i < n; i++){
        if(vet[i].aponta_para < vet[i].valor){ //� d�gito m�vel
            if(maior < vet[i].valor){
                maior = i;
            }
        }
    }
    return maior;
}

int main(int argc, char* argv[]){
    int n, i, k;
    scanf("%d",&n);
    Digito vet[n];
    
    /* Constru��o da lista de d�gitos */
    vet[0].valor = 1;
    vet[0].aponta_para = 0;
    for(i = 1; i < n; i++){
        vet[i].valor = i+1;
        vet[i].aponta_para = i;
    }
    
    imprime(vet, n);
    
    k = MaiorDigitoMovel(vet, n);
    
    while(k != -1){
        
    }
    
    //debug
    /*for(i = 0; i < n; i++){
        printf("Valor %d aponta para %d\n", vet[i].valor, vet[i].aponta_para);
    }*/
    
    return 0;
}
