/*--------------------
  SCC5900 - Projeto de algoritmos       31-março-2011
  Matheus Ricardo Uihara Zingarelli nUSP: 5377855
  Algoritmos do tipo Decrease & Conquer
  
  Descrição:
  Dado um número inteiro positivo n, gerar todas as permutações envolvendo os
  números de 1 a n. Utilizar o algoritmo de Johnson-Trotter.
  
  Algoritmo: o algoritmo utiliza o conceito de dígitos móveis. O dígito é móvel
  se ele possui uma flecha acima dele que aponta para um número menor que ele e
  adjacente.
     se n = 1 imprima 1 e termine o programa.
     imprima a permutação de 1...n, sendo que todos são dígitos móveis
     enquanto houver dígito móvel
         encontre maior dígito móvel k
         troque k com o elemento adjacente a ele para o qual aponta sua seta
         inverta a direção da flecha de todos os dígitos maiores que k
         imprima esta permutação
     fim
     
  Entrada: número inteiro positivo na linha de comando. 
           
  Saída: todas as permutações envolvendo os números de 1 a n. Uma permutação por
  linha.
  
  Complexidade: O(n!)
--------------------*/
#include <stdio.h>

/* Estrutura para armazenar um dígito móvel 
   valor: valor do dígito
   apontaParaIdx: índice para qual elemento da lista ele aponta, -1 caso ele seja
                  um extremo da lista apontando para posição fora da lista
   direcao: 0 se ele aponta para elemento à esquerda dele, 1 para a direita
*/
typedef struct digito{
        int valor;
        int apontaParaIdx;
        int direcao;
} Digito;

/* Imprime os valores contidos na lista de dígitos (valor de uma permutação) 
   Entrada: vet - vetor de dígitos
            n - tamanho da lista
   Saída: valor dos dígitos na linha de comando */
void imprime(Digito *vet, int n){
    int i;
    for(i = 1; i < n+1; i++){
        printf("%d ", vet[i].valor);
    }
    printf("\n");
}

/* Dado uma lista de dígitos, verificar se existem dígitos móveis e retornar o
maior deles
   Entrada: vet - vetor de dígitos
            n - tamanho da lista
   Saída: índice do maior dígito móvel da lista. -1 se não houver dígito móvel */
int MaiorDigitoMovel(Digito *vet, int n){
    int i;
    int maiorValor = 0;
    int indice = -1;
    for(i = 1; i < n+1; i++){
        /* verifica se o valor contido no índice para o qual o dígito aponta é
        menor do que o valor desse */
        if(vet[vet[i].apontaParaIdx].valor < vet[i].valor){ //é dígito móvel
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
    
    /* Construção da lista de dígitos com dois sentinelas nas extremidades 
     para evitar que os dígitos nas posições 1 e n sejam móveis quando apontando
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
    
    while(k != -1){//enquanto houver dígito móvel
        //troca dígito móvel em k com o dígito para o qual ele aponta
        aux.valor = vet[k].valor;
        aux.apontaParaIdx = vet[k].apontaParaIdx;
        aux.direcao = vet[k].direcao;
        vet[k].valor = vet[aux.apontaParaIdx].valor;
        vet[k].direcao = vet[aux.apontaParaIdx].direcao;
        vet[k].apontaParaIdx = k-1;
        switch(vet[k].direcao){
            case 0: //aponta para elemento à esquerda
                 vet[k].apontaParaIdx = k-1;
                 break;
            case 1: //aponta para elemento à direita
                 vet[k].apontaParaIdx = k+1;
                 break;                    
        }
        vet[aux.apontaParaIdx].valor = aux.valor;
        vet[aux.apontaParaIdx].direcao = aux.direcao;
        switch(vet[aux.apontaParaIdx].direcao){
            case 0: //aponta para elemento à esquerda
                 vet[aux.apontaParaIdx].apontaParaIdx = aux.apontaParaIdx-1;
                 break;
            case 1: //aponta para elemento à direita
                 vet[aux.apontaParaIdx].apontaParaIdx = aux.apontaParaIdx+1;
                 break;                    
        }
        
        //inverte direção do apontaPara dos elementos maiores que dígito móvel em k
        for(i = 1; i < n+1; i++){
            if(vet[i].valor > aux.valor){
                //verifica a direcao e inverte
                switch(vet[i].direcao){
                    case 0: //aponta para elemento à esquerda
                         vet[i].direcao = 1;
                         vet[i].apontaParaIdx = i+1;
                         break;
                    case 1: //aponta para elemento à direita
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
