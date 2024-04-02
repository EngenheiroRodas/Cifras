Usage: $ ./cifras [OPTIONS]

A tool for encrypting and decrypting text using various methods.

Options:
-h display user help
-i filename input file name, alternatively stdin
-o filename output file name, alternatively stdout
-s password password to use for encrypting/decrypting, alternatively "Programacao2024"
-f filters the input file and formats the output file
-c nn the operation to perform should be encrypt, with method nn
-d nn the operation to perform should be decrypt, with method nn
-e the operation to perform should be calculate statistics
-a nn the operation to perform should be attack, with method nn
-n nn for attack method 3, the maximum key size should be nn, instead of 20
-w filename name of dictionary file to use

Valid program invocation examples:
Example 1: ./cifras -h

Example 2: ./cifras -c 1

Example 3: ./cifras -d 1 -s Programacao

Example 4: ./cifras -c 1 -i 1line.txt -o 1line-cesar.txt


Invalid program invocation examples:
Example 1: ./cifras -c 20

Example 2: ./cifras -c 1 -s
