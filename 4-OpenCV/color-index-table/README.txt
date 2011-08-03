===========================
1. Pasta "Sem-Subamostragem"
===========================
Primeira tentativa de reversão utilizando a tabela de índice de cores composta por Cb e Cr de ambas imagens do par estéreo.


==================================
2. Pasta "Anaglifico-complementar" > ABORDAGEM UTILIZADA ATUALMENTE
==================================
Início da estratégia utilizando o anáglifo complementar, isto é, cria-se o anáglifo verde-magenta (anáglifo principal) e utiliza os canais de cores restantes para criação de outro anáglifo, complementar.

	==============================================
	2.1 Pasta "Anaglifico-complementar" -> "com Y"
	==============================================
	Simplesmente transforma o anáglifo complementar em YCbCr com subamostragem 4:2:2 e o armazena deste jeito junto com o anáglifo principal. Somente para testes, já descartada.
	
	==============================================
	2.2 Pasta "Anaglifico-complementar" -> "sem Y" > ABORDAGEM PARA O WEBMEDIA 2011
	==============================================
	Estratégia utilizada para publicação do artigo no WebMedia 2011. Transforma o anáglifo complementar em YCbCr com subamostragem 4:2:2 e descarta a componente Y. Na reversão, transforma o anáglifo principal em YCbCr com subamostragem 4:2:2 e utiliza a componente Y resultante em conjunto com o Cb e Cr do anáglifo complementar. Deste modo, restaura os dois anáglifos para RGB e reordena os canais de cores para obter o par estéreo. Observe que é utilizada a componente Y do anáglifo principal para recuperar o anáglifo complementar, e essa componente não é exatamente igual à componente do anáglifo complementar, o que causa crosstalking no par estéreo revertido. Porém, a qualidade visual ainda é boa e é possível criar um novo anáglifo com sensação de profundidade a partir do par estéreo revertido.
	
	===============================================================
	2.3 Pasta "Anaglifico-complementar" -> "sub-440-no-par-estereo" > ABORDAGEM PARA O SAC 2011
	===============================================================
	Primeira abordagem para melhorar a compressão obtida na abordagem 2.2. Agora iremos armazenar tanto o anáglifo principal quanto o complementar (sem a componente Y) convertidos para YCbCr e utilizando a subamostragem 4:4:0. Esse tipo de subamostragem foi testado pelo Leonardo A. Andrade e se mostrou de qualidade superior ante os outros tipos de subamostragem. A hipótese é que isso ocorre porque na 4:4:0, a subamostragem é feita horizontalmente (de uma linha armazena as componentes Y, Cb e Cr de cada pixel, da próxima, apenas a componente Y e assim por diante), o que não afetaria tanto pelo fato de a gente ter os olhos na horizontal. Na 4:2:2, a subamostragem é feita verticalmente (a cada pixel, numa varredura da esquerda para a direita, de cima para baixo, é amostrado as componentes Y, Cb e Cr, e no próximo pixel, somente a componente Y, e assim por diante). Com a 4:2:2, temos perda na resolução horizontal, o que afetaria a qualidade por nossa visão também ser na horizontal.