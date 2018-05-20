

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "dataTypes.h"

int main () {

	int idFila;
	tipoTupla exec;

	if (idFila = msgget(0x1233, 0x124) < 0 ) {
		printf("Erro na criação da fila de mensagens!\n");
		exit(EXIT_FAILURE);
	}

	while(1) {
		msgrcv(idFila, &exec, sizeof(tipoTupla), 0, 0);
		printf("mensagem recebida\n");
		printf("%s,%s,%d,%d,%d\n", exec.nome, exec.delay, exec.copias, exec.prioridade, exec.jobId );
		
	}
	
}
