Observações:
==========
1 - Programa criado e testado em Linux, podendo haver diferenças na saída caso seja utilizado Windows.
==========
==========
2 - alphabet_analysis.c
==========
  OBSERVAÇÃO[1]: ficou ambíguo nos requisitos se os símbolos que aparecem nas palavras
               consideradas inválidas seriam também contabilizados na frequência.
               Por isso, decidimos que, mesmo a palavra sendo inválida, os símbolos
               nela presentes que sejam válidos são adicionados ao alfabeto e 
               contabilizados na frequência.
  OBSERVAÇÃO[2]: também devido a essa ambiguidade, não sabíamos se as palavras
                inválidas deveriam ser consideradas no total de palavras do texto.
                Com isso, optamos por considerar apenas as palavras válidas para
                serem calculadas no total, e descartamos repetições.

==========
2 - 5377855-7493726-Horspool.c e 5377855-7493726-BoyerMoore.c
==========
        - Copie o texto e cole em um editor de textos simples, como o Notepad
    do Windows, sem quebra de linha. Foi observado que textos extraídos de .zip
    ou de editores que fazem quebra de linha adicionam caracteres a mais que irão
    atrapalhar na hora de dizer onde começa cada ocorrência da palavra no texto.
        - O programa possui uma limitação de entrada de no máximo 60000 palavras
    válidas possíveis, bem como no máximo 500000 ocorrências de cada uma delas.