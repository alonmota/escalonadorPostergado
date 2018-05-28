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

typedef struct msg {
	long msgType;
	tipoTupla tupla;
} tipoMensagem;

typedef struct {
	std::list<tipoTupla> fila[3];
} tipoEstruturaExecucao;