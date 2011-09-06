#include <stdio.h>
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
#define fat 1.0f
#define TypeT Type
#define V1 8.0f
#define V2 12.0f
#define V3 16.0f
#define V4 32.0f
#define V5 64.0f


typedef struct BmpFile
{
	signed int larg;		//largura
	signed int alt;			//altura
	signed int FileSize;		//Tamanho do arquivo
	signed int reservado;		//Campo reservado
	signed int prof;		//para nao usar os 2 bytes do inicio
	signed int offset;		//offset para dados
	signed int cabeca;		//Analiza tamanho do cabeçalho
	float **R;			//Matrix R
	float **G;			//Matrix G
	float **B;			//Matrix B
	float **Y;
	float **U;
	float **V;
	float **UV;
	signed int ni;			//linha
	signed int nj;			//coluna

}BmpFile;

int tjan;

float **MQ, fator1q = -1, fator2q = -1, fatorf = -1,
      sqrt_3, denom, h0, h1, h2, h3, g0, g1, g2, g3, Ih0, Ih1, Ih2, Ih3, Ig0, Ig1, Ig2, Ig3;

FILE* image_in = NULL;		//ponteiro para imagem a ser lida
FILE* image_out = NULL;

BmpFile file_in, file_out;	//Figura lida

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

void impr_tam_bloco(void)
{
	printf("\ntamanho do macrobloco\n\tprecisa ser uma potencia de 2, variando de 8 a 256.\n\tvalores possíveis: 8, 16, 32, 64, 128 e 256.\n");

}//void impr_tam_origem(void)

void impr_amostragem(void)
{
	printf("\namostragem YUV\n\t444 - processa 3 canais\n\t422 - processa 2 canais (Y e U/V) - reducao de U e V horizontal\n\t440 - processa 2 canais (Y e U/V) - reducao de U e V vertical\n");

}//void impr_tam_origem(void)

void impr_perfis(void)
{
	printf("\nperfis de compressao\nnq - não utiliza a matriz de quantização - maior qualidade e menor compressão\n\tUtilize: nq=x\n\tonde x varia de 0 à 100.\n");
	printf("\nq - utiliza a matriz de quantização - menor qualidade e maior compressão\n\tUtilize: q=y\n\tonde y varia de 0 à 100.\n");
	printf("\n\t0   - qualidade mínima (maior compressao)\n\t100 - qualidade maxima (menor compressao)\n");

}//void impr_tam_origem(void)

void impr_uso(void)
{
	printf("\nUtilize:\ndb2_comp <arquivo de origem> <arquivo de destino> <tamanho do macrobloco> <amostragem YUV> <perfil de compressao>\n");
	impr_arq_origem();
	impr_arq_dest();
	impr_tam_bloco();
	impr_amostragem();
	impr_perfis();
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

void MatrizQ()
{
	int	i, j, x, y, py,
		jan = tjan / 4,
		cont = 1;

	float	passo = 1,
		val1 = V1,
		val2 = V2,
		val3 = V3,
		val4 = V4,
		val5 = V5;

	for (i = 1; i <= jan + 1; i++)
	{
    		x = i - 1;
    		y = 0;
    
    		if (cont == 2)
        		val1 = val1 - 1;
        
    		if (cont == 3)
        		val1 = val1 + 0.5;
    
    		if (cont == 4)
        		val1 = val1 - 0.75;//- 0.25
    
    		while (x >= 0)
		{
        		MQ[x][y] = val1;
        		x--;
        		y++;

		}//while (x > 0)
        
	    	if (cont == 4)
		{
       			cont = 1;
		        val1 = V1 - 0.25 * passo; //8 + 0.25 * passo
		        passo = passo + 1;
		}
		else
		        cont = cont + 1;

	}//for (i = 1; i < jan; i++)

	py = 1;

	for (i = jan + 1; i <= jan * 2; i++)
	{
    		x = jan;
    		y = py;
    		py = py + 1;
    
    		if (cont == 2)
        		val1 = val1 - 1;
        
		if (cont == 3)
        		val1 = val1 + 0.5;
    
		if (cont == 4)
			val1 = val1 - 0.75;// - 0.25
    
	    	while (y <= jan)
		{
        		MQ[x][y] = val1;
		        x--;
		        y++;

		}//while (y <= jan)
        
		if (cont == 4)
		{
        		cont = 1;
        		val1 = V1 - 0.25 * passo;//8 + 0.25 * passo
        		passo = passo + 1;
		}
		else
			cont = cont + 1;
    
	}//for (i = jan + 1; i < jan * 2; i++)

	//montando bandas baixas e baixas frequencias
	for (i = 0; i <= jan; i++)
    		for (j = jan; j < 2 * jan; j++)
		{
        		MQ[i][j] = val2;
        		MQ[j][i] = val2;

		}//for (j = jan: j < 2 * jan: j++)

	for (i = jan; i <= 2 * jan; i++)
    		for (j = jan; j < 2 * jan; j++)
        		MQ[i][j] = val3;

	//montando altas frequencias e mix
	for (i = 0; i <= 2 * jan; i++)
    		for (j = 2 * jan; j < 4 * jan; j++)
		{
        		MQ[i][j] = val4;
        		MQ[j][i] = val4;

    		}//for (j = 2 * jan; i < 4 * jan; j++)

	for (i = 2 * jan; i <= 4 * jan; i++) 
		for (j = 2 * jan; j < 4 * jan; j++)
        		MQ[i][j] = val5;

}//void MatrizQ(float **m)


//funcao que retorna em um float o tempo em milisegundos
float dbtime()
{
	float time;

	time = (float)clock() / (float)CLOCKS_PER_SEC * 1000;
	return time;

}//float dbtime()


//Inicializa estrutura
void BmpFileStart()
{
	file_in.larg=0;		//largura
	file_in.alt=0;		//altura
	file_in.FileSize=0;	//Tamanho do arquivo
	file_in.reservado=0;	//Campo reservado
	file_in.prof=0;		//para nao usar os 2 bytes do inicio
	file_in.offset=0;	//offset para dados
	file_in.cabeca=0;	//Analiza tamanho do cabeçalho
	file_in.ni=0;		//Temporario linha
	file_in.nj=0;		//Temporario coluna

	file_out.larg=0;	//largura
	file_out.alt=0;		//altura
	file_out.FileSize=0;	//Tamanho do arquivo
	file_out.reservado=0;	//Campo reservado
	file_out.prof=0;	//para nao usar os 2 bytes do inicio
	file_out.offset=0;	//offset para dados
	file_out.cabeca=0;	//Analiza tamanho do cabeçalho
	file_out.ni=0;		//Temporario linha
	file_out.nj=0;		//Temporario coluna

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
	file_out.UV = alocaMatf(file_out.nj * tjan, file_out.ni * tjan);		//Aloga Matriz UV

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

	lbuf[0] = (unsigned int)(*b);//(abs(*b));
	lbuf[1] = (unsigned int)(*g);//(abs(*g));
	lbuf[2] = (unsigned int)(*r);//(abs(*r));
	
	cbuf[0] = (unsigned char)(lbuf[0]);
	cbuf[1] = (unsigned char)(lbuf[1]);
	cbuf[2] = (unsigned char)(lbuf[2]);

	fwrite(cbuf, sizeof(char), file_out.prof / 8, image_out);	//Divide pelo tamanho de 1 byte= 8 bits
	//printf("%d %d %d\n", cbuf[2], cbuf[1], cbuf[0]);

}//void PixelWrite(Type *b,Type *g,Type *r)

/*
void BmpPreftester()
{
	signed int cbuf[2];

	file_in.alt  = cbuf[1] = tjan * file_in.nj;
	file_in.larg = cbuf[0] = tjan * file_in.ni;

	rewind(image_in);				//coloca ponteiro no inicio do arquivo
	fseek(image_in, 18 * sizeof(char), 1);		//anda para a posicao de leitura

	//fwrite(cbuf, 4, 2, image_in);

}//void BmpPreftester()
*/

//carrega Se arquivo é BMP
void BmpSave()
{
	int i, j;

	//BmpPreftester();

	for (i = 0; i < file_in.larg; i++) 
        	for (j = 0; j < file_in.alt; j++) 
	      		PixelWrite(&file_out.B[i][j], &file_out.G[i][j], &file_out.R[i][j]);	

}//void BmpSave()


void transform( float* a, const int n )
{
      if (n >= 4) {
         int i, j;
         const int half = n >> 1;
         
	 float tmp[n];

         for (i = 0, j = 0; j < n-3; j += 2, i++) {
            tmp[i]      = a[j]*h0 + a[j+1]*h1 + a[j+2]*h2 + a[j+3]*h3;
            tmp[i+half] = a[j]*g0 + a[j+1]*g1 + a[j+2]*g2 + a[j+3]*g3;
         }

         tmp[i]      = a[n-2]*h0 + a[n-1]*h1 + a[0]*h2 + a[1]*h3;
         tmp[i+half] = a[n-2]*g0 + a[n-1]*g1 + a[0]*g2 + a[1]*g3;

         for (i = 0; i < n; i++) {
            a[i] = tmp[i];
         }
	 //delete [] tmp;
      }
}

void invTransform( float* a, const int n )
{
     if (n >= 4) {
       int i, j;
       const int half = n >> 1;
       const int halfPls1 = half + 1;

       float tmp[n];

       //      last smooth val  last coef.  first smooth  first coef
       tmp[0] = a[half-1]*Ih0 + a[n-1]*Ih1 + a[0]*Ih2 + a[half]*Ih3;
       tmp[1] = a[half-1]*Ig0 + a[n-1]*Ig1 + a[0]*Ig2 + a[half]*Ig3;
       for (i = 0, j = 2; i < half-1; i++) {
	 //     smooth val     coef. val       smooth val    coef. val
	 tmp[j++] = a[i]*Ih0 + a[i+half]*Ih1 + a[i+1]*Ih2 + a[i+halfPls1]*Ih3;
	 tmp[j++] = a[i]*Ig0 + a[i+half]*Ig1 + a[i+1]*Ig2 + a[i+halfPls1]*Ig3;
       }
       for (i = 0; i < n; i++) {
	 a[i] = tmp[i];
       }
       //delete [] tmp;
     }
}


void daubTrans( float* ts, int N )
{
      int n;
      for (n = N; n >= 4; n >>= 1) {
         transform( ts, n );
      }
}


void invDaubTrans( float* coef, int N )
{
      int n;
      for (n = 4; n <= N; n <<= 1) {
         invTransform( coef, n );
      }
}


void haar_Transform2D(float **rows, int width, int height, int levels, int inverse)
{
	float *plane,*line,*nextline;
	int step,step2,x,y,ll_width;
	int a,b,c,d,A,B,C,D;

	ll_width = width>>levels;
	plane = rows[0];

	if ( ! inverse ) {

		for(step=1;step<=ll_width;step<<=1) {
			step2 = step + step;
			for(y=0;(y+step)<height;y += step2) {
				line = plane + y*width;
				nextline = line + step*width;
				for(x=0;(x+step)<width;x += step2) {

					a = line[x]; b = line[x+step];
					c = nextline[x]; d = nextline[x+step];

					A = (a+b+c+d)>>2;
					B = a-b-c+d;
					C = a+b-c-d;
					D = a-b+c-d;

					line[x] 			= A; // will be coded in next pass
					line[x+step]		= B;
					nextline[x]			= C;
					nextline[x+step]	= D;
				}
			}
		}

	} else {

		/** go backwards in scale **/
		for(step=ll_width;step>=1;step>>=1) {
			step2 = step + step;

			for(y=0;(y+step)<height;y += step2) {

				line = plane + y*width;
				nextline = line + step*width;

				for(x=0;(x+step)<width;x += step2) {

					A = line[x]; // already decoded.
					B = line[x+step];
					C = nextline[x];
					D = nextline[x+step];

					a = A + ((3 + B + C + D)>>2);
					b = A + ((3 - B + C - D)>>2);
					c = A + ((3 - B - C + D)>>2);
					d = A + ((3 + B - C - D)>>2);

					line[x]			= a;
					line[x+step]	= b;
					nextline[x] 	= c;
					nextline[x+step]= d;
				}
			}
		}
	}
}



//funcao principal
void db2_compressao(char** argv)
{

  float time_ini, time_end;
  int i, j, a, l, k;
  long int kY, kU, kV, tamC, tamCY, tamCU, tamCV;
  unsigned char *cY, *cU, *cV, *cIn, *cOut;

  FILE* arq_temp = NULL;
  FILE* arq_comp = NULL;

  float **tempY, **tempU, **tempV, **tempUV,
	****YT,
	****UT,
	****VT;

  int 	****MYint,
	****MUint,
	****MVint;

/*
	**YT[file_in.ni][file_in.nj],
	**UT[file_in.ni][file_in.nj],
	**VT[file_in.ni][file_in.nj];

  int 	**MYint[file_in.ni][file_in.nj],
	**MUint[file_in.ni][file_in.nj],	//35234735
	**MVint[file_in.ni][file_in.nj];
*/  
  tempY = alocaMatf(tjan, tjan);
  tempU = alocaMatf(tjan, tjan);
  tempV = alocaMatf(tjan, tjan);
  tempUV = alocaMatf(tjan, tjan);

  cY = alocaVet(4);
  cU = alocaVet(4);
  cV = alocaVet(4);

  sqrt_3 = sqrt( 3 );
  denom = 4 * sqrt( 2 );
  h0 = (1 + sqrt_3)/denom;
  h1 = (3 + sqrt_3)/denom;
  h2 = (3 - sqrt_3)/denom;
  h3 = (1 - sqrt_3)/denom;
  g0 =  h3;
  g1 = -h2;
  g2 =  h1;
  g3 = -h0;
  Ih0 = h2;
  Ih1 = g2;  // h1
  Ih2 = h0;
  Ih3 = g0;  // h3
  Ig0 = h3;
  Ig1 = g3;  // -h0
  Ig2 = h1;
  Ig3 = g1;  // -h2

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
		printf("Leu o arquivo...\nni = %d, nj = %d\n", file_in.ni, file_in.nj);

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

		//tmp = (float **)malloc((n + 1)* sizeof(float *));

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

		MatrizQ();

		cIn  = alocaVet(file_in.nj * file_in.ni * tjan * tjan * 2);
		cOut = alocaVet(file_in.nj * file_in.ni * tjan * tjan * 2);

		printf("Iniciou variaveis, cIn e cOut...\n");

		printf("Inicia transformacao...\n");

		time_ini = dbtime();

		//conversao RGB para YUV		
		for(j = 0; j < file_in.nj * tjan; j++)
	      		for(i = 0; i < file_in.ni * tjan; i++)
			{
    				file_in.Y[j][i] = 0.299f * (file_in.R[j][i] - shift) / 255  + 0.587f * (file_in.G[j][i] - shift) / 255 + 0.114f * (file_in.B[j][i] - shift) / 255;
	    			file_in.U[j][i] = 0.565f * ((file_in.B[j][i] - shift) / 255 - file_in.Y[j][i]);
   				file_in.V[j][i] = 0.713f * ((file_in.R[j][i] - shift) / 255 - file_in.Y[j][i]);

			}//for(j = 0; j < file_in.larg; j++)

		printf("Realizou conversao YUV...\n");

		if(!strcmp("444", argv[4]))
		{		
			for(j = 0; j < file_in.nj; j++)
				for(i = 0; i < file_in.ni; i++)
					for(l = 0; l < tjan; l++)
						for(a = 0; a < tjan; a++)
						{
							YT[j][i][l][a] = file_in.Y[l + j * tjan][a + i * tjan];
							UT[j][i][l][a] = file_in.U[l + j * tjan][a + i * tjan];
							VT[j][i][l][a] = file_in.V[l + j * tjan][a + i * tjan];
						}

			//aplicacao da transformada
			for(j = 0; j < file_in.nj; j++)
				for(i = 0; i < file_in.ni; i++)
				{
					for(l = 0; l < tjan; l++)
					{
						for(a = 0; a < tjan; a++)
						{
							tempY[l][a] = YT[j][i][l][a];
							tempU[l][a] = UT[j][i][l][a];
							tempV[l][a] = VT[j][i][l][a];
							//printf("(%f %f %f)\t", tempY[l][a], tempU[l][a], tempV[l][a]);

						}//for(a = 0; a < tjan; a++)

						//printf("\n");
					}
	
					daubTrans(*tempY, tjan);
					daubTrans(*tempU, tjan);
					daubTrans(*tempV, tjan);

					//haar_Transform2D(tempY, tjan, tjan, 1, 0);
					//haar_Transform2D(tempU, tjan, tjan, 1, 0);
					//haar_Transform2D(tempV, tjan, tjan, 1, 0);

					for(l = 0; l < tjan; l++)
					{
						for(a = 0; a < tjan; a++)
						{
							YT[j][i][l][a] = tempY[l][a];
							UT[j][i][l][a] = tempU[l][a];
							VT[j][i][l][a] = tempV[l][a];
							//printf("(%f %f %f)\t", tempY[l][a], tempU[l][a], tempV[l][a]);

						}//for(a = 0; a < tjan; a++)

						//printf("\n");

					}

				}//for(j = 0; a < file_in.nj; a++)

			if (fator1q != -1)
			{
				//quantizacao
				for(j = 0; j < file_out.nj; j++)
		      			for(i = 0; i < file_out.ni; i++)
						for(l = 0; l < tjan; l++)
							for(a = 0; a < tjan; a++)
							{
								MYint[j][i][l][a]  = (int)(YT[j][i][l][a] * fator1q / MQ[l][a]);
								MUint[j][i][l][a]  = (int)(UT[j][i][l][a] * fator2q / MQ[l][a]);
								MVint[j][i][l][a]  = (int)(VT[j][i][l][a] * fator2q / MQ[l][a]);
					
							}//for(a = 0; a < tjan; a++)

				//passa pra float
				for(j = 0; j < file_out.nj; j++)
					for(i = 0; i < file_out.ni; i++)
						for(l = 0; l < tjan; l++)
							for(a = 0; a < tjan; a++)
							{
								YT[j][i][l][a]  = (float)MYint[j][i][l][a] * MQ[l][a] / fator1q;
								UT[j][i][l][a]  = (float)MUint[j][i][l][a] * MQ[l][a] / fator2q;
								VT[j][i][l][a]  = (float)MVint[j][i][l][a] * MQ[l][a] / fator2q;

							}//for(a = 0; a < tjan; a++)
			}//if (!strcmp("q", argv[4]))
			else
			{
				//nao quantizados
				for(j = 0; j < file_out.nj; j++)
		      			for(i = 0; i < file_out.ni; i++)
						for(l = 0; l < tjan; l++)
							for(a = 0; a < tjan; a++)
							{
								MYint[j][i][l][a]  = (int)(YT[j][i][l][a] * fatorf);
								MUint[j][i][l][a]  = (int)(UT[j][i][l][a] * fatorf);
								MVint[j][i][l][a]  = (int)(VT[j][i][l][a] * fatorf);
			
							}//for(a = 0; a < tjan; a++)

				//passa pra float
				for(j = 0; j < file_out.nj; j++)
					for(i = 0; i < file_out.ni; i++)
						for(l = 0; l < tjan; l++)
							for(a = 0; a < tjan; a++)
							{
								YT[j][i][l][a]  = (float)MYint[j][i][l][a] / fatorf;	
								UT[j][i][l][a]  = (float)MUint[j][i][l][a] / fatorf;
								VT[j][i][l][a]  = (float)MVint[j][i][l][a] / fatorf;

							}//for(a = 0; a < tjan; a++)

			}//else if (!strcmp("q", argv[4]))

			printf("Realizou transformada...\n");
				
			//aplicacao da transformada inversa
			for(j = 0; j < file_in.nj; j++)
				for(i = 0; i < file_in.ni; i++)
				{
					for(l = 0; l < tjan; l++)
					{
						for(a = 0; a < tjan; a++)
						{
							tempY[l][a] = YT[j][i][l][a];
							tempU[l][a] = UT[j][i][l][a];
							tempV[l][a] = VT[j][i][l][a];
							//printf("(%f %f %f)\t", tempY[l][a], tempU[l][a], tempV[l][a]);

						}//for(a = 0; a < tjan; a++)

						//printf("\n");

					}

					invDaubTrans(*tempY, tjan);
					invDaubTrans(*tempU, tjan);
					invDaubTrans(*tempV, tjan);

					//haar_Transform2D(tempY, tjan, tjan, 1, 1);
					//haar_Transform2D(tempU, tjan, tjan, 1, 1);
					//haar_Transform2D(tempV, tjan, tjan, 1, 1);

					for(l = 0; l < tjan; l++)
						for(a = 0; a < tjan; a++)
						{
							YT[j][i][l][a] = tempY[l][a];
							UT[j][i][l][a] = tempU[l][a];
							VT[j][i][l][a] = tempV[l][a];
							//printf("%f %f %f\n", temp2R[l][a], temp2G[l][a], temp2B[l][a]);

						}//for(a = 0; a < tjan; a++)

				}//for(j = 0; a < file_in.nj; a++)

			for(j = 0; j < file_out.nj; j++)
		      		for(i = 0; i < file_out.ni; i++)
					for(l = 0; l < tjan; l++)
						for(a = 0; a < tjan; a++)
						{ 
							file_out.Y[l + j * tjan][a + i * tjan] = YT[j][i][l][a];
							file_out.U[l + j * tjan][a + i * tjan] = UT[j][i][l][a];
							file_out.V[l + j * tjan][a + i * tjan] = VT[j][i][l][a];

						}

		}//if(strcmp("444", argv[3])

		else
		if(!strcmp("440", argv[4]) || !strcmp("422", argv[4]))
		{

			if(!strcmp("440", argv[4]))
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

			}//if(!strcmp("440", argv[4]))

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

			}//else if(!strcmp("440", argv[4]))

			for(j = 0; j < file_in.nj; j++)
				for(i = 0; i < file_in.ni; i++)
					for(l = 0; l < tjan; l++)
						for(a = 0; a < tjan; a++)
						{
								YT[j][i][l][a] = file_in.Y[l + j * tjan][a + i * tjan];
								UT[j][i][l][a] = file_in.UV[l + j * tjan][a + i * tjan];
	
						}//if((l + i * tjan) < file_in.larg && (a + j * tjan)...

			//aplicacao da transformada
			for(j = 0; j < file_in.nj; j++)
				for(i = 0; i < file_in.ni; i++)
				{
					for(l = 0; l < tjan; l++)
					{
						for(a = 0; a < tjan; a++)
						{
							tempY[l][a] = YT[j][i][l][a];
							tempU[l][a] = UT[j][i][l][a];
							//printf("(%f %f %f)\t", tempY[l][a], tempU[l][a], tempV[l][a]);

						}//for(a = 0; a < tjan; a++)

						//printf("\n");
					}

					daubTrans(*tempY, tjan);
					daubTrans(*tempU, tjan);

					//haar_Transform2D(tempY, tjan, tjan, 1, 0);
					//haar_Transform2D(tempUV, tjan, tjan, 1, 0);

					for(l = 0; l < tjan; l++)
					{
						for(a = 0; a < tjan; a++)
						{
							YT[j][i][l][a] = tempY[l][a];
							UT[j][i][l][a] = tempU[l][a];
							//printf("(%f %f %f)\t", tempY[l][a], tempU[l][a], tempV[l][a]);

						}//for(a = 0; a < tjan; a++)

						//printf("\n");

					}

				}//for(j = 0; a < file_in.nj; a++)
			
			if (fator1q != -1)
			{
				//passa pra int
				for(j = 0; j < file_out.nj; j++)
		      			for(i = 0; i < file_out.ni; i++)
						for(l = 0; l < tjan; l++)
							for(a = 0; a < tjan; a++)
							{
								MYint[j][i][l][a]  = (int)(YT[j][i][l][a] * fator1q / MQ[l][a]);
								MUint[j][i][l][a]  = (int)(UT[j][i][l][a] * fator2q / MQ[l][a]);

							}//for(a = 0; a < tjan; a++)
		
				//passa pra float
				for(j = 0; j < file_out.nj; j++)
			      		for(i = 0; i < file_out.ni; i++)
						for(l = 0; l < tjan; l++)
							for(a = 0; a < tjan; a++)
							{
								YT[j][i][l][a] = (float)MYint[j][i][l][a] * MQ[l][a] / fator1q;
								UT[j][i][l][a] = (float)MUint[j][i][l][a] * MQ[l][a] / fator2q;

							}//for(a = 0; a < tjan; a++)

			}//if (!strcmp("q", argv[4]))
			else
			{
				//passa pra int
				for(j = 0; j < file_out.nj; j++)
		      			for(i = 0; i < file_out.ni; i++)
						for(l = 0; l < tjan; l++)
							for(a = 0; a < tjan; a++)
							{
								MYint[j][i][l][a]  = (int)(YT[j][i][l][a] * fatorf);
								MUint[j][i][l][a]  = (int)(UT[j][i][l][a] * fatorf);

							}//for(a = 0; a < tjan; a++)
		
				//passa pra float
				for(j = 0; j < file_out.nj; j++)
			      		for(i = 0; i < file_out.ni; i++)
						for(l = 0; l < tjan; l++)
							for(a = 0; a < tjan; a++)
							{
								YT[j][i][l][a]  = (float)MYint[j][i][l][a] / fatorf;
								UT[j][i][l][a]  = (float)MUint[j][i][l][a] / fatorf;

							}//for(a = 0; a < tjan; a++)

			}//else if (!strcmp("q", argv[4]))

			//aplicacao da transformada inversa
			for(j = 0; j < file_in.nj; j++)
				for(i = 0; i < file_in.ni; i++)
				{
					for(l = 0; l < tjan; l++)
					{
						for(a = 0; a < tjan; a++)
						{
							tempY[l][a] = YT[j][i][l][a];
							tempU[l][a] = UT[j][i][l][a];
							//printf("(%f %f %f)\t", tempY[l][a], tempU[l][a], tempV[l][a]);

						}//for(a = 0; a < tjan; a++)

						//printf("\n");
					}

					invDaubTrans(*tempY, tjan);
					invDaubTrans(*tempUV, tjan);

					//haar_Transform2D(tempY, tjan, tjan, 1, 1);
					//haar_Transform2D(tempUV, tjan, tjan, 1, 1);

					for(l = 0; l < tjan; l++)
						for(a = 0; a < tjan; a++)
						{
							YT[j][i][l][a] = tempY[l][a];
							UT[j][i][l][a] = tempU[l][a];

						}//for(a = 0; a < tjan; a++)

				}//for(j = 0; a < file_in.nj; a++)

			for(j = 0; j < file_out.nj; j++)
		      		for(i = 0; i < file_out.ni; i++)
					for(l = 0; l < tjan; l++)
						for(a = 0; a < tjan; a++)
						{ 
							file_out.Y[l + j * tjan][a + i * tjan] = YT[j][i][l][a];
							file_out.UV[l + j * tjan][a + i * tjan] = UT[j][i][l][a];

						}//if ((l + i  * tjan) < file_out.larg && (a + j * tjan) < file_out.alt)

			if(!strcmp(argv[4], "440"))
			{
			  for(j = 0; j < file_out.nj * tjan; j++)
			  {
				k = 0;
				for (i = 0; i < file_out.ni * tjan / 2; i++)
				{
					if(j % tjan != 0)
					{
						file_out.U[j][k] = file_out.U[j][k + 1] = file_out.UV[j][i];
						file_out.V[j][k] = file_out.V[j][k + 1] = file_out.UV[j][i + file_out.ni * tjan / 2];

					}//if(i % tjan != 0)

					else
					{
						  file_out.U[j][k] = file_out.U[j][k + 1] = file_out.UV[j + 1][i];
						  file_out.V[j][k] = file_out.V[j][k + 1] = file_out.UV[j + 1][i + file_out.ni * tjan / 2 ];

					}//else if(i % tjan != 0)

					k += 2;

				}//for (i = 0; i < file_in.alt; i++)

			  }//for(j = 0; j < file_in.larg; j++)

			}// if(strcmp(argv[4], "444")

			else //422
			{
			
			  for(i = 0; i < file_out.ni * tjan; i++)
			  {
				k = 0;
				for (j = 0; j < file_out.nj * tjan / 2; j++)
				{
					if(j % tjan != 0)
					{
					  file_out.U[k][i] = file_out.U[k + 1][i] = file_out.UV[j][i];
					  file_out.V[k][i] = file_out.V[k + 1][i] = file_out.UV[j + file_out.nj * tjan / 2][i];
					}//if(i % tjan != 0)
					else
					{
					    file_out.U[k][i] = file_out.U[k + 1][i] = file_out.UV[j + 1][i];
					    file_out.V[k][i] = file_out.V[k + 1][i] = file_out.UV[j + 1 + file_out.nj * tjan / 2][i];

					}//else if(i % tjan != 0)

					k += 2;

				}//for (i = 0; i < file_in.alt; i++)

			  }//for(j = 0; j < file_in.larg; j++)

			}//else if(strcmp(argv[4], "444")
			
		}//else if(strcomp("444", argv[3])

		printf("Retornando para RGB...\n");

		for(j = 0; j < file_in.larg; j++)
	      		for(i = 0; i < file_in.alt; i++)
			{
    				file_out.R[j][i] = 255 * (file_out.Y[j][i] + 1.403f * file_out.V[j][i]) + shift;
	    			file_out.G[j][i] = 255 * (file_out.Y[j][i] - 0.344f * file_out.U[j][i] - 0.714f * file_out.V[j][i]) + shift;
				file_out.B[j][i] = 255 * (file_out.Y[j][i] + 1.77f * file_out.U[j][i]) + shift;

				if(file_out.R[j][i] < 0)
					file_out.R[j][i] = 0;//abs(file_out.R[j][i]);

				if(file_out.B[j][i] < 0)
					file_out.B[j][i] = 0;//abs(file_out.B[j][i]);

				if(file_out.G[j][i] < 0)
					file_out.G[j][i] = 0;//abs(file_out.G[j][i]);

				if(file_out.R[j][i] > 255)
					file_out.R[j][i] = 255;
				
				if(file_out.B[j][i] > 255)
					file_out.B[j][i] = 255;

				if(file_out.G[j][i] > 255)
					file_out.G[j][i] = 255;

			}//for(j = 0; j < file_in.alt; j++)

		time_end = dbtime();

		//for(i = tjan; i <= tjan; i++)
		//	for(j = 0; j <= tjan; j+=2)
		//		file_out.R[i][j] = file_out.G[i][j] = file_out.B[i][j] = 255;

		BmpHeadOut();
		BmpSave();
		BmpClose();

		free(tempY);
		free(tempU);
		free(tempV);
		free(tempUV);

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

		arq_temp = fopen("out.tmp", "w+");
		arq_comp = fopen("out.db2", "w+");

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

						//cIn[kY] = '\0';

						//printf("[%d = %d %d, %ld]\n", MYint[i][j][l][a], (int)cY[0], (int)cY[1], kY);

					}//for(a = 0; a < tjan; a++)

		cIn[kY] = '\0';
		ezcompress(cOut, &tamC, cIn, (long)strlen(cIn));
		//huffman_encode_memory(cIn, strlen(cIn), &cOut, &tamC);
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
		//huffman_encode_memory(cIn, strlen(cIn), &cOut, &tamC);
		fwrite(cIn, sizeof(char), kU,  arq_temp);
		fwrite(cOut, sizeof(char), tamC,  arq_comp);
		tamCU = tamC;
		printf("num U = %ld,  comp U = %ld.\n", kU, tamC);

		if(!strcmp("444", argv[4]))
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
			//huffman_encode_memory(cIn, strlen(cIn), &cOut, &tamC);
			tamCV = tamC;
			fwrite(cIn, sizeof(char), kV,  arq_temp);
			fwrite(cOut, sizeof(char), tamC,  arq_comp);
			printf("num V = %ld,  comp V = %ld.\n", kV, tamC);


		}//if(strcmp("440", argv[3]))

		//printf("\nRetorno Compressao = %d.\n", comprime(arq_temp, arq_comp, Z_DEFAULT_COMPRESSION));
		//fwrite(cOut, sizeof(char), size(cOut),  arq_comp);

		printf("Tamanho total = %ld, Tamnho comprimido = %ld, Relacao de Compressao = %f.\n", (kY + kU + kV), (tamCY + tamCU + tamCV), (float)(tamCY + tamCU + tamCV)/file_in.FileSize);
		
		fclose(arq_temp);
		fclose(arq_comp);

		printf("Tempo para %s :%lf(s)\n", argv[1], (time_end - time_ini) / 1000.0f); //Abre em binario arquivo - argv[2]

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

}//void db2_compressao(char** argv)


//funcao principal
void main(int argc, char** argv)
{
  int tam_macro, ecYUV, quant = -1, n_quant = -1, i, j;

  char str_t[4];

  if(argc != 6)
    impr_uso();
  else
    if((strstr(argv[1], ".bmp") != NULL) || (strstr(argv[1], ".BMP") != NULL))
	if((strstr(argv[2], ".bmp") != NULL) || (strstr(argv[2], ".BMP") != NULL))
	{
	  tam_macro = atoi(argv[3]);

	  if(tam_macro == 8 || tam_macro == 16 || tam_macro == 32 || tam_macro == 64 || tam_macro == 128 || tam_macro == 256)
	  {
	    tjan = tam_macro;
	    MQ = alocaMatf(tjan, tjan);
	    ecYUV = atoi(argv[4]);

	    if(ecYUV == 444 || ecYUV == 422 || ecYUV == 440)
	      if(!strncmp(argv[5], "nq=", 3) || !strncmp(argv[5], "q=", 2))
	      {

		if(!strncmp(argv[5], "nq=", 3))
		{
		  i = 3;
		  j = 0;
		  while(&argv[5][i] != NULL && j < 3)
		    str_t[j++] = argv[5][i++];

		  str_t[j] = '\0';
		  n_quant = atoi(str_t);

		}//if(!strncmp(argv[5], "nq=", 3))
		else
        	{
		  i = 2;
		  j = 0;
		  while(&argv[5][i] != NULL && j < 3)
		    str_t[j++] = argv[5][i++];

		  str_t[j] = '\0';
		  quant = atoi(str_t);

		}//if(!strncmp(argv[5], "nq=", 3))

		if((n_quant >= 0 && n_quant <= 100) || (quant >= 0 && quant <= 100))
		{
		  if(n_quant != -1)
		  {
		    fatorf = 496 * n_quant / 100 + 16;
		    printf("\nfatorf = %f\n", fatorf);

		  }//if(n_quant != -1)

		  if(quant != -1)
		  {
		    fator1q = fator2q = 7168 * quant / 100 + 1024;
		    printf("\nfatorq = %f\n", fator1q);

		  }//if(n_quant != -1)

		  db2_compressao(&argv[0]);

		}//if((n_quant >= 0 && n_quant <= 100) || (quant >= 0 && quant <= 100))

		else
		  impr_perfis();

	      }//if(!strncmp(argv[5], "nq=", 3) || !strncmp(argv[5], "q=", 2)

	      else
		impr_perfis();

	    else
	      impr_amostragem();

	  }//if(tam_macro == 8 || ...
	  else
	    impr_tam_bloco();

	}//if((strstr(argv[2], ".bmp") != NULL) || (strstr(argv[2], ".BMP") != NULL))
	else
	  impr_arq_dest();

    else
      impr_arq_origem();

}//void main(int argc, char** argv)

