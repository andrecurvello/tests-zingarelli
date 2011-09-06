#include <stdio.h>
#include <stdlib.h>

#define Type unsigned int

/*Aloca vetor dinamicamente*/
unsigned char *alocaVet(int tam) {
	unsigned char *tmp;
	tmp = (char *)malloc((tam+1) * sizeof(char));
        return tmp;
}

//void inicializa(Type **A, int n, int m);
//void inicializa(double **A, int n, int m);
void inicializaf(float **A, int n, int m);
void inicializai(int **A, int n, int m);

/*
//Aloca matriz dinamicamente
Type **alocaMat(int n, int m)
{
	int i;
	Type **tmp;
	tmp = (Type **)malloc((n+1)* sizeof(Type *));
	for (i=0;i<n+1;i++) tmp[i] = (Type *)malloc(m * sizeof(Type));
	inicializa(tmp,n,m);
	return tmp;
}

double **alocaMat(int n, int m)
{
	int i;
	double **tmp;
	tmp = (double **)malloc((n+1)* sizeof(double *));
	for (i=0;i<n+1;i++) tmp[i] = (double *)malloc(m * sizeof(double));
	inicializa(tmp, n, m);
	return tmp;
}
*/

float **alocaMatf(int n, int m)
{
	int i;
	float **tmp;

	tmp = (float **)malloc((n + 1)* sizeof(float *));

	for (i = 0; i < n + 1; i++)
		tmp[i] = (float *)malloc((m + 1) * sizeof(float));

	inicializaf(tmp, n, m);

	return tmp;
}

int **alocaMati(int n, int m)
{
	int i, **tmp;

	tmp = (int **)malloc((n + 1)* sizeof(int *));
	//tmp = (int **)calloc((n + 1), sizeof(int *));

	for (i = 0; i < n + 1; i++)
		tmp[i] = (int *)malloc((m + 1) * sizeof(int));

	inicializai(tmp, n, m);

	return tmp;
}


/*
//imprime uma dada matriz de tamanho n
void inicializa(Type **A, int n,int m) {
    int i,j;
    for (i=0; i<n; i++) {
        for (j=0; j<m; j++) { A[i][j]=0; }//imprime matriz na notacao pedida
    }
}


//imprime uma dada matriz de tamanho n
void inicializa(double **A, int n, int m)
{
    int i,j;
    for (i=0; i<n; i++) 
    {
	for (j=0; j<m; j++) 
	{
		A[i][j]=0; 

	}//imprime matriz na notacao pedida
    }
}
*/

void inicializaf(float **A, int n, int m)
{
    int i, j;

    for (i = 0; i < n; i++) 
    {
	for (j = 0; j < m; j++) 
	{
		A[i][j] = 0; 

	}
   }

}

void inicializai(int **A, int n, int m)
{
    int i, j;

    for (i = 0; i < n; i++) 
    {
	for (j = 0; j < m; j++) 
	{
		A[i][j] = 0; 

	}
   }

}


/*
//le vetor de tamanho n sendo n
void leVetor(Type *b, int n) {
    int i;
    for (i=0; i<n; i++) {
        scanf("%X",&b[i]);//le vetor de double
    }
}
//imprime uma dada matriz de tamanho n
void imprimeMatriz(Type **A, int n,int m) {
    int i,j;
    for (i=0; i<n; i++) {
        for (j=0; j<m; j++) { printf("%X ",A[i][j]); }//imprime matriz na notacao pedida
	printf("\n");
    }
}

//imprime um dado vetor de tamanho n
void imprimeVetor(Type *x, int n) {
    int i;
    for (i=0; i<n-1; i++) { printf("%X ",x[i]); }//imprme um dado vetor com dada precisao 5 casas e em notacao exponencial
    printf("%X\n",x[i]);
}
*/

void liberaMatriz(int tam,Type **x){
  int i;
  for (i=0;i<tam+1;i++) free(x[i]);
  free(x);
}

void liberaVector(int tam,Type *x){
 free(x);
}
