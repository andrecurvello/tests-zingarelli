/*--------------------
  SCC5900 - Projeto de algoritmos       07-abril-2011
  Matheus Ricardo Uihara Zingarelli nUSP: 5377855
  Algoritmo utilizando o método Dividir para Conquistar - DC
  
  Descrição:
  Calcule o fecho convexo de um conjunto de pontos. Seu programa lerá os dados
  a partir de um arquivo texto. O nome do arquivo será passado como o único
  argumento de seu programa (use argv, argc). Por simplicidade, assuma que os
  valores das coordenadas são inteiros, positivos, e de no máximo 3 digitos cada.
  Utilize o método de Divisão e Conquista

  Algoritmo
  1 - Ordenar os pontos pelo eixo x, usando MergeSort (Complex. Ômega(n log n))
  2 - Os pontos extremos desta ordenação são vértices do fecho convexo e a reta 
      formada por eles divide os pontos em duas regiões (upperHull e lowerHull)
  3 - Em cada região, descobre qual o ponto mais distante dos extremos, calculando
      a área do triângulo que é formado. Este ponto mais distante é outro vértice
      do fecho convexo.
  4 - Recursivamente, faz 3, considerando um dos extremos originais e o ponto
      mais distante
   
  Complexidade: pior caso - Ômega(n^2) ; caso médio - Ômega(n)
  
  Entrada: arquivo texto no formato:
           número_pontos N
           p1x p1y
           p2x p2y
           .
           .
           .
           pNx pNy
           
  Saída: Coordenadas das extremidades das arestas no formato abaixo. A cada duas linhas, temos uma aresta:
         coord1x coord1y
         coord2x coord2y
         .
         .
         .
         coordMx coordMy
  Observe que pontos adjacentes aparecerão mais de uma vez na saída.
         
--------------------*/

#include <stdio.h>
#include <math.h>

typedef struct par_ordenado{
           int pX;
           int pY;
    } Pontos;

/*
  Faz a junção de dois vetores, mantendo a ordenação.
  Para verificar fim de lista, usamos um sentinela na última posição de cada
  vetor, com o valor de 100000.
  Entradas: vet - vetor de pontos para serem ordenados
            inicio - índice para o início do vetor
            fim - índice para o fim do vetor
            m - índice para o meio do vetor
*/
void merge(Pontos *vet, int inicio, int fim, int m){
     Pontos L1[m+2];
     Pontos L2[m+2];
     int n1 = m - inicio + 1; //tamanho do vetor L1
     int n2 = fim - m; //tamanho do vetor L2
     int sentinela = 100000;
     int i;
     //copiar metade dos dados para L1
     for(i = 0; i < n1; i++){
           L1[i].pX = vet[i+inicio].pX;
           L1[i].pY = vet[i+inicio].pY;
     }
     //sentinela para fim de lista
     L1[n1].pX = sentinela;
     //copiar metade dos dados para L2
     for(i = 0; i < n2; i++){
           L2[i].pX = vet[i+m+1].pX;
           L2[i].pY = vet[i+m+1].pY;
     }
     L2[n2].pX = sentinela;
     
     //juntar L1 e L2 mantendo ordem crescente pelo eixo X
     int j = 0;
     int k = 0;     
     for(i = inicio; i <= fim; i++){
           if(L1[j].pX == sentinela){ //fim de L1, copio restante de L2, se houver
               if(L2[k].pX != sentinela){
                   vet[i].pX = L2[k].pX;
                   vet[i].pY = L2[k].pY;
                   k++;
               }
           }
           else if(L2[k].pX == sentinela){ //fim de L2, copio restante de L1, se houver
               if(L1[j].pX != sentinela){
                   vet[i].pX = L1[j].pX;
                   vet[i].pY = L1[j].pY;
                   j++;
               }
           }
           else if(L1[j].pX < L2[k].pX){
               vet[i].pX = L1[j].pX;
               vet[i].pY = L1[j].pY;
               j++;
           }
           else if(L1[j].pX > L2[k].pX){
               vet[i].pX = L2[k].pX;
               vet[i].pY = L2[k].pY;
               k++;
           }
           else if(L1[j].pX = L2[k].pX){ //quando eixo X é igual, ordena crescente por Y
               if(L1[j].pY > L2[k].pY){
                  vet[i].pX = L2[k].pX;
                  vet[i].pY = L2[k].pY;
                  k++;
               }
               else{
                  vet[i].pX = L1[j].pX;
                  vet[i].pY = L1[j].pY;
                  j++;     
               }
           }
     }
     
     //debug
     /*printf("L1\n");
     for(i = 0; i < n1; i++){
           printf("X: %d | Y: %d\n", L1[i].pX, L1[i].pY);
     }
     printf("L2\n");
     for(i = 0; i < n2; i++){
           printf("X: %d | Y: %d\n", L2[i].pX, L2[i].pY);
     }*/
}

/* 
   Ordenação utilizando o MergeSort, recursivamente dividindo o vetor ao meio
   e ordenando os vetores resultantes da divisão. Para quando o vetor tiver apenas
   um elemento, fazendo a volta através da junção dos vetores divididos.
   Entradas: vet - vetor de pontos para serem ordenados
            início - índice pro início do vetor
            fim - índice pro final do vetor
*/
void mergeSort(Pontos *vet, int inicio, int fim){
     if(inicio < fim){ //para quando houver apenas um elemento
          int m = (inicio + fim) / 2;
          mergeSort(vet, inicio, m);
          mergeSort(vet, m+1, fim);
          merge(vet, inicio, fim , m);
     }
}

/*
  Dado um vetor de pontos e dois pontos (p1 e p2) extremos deste vetor, cria um 
  sub-vetor que contém todos os pontos acima de p1p2. Então, verifica qual ponto
  neste sub-vetor é mais distante de p1p2, denominado pMax. Este ponto é vértice
  do fecho convexo. Recursivamente, calcula novamente para os segmentos p1pMax e
  pMaxp2 para encontrar mais vértices do fecho.
  Entradas: vet - vetor de pontos
            p1,p2 - pontos extremos do vetor de pontos
            n - quantidade de pontos
*/
void verificarUpperHull(Pontos *vet, Pontos p1, Pontos p2, int n){
     if(n < 3){ //vetor contém somente os extremos, formam aresta do fecho
          //salvar aresta
          //debug
          printf("Nao ha mais pontos acima ou estao na reta\n");
     }
     else{     
         /* 
            Para encontrar o upperHull, calculamos a equação geral da reta p1p2 (aX+bY=-c)
            Em cada ponto de vet, verificamos o sinal do resultado de se aplicar o 
            ponto na equação encontrada. Pontos à esquerda fazem parte do upper hull
         */
         int a, b, c, i, j;
         Pontos upperHull[n];//terá no máximo n pontos (se todos os pontos forem alinhados)
         Pontos pMax;
         a = p1.pY - p2.pY;
         b = p2.pX - p1.pX;
         c = -( p1.pX*p2.pY - p1.pY*p2.pX );
         
         j = 0;
         
         //debug
         printf("\nResultados dos pontos\nc=%d\n", c);
         
         for(i = 1; i < n - 1; i++){ //elimino primeiro e último elementos (são p1 e p2)
               //verifico sinal do ponto na equação da reta
               int result = a*vet[i].pX + b*vet[i].pY;
               
               //debug
               printf("Result (%d, %d): %d\n", vet[i].pX, vet[i].pY, result);
               
               if(result > c){ //faz parte do upperHull
                   upperHull[j].pX = vet[i].pX;
                   upperHull[j].pY = vet[i].pY;
                   j++;
               }
         }
         
         //debug
         printf("UpperHull\n");
         for(i = 0; i < j; i++){
               printf("X: %d | Y: %d\n", upperHull[i].pX, upperHull[i].pY);
         }
         printf("\n\nAreas do Upperhull, %d pontos\n", j);
         
         /*
            pMax será o ponto que forma um triângulo com p1 e p2 de maior área. A área
            é metade do determinante formado por estes pontos. Como só estou interessado
            no valor, não irei dividir ao meio, já que o resultado é proporcional,
            economizando uma operação
            Área = p1x*p2y + p3x*p1y + p2x*p3y - p3x*p2y - p2x*p1y -  p1x*p3y
         */
         if(j!=0){ //j é 0 quando todos os pontos estão alinhados
             int maxArea = 0;
             for(i = 0; i < j; i++){
                   int area = p1.pX*p2.pY + upperHull[i].pX*p1.pY + p2.pX*upperHull[i].pY -
                              upperHull[i].pX*p2.pY - p2.pX*p1.pY - p1.pX*upperHull[i].pY;
                              
                   //debug
                   printf("Area de (%d, %d): %d\n",upperHull[i].pX,upperHull[i].pY, area);
                              
                   if(area > maxArea){
                           maxArea = area;
                           pMax.pX = upperHull[i].pX;
                           pMax.pY = upperHull[i].pY;
                   }
             }
             
             //debug
             printf("PMax\nX: %d | Y: %d\n", pMax.pX, pMax.pY);
             
             //recursão
             //debug
             printf("\nUpperhull p1(%d, %d) | pMax(%d, %d)\n", p1.pX, p1.pY, pMax.pX, pMax.pY);
             
             verificarUpperHull(upperHull, p1, pMax, j+1);
             
             //debug
             printf("\nUpperhull pMax(%d, %d) | p2(%d, %d)\n", pMax.pX, pMax.pY, p2.pX, p2.pY);
             
             verificarUpperHull(upperHull, pMax, p2, j+1);
         }
     }
}

int main(int argc, char* argv[]){    
    //lê dados do arquivo
    FILE *fp;
    fp = fopen(argv[1], "r");

//debug
    if(!fp) {printf("\nErro na abertura do arquivo"); return -1;}
       
    int n, i;
    fscanf(fp, "%d", &n);
    Pontos vet_pontos[n];
    for(i = 0; i < n; i++){
       fscanf(fp, "%d %d", &vet_pontos[i].pX, &vet_pontos[i].pY);
    }
    
    //debug
    /*printf("Lista Original\n");
    for(i = 0; i < n; i++){
       printf("X: %d | Y: %d\n", vet_pontos[i].pX, vet_pontos[i].pY);
    }*/
    
    //ordena pontos pelo eixo X
    mergeSort(vet_pontos, 0, n-1);
    
    //debug
    printf("Lista Ordenada por X\n");
    for(i = 0; i < n; i++){
       printf("X: %d | Y: %d\n", vet_pontos[i].pX, vet_pontos[i].pY);
    }
    
    verificarUpperHull(vet_pontos, vet_pontos[0], vet_pontos[n-1], n);
    //verificarLowerHull()
    
    
    fclose(fp);
}































