/*--------------------
  SCC5900 - Projeto de algoritmos       12-junho-2011
  Matheus Ricardo Uihara Zingarelli nUSP: 5377855
  Implementação do algoritmo de fluxo máximo utilizando o método de Ford-Fulkerson
  
  Descrição:
  Dado um grafo dirigido com pesos inteiros positivos em suas arestas, um vértice
  fonte e um sumidouro, ou seja, do tipo "flow network", calcular o fluxo máximo
  que pode passar por este vértice, utilizando o método de Ford-Fulkerson, no
  paradigma de melhoria iterativa (Iterative Improvement). Utilização de fila,
  busca em largura, e vértices forward e backward. Implementação baseada no 
  algoritmo apresentado em Levitin, Capítulo 10 Seção 2.
  
  Entrada:  vertice_fonte (considerando que vai ser 1)
            vertice_sumidouro (seu número me dará o número de vértices do grafo)
            vertice-i vertice-j peso (assume que a direção é do vertice-i para vertice-j)
           
  Saída: Valor do fluxo máximo
  
  Utilização: fluxo-maximo <arquivo_txt>
  
  Observação: assumindo que a capacidade máxima é de 100000 (infinito do problema)
         
--------------------*/
#include <stdio.h>
#include "fila.c"

typedef struct Aresta{
    int u; //capacidade (peso) do vertice i ao vertice j. Negativo quando nao liga vertice
    int x; //total da capacidade utilizado
} aresta;

typedef struct Vertice{
    int visitado;   //1: vertice foi visitado; 0: nao visitado
    int l;          //qtde de fluxo passando por ele
    int pai;        //vertice pelo qual este vertice foi alcancado
    int sinal;      //1: forward edge; 0: backward edge
} vertice;

/*
    Retorna o número de arestas ligadas a um vertice inicial e a lista de todos
    os vertices que ele esta ligado. Retorna arestas do tipo forward ou backward
    de acordo com a entrada passada
    Entradas:
        matriz - grafo
        vert - vertice inicial
        tamanho - tamanho da matriz
        listaVertices - lista de vertices que sao ligados a vert por uma aresta
        tipoBusca - 0: retorna arestas forward. 1: retorna arestas backward
    Saída: número de arestas de um vertice inicial
*/
int getArestas(aresta **matriz, int vert, int tamanho, int *listaVertices, int tipoBusca){
    int i, count;
    //limpa a lista de vertices
    for(i=0;i<tamanho;i++){
        listaVertices[i]=0;
    }
    
    //verifica se há aresta ligando vert com os outros vertices
    switch(tipoBusca){
        case 0: //busca forward edges           
            count = 0;
            for(i=1;i<tamanho+1;i++){
                if(matriz[vert][i].u != -1){//adiciona i a lista de vertices
                    listaVertices[count] = i;
                    count++;
                }
            }
            break;
        case 1: //busca backward edges
            count = 0;
            for(i=1;i<tamanho+1;i++){
                if(matriz[i][vert].u != -1){//adiciona i a lista de vertices
                    listaVertices[count] = i;
                    count++;
                }
            }
            break;
    } 
    return count;   
}

int main(int argc, char* argv[]){
    int fonte, sumidouro; //numero dos vertice fonte e sumidouro
    int i, j, k, arestas;
    int vert1, vert2; //vertices ligados por uma aresta
    
    //leitura dos dados de arquivo
    FILE *fp;    
    fp = fopen(argv[1],"r");
    
    //vertice fonte
    fscanf(fp,"%d",&fonte);    
    //vertice sumidouro
    fscanf(fp,"%d",&sumidouro);
    
    //dados do grafo
    aresta **matriz = (aresta**)malloc(sizeof(aresta)*(sumidouro+1));
    for(i=0;i<=sumidouro;i++){
        matriz[i] = (aresta*)malloc(sizeof(aresta)*(sumidouro+1));
    }
    for(i=1;i<=sumidouro;i++){
        for(j=1;j<=sumidouro;j++){
            matriz[i][j].u = -1;
            matriz[i][j].x = 0;
        }
    }    
    while(!feof(fp)){
        fscanf(fp,"%d %d %d",&i,&j,&k);
        matriz[i][j].u = k;
        matriz[i][j].x = 0;
    }
    
    //debug
    /*for(i=1;i<=sumidouro;i++){
        for(j=1;j<=sumidouro;j++){
            printf("%d/%d\t", matriz[i][j].x, matriz[i][j].u);
        }
        printf("\n");
    }*/
    
    //inicia os dados de cada vértice
    vertice vertices[sumidouro+1];
    
    for(i=0;i<=sumidouro;i++){
        vertices[i].visitado = 0;
        vertices[i].sinal = 0;
        vertices[i].l = 0;
        vertices[i].pai = 0;
    }
        
    //cria fila
    int fila[sumidouro];
    iniciaFila(fila, sumidouro);
    
    //debug
    /*
    if(vazia(fila, sumidouro)) printf("fila vazia\n");
    if(cheia(fila, sumidouro)) printf("fila cheia\n");
    */
    
    //label no vertice fonte
    vertices[fonte].visitado = 1;
    vertices[fonte].l = 100000;
    vertices[fonte].pai = -1; //nao tem pai
    //insere fonte na fila
    push(fila, sumidouro, fonte);
    
    //execução do algoritmo
    while(!vazia(fila, sumidouro)){
        vert1 = pop(fila, sumidouro);      
        
        //lista de vertices ligados a vert
        int *listaVertices = (int*)malloc(sizeof(int)*sumidouro);
        
        //verifica forward edges
        //debug
        //printf("-- Verificando arestas forward --\n");        
        arestas = getArestas(matriz, vert1, sumidouro, listaVertices, 0);
        for(i=0;i<arestas;i++){ //enquanto houver arestas forward
            vert2 = listaVertices[i];
            if(vertices[vert2].visitado==0){ //vertice nao visitado
                //verifica fluxo maximo nessa aresta
                int fluxo = matriz[vert1][vert2].u - matriz[vert1][vert2].x;
                if(fluxo>0){
                    vertices[vert2].l = (vertices[vert1].l<fluxo ? vertices[vert1].l : fluxo);
                    vertices[vert2].visitado = 1;//marca como visitado
                    vertices[vert2].pai = vert1;//marca pai
                    vertices[vert2].sinal = 1;//marca forward
                    //insere na fila
                    push(fila,sumidouro,vert2);
                    //debug
                    //printf("----Insercao forward\n");
                }
                //debug
                //printf("Aresta (%d,%d) recebe fluxo %d\n", vert1, vert2,  vertices[vert2].l);               
            }            
        }
        //debug
        //printf("-- Fim arestas forward --\n");              
        
        //verifica backward edges
        //debug
        //printf("-- Verificando arestas backward --\n");
        arestas = getArestas(matriz, vert1, sumidouro, listaVertices, 1);
        for(i=0;i<arestas;i++){ //enquanto houver arestas backward
            vert2 = listaVertices[i];
            if(vertices[vert2].visitado==0){ //vertice nao visitado
                //debug
                //printf("Verificando (%d,%d) capacidad\n", vert2, vert1, matriz[vert2][vert1].x);
                
                //verifica fluxo maximo nessa aresta
                if(matriz[vert2][vert1].x>0){
                    vertices[vert2].l = (vertices[vert1].l<matriz[vert2][vert1].x ? vertices[vert1].l : matriz[vert2][vert1].x);
                    vertices[vert2].visitado = 1;//marca como visitado
                    vertices[vert2].pai = vert1;//marca pai
                    vertices[vert2].sinal = 0;//marca backward
                    //insere na fila
                    push(fila,sumidouro,vert2);
                    //debug
                    //printf("Aresta (%d,%d) recebe fluxo %d\n", vert1, vert2,  vertices[vert2].l);     
                }          
            }            
        }
        //debug
        //printf("-- Fim arestas backward --\n");       
        
        //verifica se sumidouro foi marcado
        if(vertices[sumidouro].visitado == 1){
            //debug
            /*printf("-- Sumidouro foi marcado, atualizando --\n");
            for(j=1;j<sumidouro+1;j++){
                printf("Vertice %d\n%d/%d/Sinal:%d\n",j,vertices[j].l,vertices[j].pai,vertices[j].sinal);
            }
            printf("-- fim Grafo --\n\n");   */
            
            vert2 = sumidouro;
            while(vert2 != 1){
                int pai = vertices[vert2].pai;
                if(vertices[vert2].sinal == 1){
                    matriz[pai][vert2].x += vertices[sumidouro].l;
                }
                else{
                    matriz[vert2][pai].x -= vertices[sumidouro].l;
                }
                vert2 = pai;
                pai = vertices[pai].pai;
            }//end while
            //reinicia todos os labels dos vertices
            for(i=0;i<=sumidouro;i++){
                vertices[i].visitado = 0;
                vertices[i].sinal = 0;
                vertices[i].l = 0;
                vertices[i].pai = 0;
            }
            vertices[fonte].visitado = 1;
            vertices[fonte].l = 100000;
            vertices[fonte].pai = -1;
            //reinicia a fila
            iniciaFila(fila, sumidouro);
            //insere o vertice fonte
            push(fila, sumidouro, fonte);
            //debug
            //printf("-- Fim atualizacao sumidouro --\n");            
        }
        
        //debug
        /*printf("Vertice %d\n", vert);
        printQueue(fila, sumidouro);
        printf("-- Grafo ate agora --\n");*/
        /*for(j=1;j<sumidouro+1;j++){
            printf("Vertice %d\n%d/%d/Sinal:%d\n",j,vertices[j].l,vertices[j].pai,vertices[j].sinal);
        }
        printf("-- fim Grafo --\n\n");*/
        
        free(listaVertices);
    }//end while fila não vazia
    
    //debug
    /*printf("\nMatriz Final:\n");
    for(i=1;i<=sumidouro;i++){
        for(j=1;j<=sumidouro;j++){
            printf("%d/%d\t", matriz[i][j].x, matriz[i][j].u);
        }
        printf("\n");
    }  */  
            
    //calcula o fluxo máximo no sumidouro
    int fluxoMaximo = 0;
    for(i=1;i<=sumidouro;i++){
        fluxoMaximo += matriz[i][sumidouro].x;
    }
    printf("%d", fluxoMaximo);
    
    //libera ponteiros
    for(i=0;i<sumidouro+1;i++){
        free(matriz[i]);
    }
    free(matriz);
}
