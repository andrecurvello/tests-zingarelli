/*--------------------
  SCC5900 - Projeto de algoritmos       30-mar�o-2011
  Matheus Ricardo Uihara Zingarelli nUSP: 5377855
  Algoritmos de for�a bruta
  
  Descri��o:
  O problema do caixeiro viajante, tamb�m por for�a bruta, que calcula o trajeto
  mais "barato" de uma cidade, passando por todas as demais, e retornando para
  cidade de origem. POR SIMPLICIDADE, vamos empregar SOMENTE grafos COMPLETOS, 
  com n v�rtices. Assim, n�o � preciso verificar se o caminho existe ou n�o.
   
  Complexidade: O((n-1)!)
  
  Entrada: arquivo texto no formato:
           numero_arestas N
           vertice_origem vertice_destino peso
           .
           .
           . (N-2) linhas
           vertice_origem vertice_destino peso           
           
  Sa�da: sa�da na linha de comando com o seguinte formato:
         ciclo de v�rtices, separados por espa�o
         peso
         
  Observa��o 1: Considerando o v�rtice zero como ponto de partida         
--------------------*/

#include <stdio.h>
#include <math.h>

int main(int argc, char* argv[]){
    int arestas, vertices, origem, destino, peso, i, j;     
      
    /* ler dados do arquivo e criar matriz de adjac�ncias */
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
    
    // gerar permuta��es de todos os caminhos poss�veis
    // adaptado do algoritmo de permuta��o de Alexander Bogomolyn
    // dispon�vel em http://www.bearcave.com/random_hacks/permute.html
    
    return 0;
}
