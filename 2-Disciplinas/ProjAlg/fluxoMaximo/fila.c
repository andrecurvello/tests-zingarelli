#include <stdio.h>
/*
    Inicia uma fila com -1 em todas as posicoes
    Entradas:   fila - vetor de inteiros
                tamanho - tamanho da fila
*/
void iniciaFila(int *fila, int tamanho){
    int i;
    for(i=0;i<tamanho;i++){
        fila[i] = -1;
    }
}

/*
    Imprime conteúdo da fila
    Entrada:    fila - vetor de inteiros
                tamanho - tamanho da fila
*/
void printQueue(int *fila, int tamanho){
    int i;
    for(i=0;i<tamanho;i++){
        printf("%d ", fila[i]);
    }
}

/*
    Verifica se uma fila é vazia
    Entradas:   fila - vetor de inteiros
                tamanho - tamanho da fila
    Saída:      0, caso a fila não esteja vazia
                1, caso a fila esteja vazia
*/
int vazia(int *fila, int tamanho){
    int i;
    for(i=0;i<tamanho;i++){
        if(fila[i]!=-1) return 0;
    }
    return 1;
}

/*
    Verifica se uma fila está cheia
    Entradas:   fila - vetor de inteiros
                tamanho - tamanho da fila
    Saída:      0, caso a fila não esteja cheia 
                1, caso a fila esteja cheia
*/
int cheia(int *fila, int tamanho){
    int i=0;
    while(i<tamanho){
        if(fila[i]==-1) return 0;
        else i++;
    }
    return 1;
}

/*
    Insere elemento na fila
    Entradas:   fila - vetor de inteiros
                tamanho - tamanho da fila
                elemento - numero do vertice a ser inserido
*/
void push(int *fila, int tamanho, int elemento){
    if(!cheia(fila, tamanho)){
        int i=0;
        while(fila[i]!=-1) i++; //procura posicao vaga na fila
        fila[i] = elemento;
    }
    else{
        printf("Erro, fila cheia!\n");
        exit(-1);
    }
}

/*
    Remove elemento da fila
    Entradas:   fila - vetor de inteiros
                tamanho - tamanho da fila
    Saída: elemento removido
*/
int pop(int *fila, int tamanho){
    int elemento = fila[0];
    int i;
    for(i=1;i<tamanho-1;i++){
        fila[i-1] = fila[i];
    }
    fila[tamanho-1] = -1;
    return elemento;
}
