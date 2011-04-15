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

/* Estrutura para armazenar um dígito móvel, com seu valor e o valor do dígito
para o qual ele aponta */
typedef struct digito{
        int valor;
        int aponta_para;
} Digito;

/* Imprime os valores contidos na lista de dígitos (valor de uma permutação) 
   Entrada: vet - vetor de dígitos
            n - tamanho da lista
   Saída: valor dos dígitos na linha de comando */
void imprime(Digito *vet, int n){
    int i;
    for(i = 0; i < n; i++){
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
    int maior = -1;
    for(i = 0; i < n; i++){
        if(vet[i].aponta_para < vet[i].valor){ //é dígito móvel
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
    
    /* Construção da lista de dígitos */
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
