/*--------------------
  SCC5900 - Projeto de algoritmos       31-mar�o-2011
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

/*
Calcula o valor da dist�ncia de um caminho completo percorrido e verifica se �
menor do que alguma dist�ncia j� calculada.
Entradas:
         matriz: matriz de adjac�ncias contendo os dados do grafo;
         permutacao: vetor contendo o caminho criado com a permuta��o dos v�rtices
         Nvertices: n�mero de v�rtices do grafo.
Sa�da: dist�ncia percorrida no caminho
*/
int verifica_caminho(int **matriz, int *permutacao, int Nvertices){
     if(permutacao != 0){ //elimina caso de grafos com apenas um v�rtice
         int i;
         int dist = 0;
         for(i = 0; i < Nvertices-1; i++){               
               //recebo o valor de ir do v�rtice em permutacao[i]-1 at� o v�rtice em permutacao[i+1]-1
               dist += matriz[permutacao[i]-1][permutacao[i+1]-1];
         }
         //lembrar de adicionar o valor de fechar o ciclo, ou seja, o valor 
         //do �ltimo v�rtice da permuta��o at� o v�rtice 0
         dist += matriz[permutacao[Nvertices-1]-1][0];         
         
         return dist;
     }
     return 0; 
}


/* 
Fun��o que percorre todos os caminhos poss�veis no grafo completo. A cada 
caminho encontrado, d� o valor do peso para o ciclo percorrido. Compara este valor
com valores j� encontrados e armazena somente o de menor peso. Caso haja mais
de um caminho de menor peso, guarda o primeiro.
Entradas: 
          matriz: matriz de adjac�ncias contendo os dados do grafo;
          permutacao: vetor respons�vel por gerar todos os caminhos poss�veis
                      atrav�s de permuta��o.
          ciclo_minimo: armazena o ciclo que teve menor custo para ser 
                        percorrido.
          Nvertices: n�mero de v�rtices do grafo. Utilizado com a vari�vel nivel
                    para verificar se todos os v�rtices foram visitados.
          vertice: v�rtice a ser visitado
          distMin: armazena menor dist�ncia encontrada at� o momento
Complexidade: Considerando n v�rtices, ir� fixar o primeiro e permutar os (n-1)
              v�rtices seguintes, gerando (n-1)! opera��es                    
*/
void menor_caminho(int **matriz, int *permutacao, int *ciclo_minimo, int Nvertices, int vertice, int *distMin){
    static int nivel = 0;
    int dist = 0;
    
    nivel++;
    permutacao[vertice] = nivel;
    
    if(nivel == Nvertices){ //completou um caminho, calcula a dist�ncia
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
               if(permutacao[i] == 0){ //v�rtice n�o visitado
                   menor_caminho(matriz, permutacao, ciclo_minimo, Nvertices, i, distMin);
               }
         }
    }
    
    //zera o v�rtice para ele poder ser visitado novamente nas outras chamadas recursivas;
    nivel--;
    permutacao[vertice] = 0;
}

int main(int argc, char* argv[]){
    int arestas, vertices, origem, destino, peso, i, j;     
      
    /* ler dados do arquivo e criar matriz de adjac�ncias */
    FILE *fp;
    fp = fopen(argv[1], "r");
    fscanf(fp, "%d", &arestas);    
    vertices = (1 + sqrt(1+8*arestas))/2;
    
    //matriz feita com aloca��o din�mica para poder passar como par�metro 
    //sem especificar as dimens�es
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
    
    // gerar permuta��es de todos os caminhos poss�veis
    // adaptado do algoritmo de permuta��o de Alexander Bogomolyn
    // dispon�vel em http://www.bearcave.com/random_hacks/permute.html
    int distMin = 100000; //considerando 100000 como meu infinito
    int nivel = 0; //vai controlar quando um caminho completo foi feito
    int permutacao[vertices]; //vetor para fazer a permuta��o entre os v�rtices
    int ciclo_minimo[vertices+1]; //armazena a ordem dos v�rtices visitados e a soma dos pesos
    for(i = 0; i < vertices; i++){
          permutacao[i] = 0;
          ciclo_minimo[i] = 0;               
    }
    ciclo_minimo[vertices] = 0;
    
    //o v�rtice zero foi considerado como v�rtice inicial
    menor_caminho(matriz, permutacao, ciclo_minimo, vertices, 0, &distMin);

    for(i = 0; i < vertices; i++){
         printf("%d ", ciclo_minimo[i]);
    }
    printf("%d\n%d", ciclo_minimo[0], ciclo_minimo[vertices]);
    
    fclose(fp);
    
    return 0;
}
