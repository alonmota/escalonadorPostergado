#include <list>

#define NONE_DOWN 0
#define ONE_DOWN 1
#define NONE_UP 2
#define ONE_UP 3

typedef struct st {
	char nome[200];
	int delay_sec;
	int copias;
	int prioridade;
	int jobId;
	int controleFila;
} tipoTupla;

typedef struct {
	std::list<tipoTupla> lista1;
	std::list<tipoTupla> lista2;
	std::list<tipoTupla> lista3;
} tipoEstruturaExecucao;