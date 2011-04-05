/*--------------------
  SCC5900 - Projeto de algoritmos       31-março-2011
  Matheus Ricardo Uihara Zingarelli nUSP: 5377855
  Algoritmos de força bruta
  
  Descrição:
  O problema do caixeiro viajante, também por força bruta, que calcula o trajeto
  mais "barato" de uma cidade, passando por todas as demais, e retornando para
  cidade de origem. POR SIMPLICIDADE, vamos empregar SOMENTE grafos COMPLETOS, 
  com n vértices. Assim, não é preciso verificar se o caminho existe ou não.
   
  Complexidade: O((n-1)!)
  
  Entrada: arquivo texto no formato:
           numero_arestas N
           vertice_origem vertice_destino peso
           .
           .
           . (N-2) linhas
           vertice_origem vertice_destino peso           
           
  Saída: saída na linha de comando com o seguinte formato:
         ciclo de vértices, separados por espaço
         peso
         
  Observação 1: Considerando o vértice zero como ponto de partida         
--------------------*/

#include <stdio.h>
#include <math.h>

/*
Calcula o valor da distância de um caminho completo percorrido e verifica se é
menor do que alguma distância já calculada.
Entradas:
         matriz: matriz de adjacências contendo os dados do grafo;
         permutacao: vetor contendo o caminho criado com a permutação dos vértices
         Nvertices: número de vértices do grafo.
Saída: distância percorrida no caminho
*/
int verifica_caminho(int **matriz, int *permutacao, int Nvertices){
     if(permutacao != 0){ //elimina caso de grafos com apenas um vértice
         int i;
         int dist = 0;
         for(i = 0; i < Nvertices-1; i++){               
               //recebo o valor de ir do vértice em permutacao[i]-1 até o vértice em permutacao[i+1]-1
               dist += matriz[permutacao[i]-1][permutacao[i+1]-1];
         }
         //lembrar de adicionar o valor de fechar o ciclo, ou seja, o valor 
         //do último vértice da permutação até o vértice 0
         dist += matriz[permutacao[Nvertices-1]-1][0];         
         
         return dist;
     }
     return 0; 
}


/* 
Função que percorre todos os caminhos possíveis no grafo completo. A cada 
caminho encontrado, dá o valor do peso para o ciclo percorrido. Compara este valor
com valores já encontrados e armazena somente o de menor peso. Caso haja mais
de um caminho de menor peso, guarda o primeiro.
Entradas: 
          matriz: matriz de adjacências contendo os dados do grafo;
          permutacao: vetor responsável por gerar todos os caminhos possíveis
                      através de permutação.
          ciclo_minimo: armazena o ciclo que teve menor custo para ser 
                        percorrido.
          Nvertices: número de vértices do grafo. Utilizado com a variável nivel
                    para verificar se todos os vértices foram visitados.
          vertice: vértice a ser visitado
          distMin: armazena menor distância encontrada até o momento
Complexidade: Considerando n vértices, irá fixar o primeiro e permutar os (n-1)
              vértices seguintes, gerando (n-1)! operações                    
*/
void menor_caminho(int **matriz, int *permutacao, int *ciclo_minimo, int Nvertices, int vertice, int *distMin){
    static int nivel = 0;
    int dist = 0;
    
    nivel++;
    permutacao[vertice] = nivel;
    
    if(nivel == Nvertices){ //completou um caminho, calcula a distância
        dist = verifica_caminho(matriz, permutacao, Nvertices);
        if(dist < *distMin){
                *distMin = dist;
                int i;
                for(i = 0; i < Nvertices; i++){
                       ciclo_minimo[i] = permutacao[i]-1;
                 }
                ciclo_minimo[Nvertices] = dist;
        }
    }
    else{
         int i;
         for(i = 0; i < Nvertices; i++){
               if(permutacao[i] == 0){ //vértice não visitado
                   menor_caminho(matriz, permutacao, ciclo_minimo, Nvertices, i, distMin);
               }
         }
    }
    
    //zera o vértice para ele poder ser visitado novamente nas outras chamadas recursivas;
    nivel--;
    permutacao[vertice] = 0;
}

int main(int argc, char* argv[]){
    int arestas, vertices, origem, destino, peso, i, j;     
      
    /* ler dados do arquivo e criar matriz de adjacências */
    FILE *fp;
    fp = fopen(argv[1], "r");
    fscanf(fp, "%d", &arestas);    
    vertices = (1 + sqrt(1+8*arestas))/2;
    
    //matriz feita com alocação dinâmica para poder passar como parâmetro 
    //sem especificar as dimensôes
    int **matriz = (int**) malloc(sizeof(int)*vertices);
    for(i = 0; i < vertices; i++){
          matriz[i] = (int *) malloc(sizeof(int)*vertices);
    }
    
    //atribuindo zero aos elementos da diagonal principal
    for(i = 0; i < vertices; i++){
        matriz[i][i] = 0;        
    }
    
    for(i = 0; i < arestas; i++){
          fscanf(fp, "%d %d %d", &origem, &destino, &peso);
          matriz[origem][destino] = peso;
          matriz[destino][origem] = peso;
    }
    
    // gerar permutações de todos os caminhos possíveis
    // adaptado do algoritmo de permutação de Alexander Bogomolyn
    // disponível em http://www.bearcave.com/random_hacks/permute.html
    int distMin = 100000; //considerando 100000 como meu infinito
    int nivel = 0; //vai controlar quando um caminho completo foi feito
    int permutacao[vertices]; //vetor para fazer a permutação entre os vértices
    int ciclo_minimo[vertices+1]; //armazena a ordem dos vértices visitados e a soma dos pesos
    for(i = 0; i < vertices; i++){
          permutacao[i] = 0;
          ciclo_minimo[i] = 0;               
    }
    ciclo_minimo[vertices] = 0;
    
    //o vértice zero foi considerado como vértice inicial
    menor_caminho(matriz, permutacao, ciclo_minimo, vertices, 0, &distMin);

    for(i = 0; i < vertices; i++){
         printf("%d ", ciclo_minimo[i]);
    }
    printf("%d\n%d", ciclo_minimo[0], ciclo_minimo[vertices]);
    
    fclose(fp);
    
    return 0;
}
