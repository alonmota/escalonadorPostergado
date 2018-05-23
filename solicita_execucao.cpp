

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <dataTypes.h>


bool validateParams(int argc, char* argv[]);
int getUnicJobId(char* argv[]);

int main (int argc, char* argv[]) {

	int idFila;
	int job = 0;
	tipoTupla exec;


	if(!validateParams(argc, argv)) {
		printf("Argumentos invalidos!\n");
		exit(EXIT_FAILURE);
	}
	
	if (idFila = msgget(0x1233, 0x124) < 0 ) {
		printf("Erro na criação da fila de mensagens!\n");
		exit(EXIT_FAILURE);
	}

	strcpy( exec.nome, argv[4] );
	strcpy( exec.delay, argv[1] );
	exec.copias = (int) strtol(argv[2], (char **)NULL, 10);
	exec.prioridade = (int) strtol(argv[3], (char **)NULL, 10);
	exec.jobId = getUnicJobId(argv);

	msgsnd(idFila, &exec, sizeof(tipoTupla), 0);

}


bool validateParams(int argc, char* argv[]) {
	
	//TODO apli correct validations if necessary

	if (argc < 5) {
		return false;
	} else {
		return true;
	}
}

int getUnicJobId(char* argv[]) {

	//TODO generate unic job id here, probably using mem share, now is dummy

	srand((long)argv[1] * (long)argv[2]);
	int job = rand()%500;
	return job;

}
