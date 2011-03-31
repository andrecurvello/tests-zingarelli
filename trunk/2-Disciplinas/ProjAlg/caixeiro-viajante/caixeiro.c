/*--------------------
  SCC5900 - Projeto de algoritmos       30-março-2011
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

int main(int argc, char* argv[]){
    int arestas, vertices, origem, destino, peso, i, j;     
      
    /* ler dados do arquivo e criar matriz de adjacências */
    FILE *fp;
    fp = fopen(argv[1], "r");
    fscanf(fp, "%d", &arestas);    
    vertices = (1 + sqrt(1+8*arestas))/2;
    
    int matriz[vertices][vertices];
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
    
    return 0;
}
