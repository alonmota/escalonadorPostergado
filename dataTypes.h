#include <list>

#define MAX_COPIES 50

#define NONE_DOWN 0
#define ONE_DOWN 1
#define NONE_UP 2
#define ONE_UP 3


typedef struct {
	char nome[200];
	char hora[8];
	int delay_sec;
	int copias;
	int prioridade;
	int jobId;
	int controleFila;
} tipoTupla;

typedef struct {
	long msgType;
	tipoTupla tupla;
} tipoMensagem;

typedef struct {
	long msgType;
	int jobId;
} tipoMsgRmv;

typedef struct {
	pid_t pid;
	int status;
} tipoProgExec;

typedef struct {
	std::list<tipoProgExec> fila[3];
} tipoEstruturaExecucao;