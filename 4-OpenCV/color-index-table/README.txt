===========================
1. Pasta "Sem-Subamostragem"
===========================
Primeira tentativa de revers�o utilizando a tabela de �ndice de cores composta por Cb e Cr de ambas imagens do par est�reo.


==================================
2. Pasta "Anaglifico-complementar" > ABORDAGEM UTILIZADA ATUALMENTE
==================================
In�cio da estrat�gia utilizando o an�glifo complementar, isto �, cria-se o an�glifo verde-magenta (an�glifo principal) e utiliza os canais de cores restantes para cria��o de outro an�glifo, complementar.

	==============================================
	2.1 Pasta "Anaglifico-complementar" -> "com Y"
	==============================================
	Simplesmente transforma o an�glifo complementar em YCbCr com subamostragem 4:2:2 e o armazena deste jeito junto com o an�glifo principal. Somente para testes, j� descartada.
	
	==============================================
	2.2 Pasta "Anaglifico-complementar" -> "sem Y" > ABORDAGEM PARA O WEBMEDIA 2011
	==============================================
	Estrat�gia utilizada para publica��o do artigo no WebMedia 2011. Transforma o an�glifo complementar em YCbCr com subamostragem 4:2:2 e descarta a componente Y. Na revers�o, transforma o an�glifo principal em YCbCr com subamostragem 4:2:2 e utiliza a componente Y resultante em conjunto com o Cb e Cr do an�glifo complementar. Deste modo, restaura os dois an�glifos para RGB e reordena os canais de cores para obter o par est�reo. Observe que � utilizada a componente Y do an�glifo principal para recuperar o an�glifo complementar, e essa componente n�o � exatamente igual � componente do an�glifo complementar, o que causa crosstalking no par est�reo revertido. Por�m, a qualidade visual ainda � boa e � poss�vel criar um novo an�glifo com sensa��o de profundidade a partir do par est�reo revertido.
	
	===============================================================
	2.3 Pasta "Anaglifico-complementar" -> "sub-440-no-par-estereo" > ABORDAGEM PARA O SAC 2011
	===============================================================
	Primeira abordagem para melhorar a compress�o obtida na abordagem 2.2. Agora iremos armazenar tanto o an�glifo principal quanto o complementar (sem a componente Y) convertidos para YCbCr e utilizando a subamostragem 4:4:0. Esse tipo de subamostragem foi testado pelo Leonardo A. Andrade e se mostrou de qualidade superior ante os outros tipos de subamostragem. A hip�tese � que isso ocorre porque na 4:4:0, a subamostragem � feita horizontalmente (de uma linha armazena as componentes Y, Cb e Cr de cada pixel, da pr�xima, apenas a componente Y e assim por diante), o que n�o afetaria tanto pelo fato de a gente ter os olhos na horizontal. Na 4:2:2, a subamostragem � feita verticalmente (a cada pixel, numa varredura da esquerda para a direita, de cima para baixo, � amostrado as componentes Y, Cb e Cr, e no pr�ximo pixel, somente a componente Y, e assim por diante). Com a 4:2:2, temos perda na resolu��o horizontal, o que afetaria a qualidade por nossa vis�o tamb�m ser na horizontal.