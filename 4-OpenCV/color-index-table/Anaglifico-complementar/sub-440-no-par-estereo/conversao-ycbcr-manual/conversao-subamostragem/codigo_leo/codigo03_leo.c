#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include "Estruturas.h"
#include "easyzlib.h"

#define true 1
#define false 0
#define shift 128



typedef struct BmpFile
{
	signed int larg;	//largura
	signed int alt;		//altura
	signed int FileSize;	//Tamanho do arquivo
	signed int reservado;	//Campo reservado
	signed int prof;	//para nao usar os 2 bytes do inicio
	signed int offset;	//offset para dados
	signed int cabeca;	//Analiza tamanho do cabeçalho
	float **R;		//Matrix R
	float **G;		//Matrix G
	float **B;		//Matrix B
	float **Y;
	float **U;
	float **V;
	float **UV;
	signed int ni;		//linha
	signed int nj;		//coluna

}BmpFile;


FILE* image_in = NULL;		//ponteiro para imagem a ser lida
FILE* image_out = NULL;

BmpFile file_in, file_out;	//imagem lida

int tjan = 32;

//void PixelReader(Type *b, Type *g, Type *r);
void PixelReader(float *b, float *g, float *r);

void impr_arq_origem(void)
{
	printf("\narquivo de origem\n\tarquivo imagem de origem, extensão bitmap (.bmp ou .BMP)\n");

}//void impr_tam_origem(void)

void impr_arq_dest(void)
{
	printf("\narquivo de destino\n\tarquivo imagem de destino, extensão bitmap (.bmp ou .BMP)\n");

}//void impr_tam_origem(void)

void impr_amostragem(void)
{
	printf("\namostragem YUV\n\t444 - processa 3 canais\n\t422 - processa 2 canais (Y e U/V) - reducao de U e V horizontal\n\t440 - processa 2 canais (Y e U/V) - reducao de U e V vertical\n");

}//void impr_amostragem(void)

void impr_uso(void)
{
	printf("\nUtilize:\ncomp <arquivo de origem> <arquivo de destino> <amostragem YUV>\n");
	impr_arq_origem();
	impr_arq_dest();
	impr_amostragem();
	printf("\n");

}//void impr_uso(void)


void dtoc(int num, unsigned char *asc_seq)
{
	//printf("num = %d\n", num);
	unsigned char aux1, aux2;

	if(num == 0)
	{
		asc_seq[0] = (int)128;
		asc_seq[1] = '\0';

	}//if(num == 0)
	else
	{
		aux1 = (unsigned int)(abs(num) & (unsigned int)63);
		//printf("aux1 = %d\n", aux1);
		
		if(num < 0)
			aux1 = (unsigned int)(aux1 | (unsigned int)128); //setar bit 8 do aux
		
		//printf("aux1- = %d\n", aux1);
		
		if(abs(num) <= 63)
		{
			asc_seq[0] = aux1;
			asc_seq[1] = '\0';
		}
		else
		{	
			asc_seq[0] = (unsigned char)(aux1 | (unsigned int)64); //setar bit 7 do aux

			if(abs(num) > 16383)
			{
				if(num > 0)
					asc_seq[0] = (unsigned char)127;
				else
					asc_seq[0] = (unsigned char)255;

				asc_seq[1] = (unsigned char)255;
			}
			else
			{
				aux2 = (unsigned int)(abs(num) >> 6);
				//printf("aux2 = %d\n", aux2);
				asc_seq[1] = (unsigned char)(aux2);
			}
			asc_seq[2] = '\0';

		}//else if(i < 0)

	}//else if(i == 0)

}//void dtoc(...)


void dec_num(unsigned char *asc_seq, int *out)
{
    unsigned int aux1, aux2, cl, pl, temp;
    
    cl = (unsigned int)asc_seq[0];
    //printf("cl = %d\n", cl);
    
    if(cl == 128)
        *out = 0;
    else
    { 
        aux1 = cl & 63;
        //printf("aux1 = %d\n", aux1);
        
	aux2 = cl & 64;

        if(aux2 >> 6)
	{
            pl = (unsigned int)asc_seq[1];
            //printf("pl = %d\n", pl);
            temp = pl << 6;
            //printf("temp = %d\n", temp);
            aux1 = aux1 + temp;

	}//if(cl & 64)

        aux2 = cl & 128;

      	if(aux2 >> 7)
            *out = -1 * (int)aux1;
        else
            *out = (int)aux1;
        
    }//else if(cl == -128)

}//void dec_num(char asc_seq[], int num)


//Inicializa estrutura
void BmpFileStart()
{
	file_in.larg=0;		//largura
	file_in.alt=0;		//altura
	file_in.FileSize=0;	//Tamanho do arquivo
	file_in.reservado=0;//Campo reservado
	file_in.prof=0;		//para nao usar os 2 bytes do inicio
	file_in.offset=0;	//offset para dados
	file_in.cabeca=0;	//Analiza tamanho do cabeçalho
	file_in.ni=0;		//Temporario linha
	file_in.nj=0;		//Temporario coluna

	file_out.larg=0;	 //largura
	file_out.alt=0;		 //altura
	file_out.FileSize=0; //Tamanho do arquivo
	file_out.reservado=0;//Campo reservado
	file_out.prof=0;	 //para nao usar os 2 bytes do inicio
	file_out.offset=0;	 //offset para dados
	file_out.cabeca=0;	 //Analiza tamanho do cabeçalho
	file_out.ni=0;		 //Temporario linha
	file_out.nj=0;		 //Temporario coluna

}//void BmpFileStart()


//cria novo arquivo Bmp - By Leo
int BmpNew(char* path)
{
	image_out = fopen(path, "wb+");	//cria arquivo

	if(image_out == NULL)
		return false;

	fputc('B', image_out);
	fputc('M', image_out);

	return true;

}//int BmpNew(char* path)


//abre arquivo Bmp
int BmpOpen(char* path)
{
	image_in = fopen(path,"rb+");	//Abre o arquivo

	if(image_in == NULL)
		return false;

	return true;

}//int BmpOpen(char* path)


//fecha arquivo Bmp
int BmpClose()
{
	fclose(image_in);	//fecha arquivo de entrada

	fclose(image_out);	//fecha arquivo de saida

	return true;
	
}//int BmpClose()


//testa arquivo Se arquivo ja aberto e bmp
int BmpTester()
{
	char* buffer=(char*) malloc(sizeof(char)*2);	//aloca string para verificacao

	fread(buffer, sizeof(char), 2, image_in);	//le string do arquivo

	if(!strcmp(buffer, "BM"))
		return true;				//Retorna true se é um arquivo BMP
	else 
		return false;				//Se não for BMP

}//int BmpTester()


//atualiza altura e largura do bmp
void BmpSize()
{	
	int temp_ni;		//temporario linha
	int temp_nj;		//Temporario coluna

	rewind(image_in);						//coloca ponteiro no inicio do arquivo

	fseek(image_in, 18 * sizeof(char), 1);				//anda para a posicao de leitura

	fread(&(file_in.alt), 4, 1, image_in);				//largura
	fread(&(file_in.larg), 4, 1, image_in);				//altura

	file_out.larg = file_in.larg;
	file_out.alt  = file_in.alt;

	temp_ni = file_in.alt  % tjan;
	temp_nj = file_in.larg % tjan;

	if (temp_ni == 0)
		file_in.ni = file_in.alt / tjan;
	else
		file_in.ni = (file_in.alt - temp_ni) / tjan + 1;

	if (temp_nj == 0)
		file_in.nj = file_in.larg / tjan;
	else
		file_in.nj = (file_in.larg - temp_nj) / tjan + 1;

	file_out.ni = file_in.ni;
	file_out.nj = file_in.nj;

	file_in.R = alocaMatf(file_in.nj * tjan, file_in.ni * tjan);		//Aloga Matriz R
	file_in.G = alocaMatf(file_in.nj * tjan, file_in.ni * tjan);		//Aloga Matriz G
	file_in.B = alocaMatf(file_in.nj * tjan, file_in.ni * tjan);		//Aloga Matriz B

	file_in.Y = alocaMatf(file_in.nj * tjan, file_in.ni * tjan);		//Aloga Matriz Y
	file_in.U = alocaMatf(file_in.nj * tjan, file_in.ni * tjan);		//Aloga Matriz U
	file_in.V = alocaMatf(file_in.nj * tjan, file_in.ni * tjan);		//Aloga Matriz V
	file_in.UV = alocaMatf(file_in.nj * tjan, file_in.ni * tjan);		//Aloga Matriz UV


	file_out.R = alocaMatf(file_out.nj * tjan, file_out.ni * tjan);		//Aloga Matriz R
	file_out.G = alocaMatf(file_out.nj * tjan, file_out.ni * tjan);		//Aloga Matriz G
	file_out.B = alocaMatf(file_out.nj * tjan, file_out.ni * tjan);		//Aloga Matriz B

	file_out.Y = alocaMatf(file_out.nj * tjan, file_out.ni * tjan);		//Aloga Matriz Y
	file_out.U = alocaMatf(file_out.nj * tjan, file_out.ni * tjan);		//Aloga Matriz U
	file_out.V = alocaMatf(file_out.nj * tjan, file_out.ni * tjan);		//Aloga Matriz V
	file_out.UV = alocaMatf(file_in.nj * tjan, file_in.ni * tjan);		//Aloga Matriz UV

	
}//void BmpSize()


//escreve cabecalho do arquivo de saida
void BmpHeadOut()
{
	signed int cbuf[2];
	cbuf[1] = 0;
	cbuf[0] = file_out.FileSize;

	printf("tam = %d\n", cbuf[0]);

	fwrite(cbuf, 4, 1, image_out);		//2
	cbuf[0] = file_out.reservado;
	fwrite(cbuf, 4, 1, image_out);		//6
	cbuf[0] = file_out.offset; 
	fwrite(cbuf, 4, 1, image_out);		//10
	cbuf[0] = 40;				//tamanho do head
	fwrite(cbuf, 4, 1, image_out);		//14
	cbuf[0] = file_out.alt;
	fwrite(cbuf, 4, 1, image_out);		//18
	cbuf[0] = file_out.larg;
	fwrite(cbuf, 4, 1, image_out);		//22
	cbuf[0] = 1;				//planos
	fwrite(cbuf, 2, 1, image_out);		//26
	cbuf[0] = 24;				//bits por pixel
	fwrite(cbuf, 2, 1, image_out);		//28
	cbuf[0] = 0;				//compressao
	fwrite(cbuf, 4, 1, image_out);		//32
	cbuf[0] = file_out.FileSize - file_out.offset;
	fwrite(cbuf, 4, 1, image_out);		//36
	cbuf[0] = 0;				//resol hor
	fwrite(cbuf, 4, 1, image_out);		//40
	cbuf[0] = 0;				//resol ver
	fwrite(cbuf, 4, 1, image_out);		//44
	cbuf[0] = 0;				//num max cores
	fwrite(cbuf, 4, 1, image_out);		//48
	cbuf[0] = 0;				//num cores importantes
	fwrite(cbuf, 4, 1, image_out);		//52



}//escreve cabecalho do arquivo de saida


//atualiza tamanho do arquivo
void BmpFileSize()
{
	rewind(image_in);				//joga ponteiro no inicio do arquivo
	fseek(image_in, 2 * sizeof(char), 1);		//caminha no arquivo para ver tamanho
	fread(&(file_in.FileSize), 4, 1, image_in);	//seta tamanho

	file_out.FileSize = file_in.FileSize;

}//void BmpFileSize()


/*Descobre profudindade do pixel*/
void BmpBitPixel()
{
	rewind(image_in);				//Joga ponteiro no inicio do arquivo
	fseek(image_in, 28 * sizeof(char), 1);		//profundidade do pixel
	fread(&(file_in.prof), 2, 1, image_in);		//seta largura

	file_out.prof = file_in.prof;

}//void BmpBitPixel()


/*Atualiza campo reservado*/
void BmpReservado()
{
	rewind(image_in);				//joga ponteiro no inicio do arquivo
	fseek(image_in, 6 * sizeof(char), 1);		//caminha no arquivo para ver campo reservado
	fread(&(file_in.reservado), 4, 1, image_in);	//seta reservado

	file_out.reservado = file_in.reservado;

}//void BmpReservado()


//coloca em Matriz R,G,B a partir do arquivo
void BmpMatrix()
{
	int i, j;

	rewind(image_in);						//joga ponteiro no inicio do arquivo
	fseek(image_in, 10 * sizeof(char), 1);				//caminha no arquivo para ver tamanho
	fread(&(file_in.offset), 4, 1, image_in);			//seta tamanho

	file_out.offset = file_in.offset;	
	
	rewind(image_in);						//coloca ponteiro no inicio
	fseek(image_in, file_in.offset * sizeof(char), 1);		//anda para os dados do arquivo

	for (i = 0; i < file_in.larg; i++)
	      	for (j = 0; j < file_in.alt; j++) 
			PixelReader(&file_in.B[i][j], &file_in.G[i][j], &file_in.R[i][j]);

}//void BmpMatrix()


//funcionando para 24 bits e para 32 porem ignora a matriz de transparencia
void PixelReader(float *b, float *g, float *r)
{
	unsigned int bt, gt, rt;
	unsigned char cbuf[4];

	fread(&cbuf, sizeof(char), file_in.prof / 8, image_in);		//Divide pelo tamanho de 1 byte= 8 bits
	//printf("%d %d %d\n", cbuf[2], cbuf[1], cbuf[0]);

	//coloca os valores lidos
	bt = (unsigned int)(cbuf[0]);
	gt = (unsigned int)(cbuf[1]);
	rt = (unsigned int)(cbuf[2]);
	*b = (float)bt;
	*g = (float)gt;
	*r = (float)rt;
	//printf("%f %f %f\n", *r, *g, *b);


}//void PixelReader(Type *b,Type *g,Type *r)


//funcionando para 24 bits e para 32 porem ignora a matriz de transparencia
void PixelWrite(float *b, float *g, float *r)
{
	unsigned int lbuf[4];
	unsigned char cbuf[4];

	lbuf[0] = (unsigned int)(*b);
	lbuf[1] = (unsigned int)(*g);
	lbuf[2] = (unsigned int)(*r);
	
	cbuf[0] = (unsigned char)(lbuf[0]);
	cbuf[1] = (unsigned char)(lbuf[1]);
	cbuf[2] = (unsigned char)(lbuf[2]);

	fwrite(cbuf, sizeof(char), file_out.prof / 8, image_out);	//Divide pelo tamanho de 1 byte= 8 bits
	//printf("%d %d %d\n", cbuf[2], cbuf[1], cbuf[0]);

}//void PixelWrite(Type *b,Type *g,Type *r)


//carrega Se arquivo é BMP
void BmpSave()
{
	int i, j;

	//BmpPreftester();

	for (i = 0; i < file_in.larg; i++) 
        	for (j = 0; j < file_in.alt; j++) 
	      		PixelWrite(&file_out.B[i][j], &file_out.G[i][j], &file_out.R[i][j]);	

}//void BmpSave()



float arredondamento(float x){

	float y = x - ceil(x);
	float z = x - floor(x);
	if( y == 0)return x;
	else if(fabs(y) > fabs(z)) return floor(x);
		else return ceil(x);

}

//compressao
void compressao(char** argv)
{
  int i, j, a, l, k;
  long int kY, kU, kV, tamC, tamCY, tamCU, tamCV;
  unsigned char *cY, *cU, *cV, *cIn, *cOut;

  FILE* arq_temp = NULL;
  FILE* arq_comp = NULL;

  float **tempY, **tempU, **tempV, **tempY2, **tempU2, **tempV2,
	****YT,
	****UT,
	****VT;

  int 	****MYint,
	****MUint,
	****MVint;
  
  tempY = alocaMatf(tjan, tjan);
  tempU = alocaMatf(tjan, tjan);
  tempV = alocaMatf(tjan, tjan);

  tempY2 = alocaMatf(tjan, tjan);
  tempU2 = alocaMatf(tjan, tjan);
  tempV2 = alocaMatf(tjan, tjan);

  cY = alocaVeti(4);
  cU = alocaVeti(4);
  cV = alocaVeti(4);

  BmpFileStart();				//zera variaveis
	
  if(BmpOpen(argv[1]))
    if(BmpNew(argv[2]))
	if(BmpTester())
	{
		BmpFileSize();			//Seta Tamanho do arquivo
		printf("Leu o tamanho...\n");

		BmpSize();			//Seta tamanho
		printf("Ajustou tamanho...\n");

		BmpBitPixel();			//Seta profundidade do pixel
		printf("Ajusta profundidade...\n");

		BmpMatrix();			//inicializa RGB e offset de dados
		printf("Inicializa matrizes...\n");

		BmpReservado();			//Seta campo reservado;

		YT = (float ****)malloc((file_in.nj + 1) * sizeof(float ****));
		UT = (float ****)malloc((file_in.nj + 1) * sizeof(float ****));
		VT = (float ****)malloc((file_in.nj + 1) * sizeof(float ****));

		MYint = (int ****)malloc((file_in.nj + 1) * sizeof(int ****));
		MUint = (int ****)malloc((file_in.nj + 1) * sizeof(int ****));
		MVint = (int ****)malloc((file_in.nj + 1) * sizeof(int ****));

		for(i = 0; i <= file_in.nj; i++)
		{
			YT[i] = (float ***)malloc((file_in.ni + 1) * sizeof(float ***));
			UT[i] = (float ***)malloc((file_in.ni + 1) * sizeof(float ***));
			VT[i] = (float ***)malloc((file_in.ni + 1) * sizeof(float ***));

			MYint[i] = (int ***)malloc((file_in.ni + 1) * sizeof(int ***));
			MUint[i] = (int ***)malloc((file_in.ni + 1) * sizeof(int ***));
			MVint[i] = (int ***)malloc((file_in.ni + 1) * sizeof(int ***));

		}//for(i = 0; i < file_in.ni + 1; i++)

		for(j = 0; j < file_in.nj; j++)
			for(i = 0; i < file_in.ni; i++)
			{
				YT[j][i] = alocaMatf(tjan, tjan);
				UT[j][i] = alocaMatf(tjan, tjan);
				VT[j][i] = alocaMatf(tjan, tjan);
				MYint[j][i] = alocaMati(tjan, tjan);
				MUint[j][i] = alocaMati(tjan, tjan);
				MVint[j][i] = alocaMati(tjan, tjan);
			}


		cIn  = alocaVeti(file_in.nj * file_in.ni * tjan * tjan * 2);
		cOut = alocaVeti(file_in.nj * file_in.ni * tjan * tjan * 2);

		//conversao RGB para YUV		
		for(j = 0; j < file_in.nj * tjan; j++)
			for(i = 0; i < file_in.ni * tjan; i++)
			{
    				file_in.Y[j][i] = 0.299f * (file_in.R[j][i] - shift)  + 0.587f * (file_in.G[j][i] - shift) + 0.114f * (file_in.B[j][i] - shift);
	    			file_in.U[j][i] = 0.565f * ((file_in.B[j][i] - shift) - file_in.Y[j][i]);
				file_in.V[j][i] = 0.713f * ((file_in.R[j][i] - shift) - file_in.Y[j][i]);

			}//for(j = 0; j < file_in.larg; j++)

		printf("Realizou conversao YUV...\n");

		if(!strcmp("444", argv[3]))
		{		
			for(j = 0; j < file_in.nj; j++)
				for(i = 0; i < file_in.ni; i++)
					for(l = 0; l < tjan; l++)
						for(a = 0; a < tjan; a++)
						{
							//file_out.Y[l + j * tjan][a + i * tjan] = file_in.Y[l + j * tjan][a + i * tjan];
							MYint[j][i][l][a] = (int)arredondamento(file_in.Y[l + j * tjan][a + i * tjan]);
							file_out.Y[l + j * tjan][a + i * tjan] = (float)MYint[j][i][l][a];
							//file_out.U[l + j * tjan][a + i * tjan] = file_in.U[l + j * tjan][a + i * tjan];
							MUint[j][i][l][a] = (int)arredondamento(file_in.U[l + j * tjan][a + i * tjan]);
							file_out.U[l + j * tjan][a + i * tjan] = (float)MUint[j][i][l][a];
							//file_out.V[l + j * tjan][a + i * tjan] = file_in.V[l + j * tjan][a + i * tjan];
							MVint[j][i][l][a] = (int)arredondamento(file_in.V[l + j * tjan][a + i * tjan]);
							file_out.V[l + j * tjan][a + i * tjan] = (float)MVint[j][i][l][a];
						}

		}//if(!strcmp("444", argv[3]))
		else
		if(!strcmp("440", argv[3]) || !strcmp("422", argv[3]))
		{

			if(!strcmp("440", argv[3]))
			{		
				for(j = 0; j < file_in.nj * tjan; j++)
				{
					k = 0;
					for(i = 0; i < file_in.ni * tjan / 2; i++)
					{

						file_in.UV[j][i] = (file_in.U[j][k] + file_in.U[j][k + 1]) / 2;
						file_in.UV[j][i + file_in.ni * tjan / 2] = (file_in.V[j][k] + file_in.V[j][k + 1]) / 2;
						k += 2;

					}//for(j = 0; j < file_in.larg; j++)

				}//for(j = 0; j < file_in.larg; j++)

			}//if(!strcmp("440", argv[3]))

			else //422
			{
				for(i = 0; i < file_in.ni * tjan; i++)
				{
					k = 0;
					for(j = 0; j < file_in.nj * tjan / 2; j++)
					{
						file_in.UV[j][i] = (file_in.U[k][i] + file_in.U[k + 1][i]) / 2;
						file_in.UV[j + file_in.nj * tjan / 2][i] = (file_in.V[k][i] + file_in.V[k + 1][i]) / 2;
						k += 2;

					}//for(j = 0; j < file_in.larg; j++)

				}//for(j = 0; j < file_in.larg; j++)

			}//else if(!strcmp("440", argv[3]))

			for(j = 0; j < file_in.nj; j++)
				for(i = 0; i < file_in.ni; i++)
					for(l = 0; l < tjan; l++)
						for(a = 0; a < tjan; a++)
						{
								MYint[j][i][l][a] = (int)arredondamento(file_in.Y[l + j * tjan][a + i * tjan]);
								file_out.Y[l + j * tjan][a + i * tjan] = (float)MYint[j][i][l][a];
								MUint[j][i][l][a] = (int)arredondamento(file_in.UV[l + j * tjan][a + i * tjan]);
								file_out.UV[l + j * tjan][a + i * tjan] = (float)MUint[j][i][l][a];
	
						}//if((l + i * tjan) < file_in.larg && (a + j * tjan)...

			if(!strcmp(argv[3], "440"))
			{
			  for(j = 0; j < file_out.nj * tjan; j++)
			  {
				k = 0;
				for (i = 0; i < file_out.ni * tjan / 2; i++)
				{
					file_out.U[j][k] = file_out.U[j][k + 1] = file_out.UV[j][i];
					file_out.V[j][k] = file_out.V[j][k + 1] = file_out.UV[j][i + file_out.ni * tjan / 2];
					k += 2;

				}//for (i = 0; i < file_in.alt; i++)

			  }//for(j = 0; j < file_in.larg; j++)

			}// if(strcmp(argv[3], "444")

			else //422
			{
			
			  for(i = 0; i < file_out.ni * tjan; i++)
			  {
				k = 0;
				for (j = 0; j < file_out.nj * tjan / 2; j++)
				{
				   file_out.U[k][i] = file_out.U[k + 1][i] = file_out.UV[j][i];
				   file_out.V[k][i] = file_out.V[k + 1][i] = file_out.UV[j + file_out.nj * tjan / 2][i];
				   k += 2;

				}//for (i = 0; i < file_in.alt; i++)

			  }//for(j = 0; j < file_in.larg; j++)

			}//else if(strcmp(argv[3], "444")
			
		}//else if(strcomp("444", argv[3])

		printf("Retornando para RGB...\n");

		for(j = 0; j < file_in.larg; j++)
	      		for(i = 0; i < file_in.alt; i++)
			{
    				file_out.R[j][i] = (file_out.Y[j][i] + 1.403f * file_out.V[j][i]) + shift;
	    			file_out.G[j][i] = (file_out.Y[j][i] - 0.344f * file_out.U[j][i] - 0.714f * file_out.V[j][i]) + shift;
				file_out.B[j][i] = (file_out.Y[j][i] + 1.77f * file_out.U[j][i]) + shift;

				if(file_out.R[j][i] < 0)
					file_out.R[j][i] = 0;

				if(file_out.B[j][i] < 0)
					file_out.B[j][i] = 0;

				if(file_out.G[j][i] < 0)
					file_out.G[j][i] = 0;

				if(file_out.R[j][i] > 255)
					file_out.R[j][i] = 255;
				
				if(file_out.B[j][i] > 255)
					file_out.B[j][i] = 255;

				if(file_out.G[j][i] > 255)
					file_out.G[j][i] = 255;

				file_out.R[j][i] = arredondamento(file_out.R[j][i]);
				file_out.G[j][i] = arredondamento(file_out.G[j][i]);
				file_out.B[j][i] = arredondamento(file_out.B[j][i]);

			
			}//for(j = 0; j < file_in.alt; j++)

		BmpHeadOut();
		BmpSave();
		BmpClose();

		free(tempY);
		free(tempU);
		free(tempV);
		free(tempY2);
		free(tempU2);
		free(tempV2);

		free(file_in.R);
		free(file_in.G);
		free(file_in.B);

		free(file_in.Y);
		free(file_in.U);
		free(file_in.V);
		free(file_in.UV);

		free(file_out.R);
		free(file_out.G);
		free(file_out.B);

		free(file_out.Y);
		free(file_out.U);
		free(file_out.V);
		free(file_out.UV);

		kY = 0;
		kU = 0;
		kV = 0;
		
		tamCY = 0;
		tamCU = 0;
		tamCV = 0;

		arq_temp = fopen("saida.tmp", "w+");
		arq_comp = fopen("saida.dat", "w+");

		printf("Abriu arquivo...\n");

		for(j = 0; j < file_out.nj; j++)
      			for(i = 0; i < file_out.ni; i++)
				for(l = 0; l < tjan; l++)
					for(a = 0; a < tjan; a++)
					{
						dtoc(MYint[j][i][l][a], cY);
						
						cIn[kY++] = cY[0];

						if(abs(MYint[j][i][l][a]) > 63)
							cIn[kY++] = cY[1];

					}//for(a = 0; a < tjan; a++)

		cIn[kY] = '\0';
		ezcompress(cOut, &tamC, cIn, (long)strlen(cIn));
		fwrite(cIn, sizeof(char), kY,  arq_temp);
		fwrite(cOut, sizeof(char), tamC,  arq_comp);
		tamCY = tamC;
		printf("num Y = %ld,  comp Y = %ld.\n", kY, tamC);

		for(j = 0; j < file_out.nj; j++)
      			for(i = 0; i < file_out.ni; i++)
				for(l = 0; l < tjan; l++)
					for(a = 0; a < tjan; a++)
					{
						dtoc(MUint[j][i][l][a], cU);
						
						cIn[kU++] = cU[0];

						if(abs(MUint[j][i][l][a]) > 63)
							cIn[kU++] = cU[1];

					}//for(a = 0; a < tjan; a++)
		
		cIn[kU] = '\0';
		ezcompress(cOut, &tamC, cIn, strlen(cIn));
		fwrite(cIn, sizeof(char), kU,  arq_temp);
		fwrite(cOut, sizeof(char), tamC,  arq_comp);
		tamCU = tamC;
		printf("num U = %ld,  comp U = %ld.\n", kU, tamC);

		if(!strcmp("444", argv[3]))
		{
			for(j = 0; j < file_out.nj; j++)
	      			for(i = 0; i < file_out.ni; i++)
					for(l = 0; l < tjan; l++)
						for(a = 0; a < tjan; a++)
						{
							dtoc(MVint[j][i][l][a], cV);
						
							cIn[kV++] = cV[0];

							if(abs(MVint[j][i][l][a]) > 63)
								cIn[kV++] = cV[1];

					}//for(a = 0; a < tjan; a++)

			cIn[kV] = '\0';
			ezcompress(cOut, &tamC, cIn, strlen(cIn));
			tamCV = tamC;
			fwrite(cIn, sizeof(char), kV,  arq_temp);
			fwrite(cOut, sizeof(char), tamC,  arq_comp);
			printf("num V = %ld,  comp V = %ld.\n", kV, tamC);


		}//if(strcmp("440", argv[3]))

		printf("Tamanho total = %ld, Tamanho comprimido = %ld, Relacao de Compressao = %f.\n", (kY + kU + kV), (tamCY + tamCU + tamCV), (float)(tamCY + tamCU + tamCV)/file_in.FileSize);
		
		fclose(arq_temp);
		fclose(arq_comp);

	      }//if(BmpTester())
	      else
	      {
		printf("\nArquivo de origem inválido.\n");
		impr_arq_origem();

	      }//else if(BmpTester())

	    else
	    {
	      printf("\nErro ao abrir arquivo de destino.\n");
	      impr_arq_dest();

	    }//else if(BmpNew(argv[2]))

	  else
	  {
	    printf("\nErro ao abrir arquivo de origem.\n");
	    impr_arq_origem();

	  }//else if(BmpOpen(argv[1]))

}//void compressao(char** argv)


//funcao principal
int main(int argc, char** argv)
{
  int ecYUV;

  char str_t[4];

  if(argc != 4)
    impr_uso();
  else
    if((strstr(argv[1], ".bmp") != NULL) || (strstr(argv[1], ".BMP") != NULL))
	if((strstr(argv[2], ".bmp") != NULL) || (strstr(argv[2], ".BMP") != NULL))
	{

	    ecYUV = atoi(argv[3]);

	    if(ecYUV == 444 || ecYUV == 422 || ecYUV == 440)

		   compressao(&argv[0]);

	    else
	      impr_amostragem();

	}//if((strstr(argv[2], ".bmp") != NULL) || (strstr(argv[2], ".BMP") != NULL))
	else
	  impr_arq_dest();

    else
      impr_arq_origem();

}//void main(int argc, char** argv)

