#ifndef _ESTRUTURAS_H_
#define _ESTRUTURAS_H_

#define Type float

unsigned char *alocaVet(int tam);

//Type **alocaMat(int n,int m);

float **alocaMatf(int n, int m);
int **alocaMati(int n, int m);

/*
//double **alocaMat(int n, int m);

//le matriz da entrada padrao  de tamanho n
void leMatriz(Type** A, int n);

//le vetor de tamanho n sendo n
void leVetor(Type*b, int n);

//imprime uma dada matriz de tamanho n
void imprimeMatriz(Type **A, int n,int m); 

//imprime um dado vetor de tamanho n
void imprimeVetor(Type *x, int n);
*/

void liberaMatriz(int tam,Type**x);

void liberaVector(int tam,Type* x);

#endif
