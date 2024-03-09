O arquivo exemplos.zip tem os seguintes ficheiros:

-rwxrwx---       82 fev 12 22:03 dic-small.txt		dicionário de Inglês com 9 palavras
-rwxrwx---       32 fev 12 15:47 dic-very-small.txt	dicionário de Inglês com 20 palavras
-rwxrwx---   860381 jan 20  2022 en_US.dic		dicionário de Inglês com 79014 linhas/palavras
drwxrwx---     4096 fev 12 23:12 expected1		pasta com os resultados esperados na execução do script "run1"
drwxrwx---     4096 fev 12 22:04 expected2		pasta com os resultados esperados na execução do script "run2"
drwxrwx---     4096 fev 12 18:19 in			pasta com os ficheiros de input para os testes
-rw-rw-r--     2400 fev 16 22:13 readme.txt		este ficheiro
-rwxrwx---     2608 fev 12 23:08 run1			script para executar testes correspondentes à 1ª fase do projecto
-rwxrwx---     3117 fev 12 21:58 run2			script para executar testes correspondentes à 2ª fase do projecto
-rwxrwx---   985084 jan 20  2022 words			dicionário de Inglês com 104334 linhas/palavras

Para usar estes exemplos, deve descomprimir o arquivo exemplos.zip para a pasta onde tem o programa
executável "cifras" do seu projecto. 

Depois pode correr o bash script "./run1" para executar testes ao seu programa usando apenas stdin e stdout,
e verificar o funcionamento do seu programa com as funcionalidades correspondentes à 1ª fase do projecto.
Se quiser, pode ver o conteúdo do script com um editor de texto.
O script cria uma pasta "out1". Para cada ficheiro da pasta "in", corre o programa, pondo o output da cifra de César,
o ouput da cifra de Vigenère sem filtragem e com filtragem na pasta "out1". 
Depois decifra cada um dos ficheiros cifrado com a cifra de César para a pasta "cesar1" e cada um dos
ficheiros cifrado com a cifra de Vigenère para a pasta "vigenere1" (estas pastas são criadas).
No fim, o script compara as pastas "out1" com a "expected1", que tem o resultado esperado, mostrando as diferenças; 
compara a pasta "cesar1" com a "in", mostrando as diferenças; e compara a pasta "vigenere1" com a "in",
mostrando as diferenças.

Para a segunda fase do projecto, pode correr o bash script "./run2" para e executar testes ao seu programa
usando as opções de leitura e escrita de ficheiros, para cifra, decifra e ataques com os diferentes métodos.
Estes script funciona de forma semelhante ao script "run1", mas com pastas com nomes terminados em "2".

Bom trabalho!
