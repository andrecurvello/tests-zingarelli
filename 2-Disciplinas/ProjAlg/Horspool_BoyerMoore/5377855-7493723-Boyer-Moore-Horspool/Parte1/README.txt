Observa��es:
==========
1 - Programa criado e testado em Linux, podendo haver diferen�as na sa�da caso seja utilizado Windows.
==========
==========
2 - alphabet_analysis.c
==========
  OBSERVA��O[1]: ficou amb�guo nos requisitos se os s�mbolos que aparecem nas palavras
               consideradas inv�lidas seriam tamb�m contabilizados na frequ�ncia.
               Por isso, decidimos que, mesmo a palavra sendo inv�lida, os s�mbolos
               nela presentes que sejam v�lidos s�o adicionados ao alfabeto e 
               contabilizados na frequ�ncia.
  OBSERVA��O[2]: tamb�m devido a essa ambiguidade, n�o sab�amos se as palavras
                inv�lidas deveriam ser consideradas no total de palavras do texto.
                Com isso, optamos por considerar apenas as palavras v�lidas para
                serem calculadas no total, e descartamos repeti��es.

==========
2 - 5377855-7493726-Horspool.c e 5377855-7493726-BoyerMoore.c
==========
        - Copie o texto e cole em um editor de textos simples, como o Notepad
    do Windows, sem quebra de linha. Foi observado que textos extra�dos de .zip
    ou de editores que fazem quebra de linha adicionam caracteres a mais que ir�o
    atrapalhar na hora de dizer onde come�a cada ocorr�ncia da palavra no texto.
        - O programa possui uma limita��o de entrada de no m�ximo 60000 palavras
    v�lidas poss�veis, bem como no m�ximo 500000 ocorr�ncias de cada uma delas.