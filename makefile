CC = g++
FLAGS = -std=c++11


solicita-execucao:
	${CC} ${FLAGS} -o solicita_execucao solicita_execucao.cpp -I.

escalonador:
	${CC} ${FLAGS} -o escalonador escalonador.cpp -I.

bild:
	${CC} ${FLAGS} -o teste teste.cpp
	${CC} ${FLAGS} -o solicita_execucao solicita_execucao.cpp -I.
	${CC} ${FLAGS} -o escalonador escalonador.cpp -I.

clear:
	rm solicita_execucao escalonador teste