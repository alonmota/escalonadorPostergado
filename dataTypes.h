#include <list>

#define MAX_COPIES 50


/**
 * Defines usados para controlar em qual fila um processo deve ser colocado
*/
#define NONE_DOWN 0 //indica se é a primeira execução na fila e deve subir
#define ONE_DOWN 1 //indica se é a segunda execução na fila e deve subir
#define NONE_UP 2 //indica se é a primeira execução na fila e deve descer
#define ONE_UP 3 //indica se é a segunda execução na fila e deve descer


/**
 * Estrutura usada para armazenar informações das tuplas a serem executadas
*/
typedef struct {
	char nome[200];
	char hora[8];
	int delay_sec;  //Para facilitar o controle do tempo
	int copias;
	int prioridade;
	int jobId;
	int controleFila;
} tipoTupla;


/**
 * Struct para enviar uma tupla como mensagen
*/
typedef struct {
	long msgType;
	tipoTupla tupla;
} tipoMensagem;

/**
 * Struct para enviar um jobId como mensagen
*/
typedef struct {
	long msgType;
	int jobId;
} tipoMsgRmv;

/**
 * Struct para controlar a execucao de um programa que ja começõu
*/
typedef struct {
	pid_t pid;
	int status;
} tipoProgExec;


/**
 * Struct para controle das filas de prioridades do escalonador
*/
typedef struct {
	std::list<tipoProgExec> fila[3];
} tipoEstruturaExecucao;


/**
 * Função chamada a cada segundo para reduzir o contador dos programas em espera
 * @param listaDeEspera = Lista das tuplas em espera
*/
void atualisaTemporizador(std::list<tipoTupla> *listaDeEspera);

/**
 * Função chamada a cada segundo eviar uma mensagem a rotina de execuçao para cada
 * programa cujo temporizador de espera chegou a 0 e deve ser executado;
 * @param listaDeEspera = Lista das tuplas em espera
 * @param idFila = Id das filas de mensagem
*/
void executaProcessosZerados(int idFila, std::list<tipoTupla> *listaDeEspera);

/**
 * Função chamada a cada segundo para verificar se chegou alguma nova mensagem pedindo a 
 * espera de execução de uma tupla
 * @param listaDeEspera = Lista das tuplas em espera
 * @param idFila = Id das filas de mensagem
*/
void recebeNovosProcessos(int idFila, std::list<tipoTupla> *listaDeEspera);

/**
 * Rotina de tratamento do SIGALARM, troca o programa em execução pelo prox que deve ser executado
 * @param signal_number = numero do sinal que despertou a funçao
*/
void funcao_sigalarm(int signal_number);

/**
 * Rotina de tratamento do SIGUSR1, nao implementado
 * @param signal_number = numero do sinal que despertou a funçao
*/
void shutdown(int signal_number);


/**
 * Rotina rodada pelo filho que controla a lista de espera e coloca em execuçao processos zerados
 * @param idFilaMsg = id da fila de mensagens
*/
void rotinaDeControle(int idFilaMsg);

/**
 * Rotina rodada pelo pai que controla a execução dos programas
 * @param idFilaMsg = id da fila de mensagens
*/
void rotinaDeExecucao(int idFilaMsg);