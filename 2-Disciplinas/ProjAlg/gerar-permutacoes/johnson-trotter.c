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

/* Estrutura para armazenar um d�gito m�vel 
   valor: valor do d�gito
   apontaParaIdx: �ndice para qual elemento da lista ele aponta, -1 caso ele seja
                  um extremo da lista apontando para posi��o fora da lista
   direcao: 0 se ele aponta para elemento � esquerda dele, 1 para a direita
*/
typedef struct digito{
        int valor;
        int apontaParaIdx;
        int direcao;
} Digito;

/* Imprime os valores contidos na lista de d�gitos (valor de uma permuta��o) 
   Entrada: vet - vetor de d�gitos
            n - tamanho da lista
   Sa�da: valor dos d�gitos na linha de comando */
void imprime(Digito *vet, int n){
    int i;
    for(i = 1; i < n+1; i++){
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
    int maiorValor = 0;
    int indice = -1;
    for(i = 1; i < n+1; i++){
        /* verifica se o valor contido no �ndice para o qual o d�gito aponta �
        menor do que o valor desse */
        if(vet[vet[i].apontaParaIdx].valor < vet[i].valor){ //� d�gito m�vel
            if(maiorValor < vet[i].valor){
                maiorValor = vet[i].valor;
                indice = i;
            }
        }
    }
    return indice;
}

int main(int argc, char* argv[]){
    int n, i, k;
    scanf("%d",&n);
    Digito vet[n], aux;
    
    /* Constru��o da lista de d�gitos com dois sentinelas nas extremidades 
     para evitar que os d�gitos nas posi��es 1 e n sejam m�veis quando apontando
     para estas extremidades */
    vet[0].valor = 100000;
    vet[0].apontaParaIdx = -1;
    vet[0].direcao = 0;
    vet[n+1].valor = 100000;
    vet[n+1].apontaParaIdx = -1;
    vet[n+1].direcao = 0;
    for(i = 1; i < n+1; i++){
        vet[i].valor = i;
        vet[i].apontaParaIdx = i-1;
        vet[i].direcao = 0;
    }
    
    imprime(vet, n);
    
    k = MaiorDigitoMovel(vet, n);
    
    while(k != -1){//enquanto houver d�gito m�vel
        //troca d�gito m�vel em k com o d�gito para o qual ele aponta
        aux.valor = vet[k].valor;
        aux.apontaParaIdx = vet[k].apontaParaIdx;
        aux.direcao = vet[k].direcao;
        vet[k].valor = vet[aux.apontaParaIdx].valor;
        vet[k].direcao = vet[aux.apontaParaIdx].direcao;
        vet[k].apontaParaIdx = k-1;
        switch(vet[k].direcao){
            case 0: //aponta para elemento � esquerda
                 vet[k].apontaParaIdx = k-1;
                 break;
            case 1: //aponta para elemento � direita
                 vet[k].apontaParaIdx = k+1;
                 break;                    
        }
        vet[aux.apontaParaIdx].valor = aux.valor;
        vet[aux.apontaParaIdx].direcao = aux.direcao;
        switch(vet[aux.apontaParaIdx].direcao){
            case 0: //aponta para elemento � esquerda
                 vet[aux.apontaParaIdx].apontaParaIdx = aux.apontaParaIdx-1;
                 break;
            case 1: //aponta para elemento � direita
                 vet[aux.apontaParaIdx].apontaParaIdx = aux.apontaParaIdx+1;
                 break;                    
        }
        
        //inverte dire��o do apontaPara dos elementos maiores que d�gito m�vel em k
        for(i = 1; i < n+1; i++){
            if(vet[i].valor > aux.valor){
                //verifica a direcao e inverte
                switch(vet[i].direcao){
                    case 0: //aponta para elemento � esquerda
                         vet[i].direcao = 1;
                         vet[i].apontaParaIdx = i+1;
                         break;
                    case 1: //aponta para elemento � direita
                         vet[i].direcao = 0;
                         vet[i].apontaParaIdx = i-1;
                         break;
                }
            }
        }
        
        imprime(vet, n);
        k = MaiorDigitoMovel(vet, n);
    }
    
    return 0;
}
