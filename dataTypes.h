#include <list>

typedef struct st {
	char nome[200];
	char delay[8];
	int copias;
	int prioridade;
	int jobId;
} tipoTupla;

typedef struct {
	std::list<tipoTupla> lista1;
	std::list<tipoTupla> lista2;
	std::list<tipoTupla> lista3;
	std::list<tipoTupla> listaDeEspera;
} tipoEstruturaExecucao;