/*--------------------
  SCC5900 - Projeto de algoritmos       30-mar�o-2011
  Matheus Ricardo Uihara Zingarelli nUSP: 5377855
  Algoritmos de for�a bruta
  
  Descri��o:
  Calcule o fecho convexo de um conjunto de pontos.
  Seu programa ler� os dados a partir de um arquivo texto
  (por exemplo, dados.txt). O nome do arquivo ser� passado
  como o �nico argumento de seu programa (use argv, argc).
  Por simplicidade, assuma que os valores das coordenadas 
  s�o inteiros, positivos, e de no m�ximo 3 digitos cada.

  Algoritmo implementado baseado nas solu��es apresentadas por:
  - Anany V. Levitin, Introduction to the Design and Analysis of Algorithms (2nd Edition), Addison Wesley
  - Gilberto C�mara, Ant�nio Miguel Monteiro e Clodoveu Davis, Geoprocessamento: Teoria e Aplica��es, volume IV
   (dispon�vel em http://www.dpi.inpe.br/gilberto/livro/geocomp/geometria.pdf)
   
  Complexidade: O(n^3)
  
  Entrada: arquivo texto no formato:
           n�mero_pontos N
           p1x p1y
           p2x p2y
           .
           .
           .
           pNx pNy
           
  Sa�da: Coordenadas das extremidades das arestas no formato abaixo. A cada duas linhas, temos uma aresta:
         coord1x coord1y
         coord2x coord2y
         .
         .
         .
         coordMx coordMy
  Observe que pontos adjacentes aparecer�o mais de uma vez na sa�da.
         
--------------------*/

#include <stdio.h>

int main(int argc, char* argv[]){
    //estrutura para armazenar os pontos
    struct par_ordenado{
           int pX;
           int pY;
    } p1, p2, pBusca;
       
    int i, j, k, l, a, b, c;
    
    //estas vari�veis me informam de que lado da reta o ponto est�
    int lado1, lado2;
    
    //leitura do arquivo
    FILE *fp;
    fp = fopen(argv[1], "r");

//debug
    if(!fp) {printf("\nErro na abertura do arquivo"); return -1;}
    
    //l� n�mero de pontos
    int n = 0;
    fscanf(fp, "%d", &n);
    
    //l� pontos e armazena em um vetor de pontos
    struct par_ordenado vet_pontos[n];
    for(i = 0; i < n; i++){
       fscanf(fp, "%d %d", &vet_pontos[i].pX, &vet_pontos[i].pY);
    }
    
    fclose(fp);
    
//debug
    /*for(i = 0; i < n; i++){
          printf("%d %d\n", vet_pontos[i].pX, vet_pontos[i].pY);
    }
    printf("%d\n\n", n);*/
    
    //c�lculo do fecho convexo atrav�s da busca das arestas que o formam
    struct par_ordenado vet_resultado[n]; //ir� armazenar as arestas do fecho convexo
    l = 0; //�ndice para armazenar pontos no vetor de resultado
    for(i = 0; i <= n-2; i++){
          p1 = vet_pontos[i];
          for(j = i+1; j <= n-1; j++){
                p2 = vet_pontos[j];
                //c�lculo da equa��o geral da reta (aX + bY = -c)
                a = p1.pY - p2.pY;
                b = p2.pX - p1.pX;
                c = -( p1.pX*p2.pY - p1.pY*p2.pX );
                //zera lados
                lado1 = 0;
                lado2 = 0;
                //em cada ponto, olha o sinal para saber de que lado da reta ele se encontra
                for(k = 0; k <= n-1; k++){
                      if((k != i) && (k != j)){ //evita de procurar no ponto extremo do segmento de reta
                           pBusca = vet_pontos[k];
                           int result = a*pBusca.pX + b*pBusca.pY; //opera��o b�sica do algoritmo                                                    
                           if(result > c) lado1++;
                           else if(result < c) lado2++;
                           //se result = c o ponto est� sobreposto no segmento de reta
                           
                           //printf("Busca X: %d Busca Y: %d\n a: %d, b: %d, c: %d, result: %d lado1: %d, lado2: %d\n", pBusca.pX, pBusca.pY,a, b, c, result, lado1, lado2);
                           //printf("p1X %d p1Y %d\nP2X %d p2Y %d\nbuscaX %d buscaY %d\n\n", p1.pX, p1.pY, p2.pX, p2.pY,pBusca.pX, pBusca.pY);
                      }
                }
                /* Para o segmento ser considerado aresta do fecho convexo, 
                todos os pontos devem estar ou em lado1 ou em lado2. Se estiverem
                em ambos os lados o segmento n�o � aresta do fecho convexo.
                Se ambos os lados forem 0, todos os pontos est�o alinhados e s�o 
                arestas do fecho convexo.                                
                */
                if(lado1 == 0 || lado2== 0){
                   vet_resultado[l++]=p1;
                   vet_resultado[l++]=p2;
                }
          }
    }
    
    for(i = 0; i < l; i++)
          printf("%d %d\n", vet_resultado[i].pX, vet_resultado[i].pY);
    
    return 0;
}
