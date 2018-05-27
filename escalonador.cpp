#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <string.h>
#include <unistd.h>

#include "dataTypes.h"

void atualisaTemporizador(tipoEstruturaExecucao* filasDeExecucao);
void executaProcessosZerados(tipoEstruturaExecucao* filasDeExecucao);
void recebeNovosProcessos(int idFila, tipoEstruturaExecucao* filasDeExecucao);

int main () {

	int idFila;
	tipoEstruturaExecucao filasDeExecucao;

	if (idFila = msgget(0x1233, IPC_CREAT | 0x1FF) < 0 ) {
		printf("Erro na criação da fila de mensagens!\n");
		exit(EXIT_FAILURE);
	}

	while (1) {
		atualisaTemporizador(&filasDeExecucao);
		recebeNovosProcessos(idFila, &filasDeExecucao);
		executaProcessosZerados(&filasDeExecucao);
		fflush(stdout);
		sleep(1);
	}
}

void atualisaTemporizador(tipoEstruturaExecucao* filasDeExecucao) {
	
	for(auto &exec : filasDeExecucao->listaDeEspera) {
		exec.delay_sec = exec.delay_sec - 1;
	}

}

void executaProcessosZerados(tipoEstruturaExecucao* filasDeExecucao) {

	for(tipoTupla exec : filasDeExecucao->listaDeEspera) {
		if( exec.delay_sec == 0 ) { 
			switch (exec.prioridade) {
				case 1:
					filasDeExecucao->lista1.push_back(exec);
					printf("Posto na fila 1\n");
					break;
				case 2: 
					filasDeExecucao->lista2.push_back(exec);
					printf("Posto na fila 2\n");				
					break;
				case 3: 
					filasDeExecucao->lista3.push_back(exec);
					printf("Posto na fila 3\n");
					break;
				default:
					printf("Prioridade invalida\n");
			}
		}
	}
	filasDeExecucao->listaDeEspera.remove_if([](tipoTupla t){ return t.delay_sec == 0; });
}


void recebeNovosProcessos(int idFila, tipoEstruturaExecucao* filasDeExecucao) {
	tipoTupla exec;
	while(msgrcv(idFila, &exec, sizeof(tipoTupla), 0, IPC_NOWAIT) != -1) {
		printf("Solicitação de execuçao confirmada!\n");
		filasDeExecucao->listaDeEspera.push_back(exec);
		
		printf("lista de Espera\n");
		for(tipoTupla t : filasDeExecucao->listaDeEspera) {
			printf("   %s,%d,%d,%d,%d\n", t.nome, t.delay_sec, t.copias, t.prioridade, t.jobId );	
		}
	}
}
