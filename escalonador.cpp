#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "dataTypes.h"

void atualisaTemporizador(std::list<tipoTupla> *listaDeEspera);
void executaProcessosZerados(int idFila, std::list<tipoTupla> *listaDeEspera);
void recebeNovosProcessos(int idFila, std::list<tipoTupla> *listaDeEspera);


void rotinaDeControle(int idFilaMsg);
void rotinaDeExecucao(int idFilaMsg);

int main () {

	int idFila;	
	if (idFila = msgget(0x1233, IPC_CREAT | 0x1FF) < 0 ) {
		printf("Erro na criação da fila de mensagens!\n");
		exit(EXIT_FAILURE);
	}

	int pid = fork();	
	if ( pid < 0 ) {
		printf( "Erro no fork!\n" );
		exit (EXIT_FAILURE);
	} else if (pid == 0) {
		rotinaDeControle(idFila);
	} else {
		rotinaDeExecucao(idFila);
	}

}

void rotinaDeControle(int idFila) {
	std::list<tipoTupla> listaDeEspera;

	while (1) {
		atualisaTemporizador(&listaDeEspera);
		recebeNovosProcessos(idFila, &listaDeEspera);
		executaProcessosZerados(idFila, &listaDeEspera);
		fflush(stdout);
		sleep(1);
	}
}

void atualisaTemporizador(std::list<tipoTupla> *listaDeEspera) {
	for(auto &exec : *listaDeEspera) {
		exec.delay_sec = exec.delay_sec - 1;
	}
}

void executaProcessosZerados(int idFila, std::list<tipoTupla> *listaDeEspera) {
	for(auto exec : *listaDeEspera) {
		if( exec.delay_sec == 0 ) {
			exec.controleFila = NONE_DOWN;
			tipoMensagem msg;
			msg.msgType = 2;
			msg.tupla = exec;
			msgsnd(idFila, &msg, sizeof(msg.tupla), 0);
		}
	}
	(*listaDeEspera).remove_if([](tipoTupla t){ return t.delay_sec == 0; });
}


void recebeNovosProcessos(int idFila, std::list<tipoTupla> *listaDeEspera) {
	tipoMensagem msg;
	
	while(msgrcv(idFila, &msg, sizeof(msg.tupla), 1, IPC_NOWAIT) != -1) {
		printf("Solicitação de execuçao confirmada!%d\n",msg.tupla.prioridade);
		(*listaDeEspera).push_back(msg.tupla);
		sleep(1);
	}
}


void rotinaDeExecucao (int idFila) {
	tipoEstruturaExecucao filasDeExecucao;
	tipoMensagem msg;
	while (1) {
		//recebe novos programas q devem começar a execucao
		while (msgrcv(idFila, &msg, sizeof(msg.tupla), 2, IPC_NOWAIT) != -1) {
			for (int i = 0; i<msg.tupla.copias; i++) {
				filasDeExecucao.fila[msg.tupla.prioridade-1].push_back(msg.tupla);
			}
		}
		//controla a execucao



		fflush(stdout);
	}
}
