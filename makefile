
solicita-execucao:
	gcc -std=c99  -o solicita_execucao solicita_execucao.c

escalonador:
	gcc -std=c99  -o escalonador escalonador.c

bild:
	gcc -std=c99  -o solicita_execucao solicita_execucao.c -I.
	gcc -std=c99  -o escalonador escalonador.c -I.

clear:
	rm solicita_execucao escalonador