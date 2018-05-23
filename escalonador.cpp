#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <string.h>
#include <time.h>

#include "dataTypes.h"

void atualisaTemporizador();
void executaProcessosZerados(tipoEstruturaExecucao* filasDeExecucao);
void recebeNovosProcessos(int idFila, tipoEstruturaExecucao* filasDeExecucao);

int main () {

	int idFila;
	tipoEstruturaExecucao filasDeExecucao;

	if (idFila = msgget(0x1233, IPC_CREAT | 0x1FF) < 0 ) {
		printf("Erro na criação da fila de mensagens!\n");
		exit(EXIT_FAILURE);
	}

	atualisaTemporizador();
	recebeNovosProcessos(idFila, &filasDeExecucao);
	executaProcessosZerados(&filasDeExecucao);

}

void atualisaTemporizador() {
}

void executaProcessosZerados(tipoEstruturaExecucao* filasDeExecucao) {
	for(tipoTupla exec : filasDeExecucao->listaDeEspera) {
		if( strcmp(exec.delay, "0:00")  == 0 ) { //TODO descobrir como trabalhar com tempo
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
			printf("lsita 1\n");
			for(tipoTupla t : filasDeExecucao->lista1) {
				printf("   %s,%s,%d,%d,%d\n", t.nome, t.delay, t.copias, t.prioridade, t.jobId );	
			}
			printf("lsita 2\n");
			for(tipoTupla t : filasDeExecucao->lista2) {
				printf("   %s,%s,%d,%d,%d\n", t.nome, t.delay, t.copias, t.prioridade, t.jobId );	
			}
			printf("lsita 3\n");
			for(tipoTupla t : filasDeExecucao->lista3) {
				printf("   %s,%s,%d,%d,%d\n", t.nome, t.delay, t.copias, t.prioridade, t.jobId );	
			}
			printf("lsita de Espera\n");
			for(tipoTupla t : filasDeExecucao->listaDeEspera) {
				printf("   %s,%s,%d,%d,%d\n", t.nome, t.delay, t.copias, t.prioridade, t.jobId );	
			}
		}
	}
	filasDeExecucao->listaDeEspera.remove_if([](tipoTupla t){ return strcmp(t.delay, "0:00")  == 0; });
}


void recebeNovosProcessos(int idFila, tipoEstruturaExecucao* filasDeExecucao) {
	tipoTupla exec;
	while(msgrcv(idFila, &exec, sizeof(tipoTupla), 0, IPC_NOWAIT) != -1) {
		printf("mensagem recebida\n");
		filasDeExecucao->listaDeEspera.push_back(exec);
	
		printf("lsita 1\n");
		for(tipoTupla t : filasDeExecucao->lista1) {
			printf("   %s,%s,%d,%d,%d\n", t.nome, t.delay, t.copias, t.prioridade, t.jobId );	
		}
		printf("lsita 2\n");
		for(tipoTupla t : filasDeExecucao->lista2) {
			printf("   %s,%s,%d,%d,%d\n", t.nome, t.delay, t.copias, t.prioridade, t.jobId );	
		}
		printf("lsita 3\n");
		for(tipoTupla t : filasDeExecucao->lista3) {
			printf("   %s,%s,%d,%d,%d\n", t.nome, t.delay, t.copias, t.prioridade, t.jobId );	
		}
		printf("lsita de Espera\n");
		for(tipoTupla t : filasDeExecucao->listaDeEspera) {
			printf("   %s,%s,%d,%d,%d\n", t.nome, t.delay, t.copias, t.prioridade, t.jobId );	
		}
	}

	
}
