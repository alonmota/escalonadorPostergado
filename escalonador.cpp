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
void executaProcessosZerados(tipoEstruturaExecucao* filasDeExecucao, 
								std::list<tipoTupla> *listaDeEspera);
void recebeNovosProcessos(int idFila, std::list<tipoTupla> *listaDeEspera);
void rotinaDeControle(tipoEstruturaExecucao* filasDeExecucao);
void rotinaDeExecucao(tipoEstruturaExecucao* filasDeExecucao);


struct sembuf operacao[2];
int idsem;

int p_sem() {
	operacao[0].sem_num = 0;
	operacao[0].sem_op = 0;
	operacao[0].sem_flg = 0;
	operacao[1].sem_num = 0;
	operacao[1].sem_op = 1;
	operacao[1].sem_flg = 0;
	if ( semop(idsem, operacao, 2) < 0)
	  printf("erro no p=%d\n", errno);
}
int v_sem() {
	operacao[0].sem_num = 0;
	operacao[0].sem_op = -1;
	operacao[0].sem_flg = 0;
	if ( semop(idsem, operacao, 1) < 0)
	  printf("erro no p=%d\n", errno);
}


int main () {

	int idshm;
	tipoEstruturaExecucao *filasDeExecucao;
	int pid = fork();

	if ((idsem = semget(0x1223, 1, IPC_CREAT|0x1ff)) < 0) {
	  printf("Erro na criação do semáforo!\n");
	  exit(EXIT_FAILURE);
	}

	if ((idshm = shmget(0x1224, sizeof(tipoEstruturaExecucao), IPC_CREAT|0x1ff)) < 0) {
	  printf("Erro na obtenção da memória compartilhada!\n");
	  exit(EXIT_FAILURE);
	}

	if ( pid < 0 ) {
		printf( "Erro no fork!\n" );
		exit (EXIT_FAILURE);
	} else if (pid == 0) {
		filasDeExecucao = (tipoEstruturaExecucao *) shmat(idshm, (char *)0, 0);
		if (filasDeExecucao == (tipoEstruturaExecucao *)-1) {
			printf("Erro no atach!\n");
			exit(1);
		}
		rotinaDeControle(filasDeExecucao);
	} else {
		filasDeExecucao = (tipoEstruturaExecucao *) shmat(idshm, (char *)0, 0);
		if (filasDeExecucao == (tipoEstruturaExecucao *)-1) {
			printf("Erro no atach!\n");
			exit(1);
		}
		rotinaDeExecucao(filasDeExecucao);
	}

}

void rotinaDeControle(tipoEstruturaExecucao* filasDeExecucao) {
	int idFila;
	std::list<tipoTupla> listaDeEspera;
	
	
	if (idFila = msgget(0x1233, IPC_CREAT | 0x1FF) < 0 ) {
		printf("Erro na criação da fila de mensagens!\n");
		exit(EXIT_FAILURE);
	}

	while (1) {
		atualisaTemporizador(&listaDeEspera);
		recebeNovosProcessos(idFila, &listaDeEspera);
		executaProcessosZerados(filasDeExecucao, &listaDeEspera);
		fflush(stdout);
		sleep(1);
	}
}

void atualisaTemporizador(std::list<tipoTupla> *listaDeEspera) {
	for(auto &exec : *listaDeEspera) {
		exec.delay_sec = exec.delay_sec - 1;
	}
}

void executaProcessosZerados(tipoEstruturaExecucao* filasDeExecucao, 
	std::list<tipoTupla> *listaDeEspera) {

	for(tipoTupla exec : *listaDeEspera) {
		if( exec.delay_sec == 0 ) {
			exec.controleFila = NONE_DOWN;
			switch (exec.prioridade) {
				case 1:
					p_sem();
					filasDeExecucao->lista1.push_back(exec);
					printf("Posto na fila 1\n");					
					v_sem();
					printf("Case 1\n");										
					break;
				case 2:
					p_sem();
					filasDeExecucao->lista2.push_back(exec);
					v_sem();
					printf("Posto na fila 2\n");				
					break;
				case 3:
					p_sem();
					filasDeExecucao->lista3.push_back(exec);
					v_sem();
					printf("Posto na fila 3\n");
					break;
				default:
					printf("Prioridade invalida\n");
			}
			for(tipoTupla t : filasDeExecucao->lista1) {
				printf("   %s,%d,%d,%d,%d\n", t.nome, t.delay_sec, t.copias, t.prioridade, t.jobId );	
			}
		}
	}
	(*listaDeEspera).remove_if([](tipoTupla t){ return t.delay_sec == 0; });
}


void recebeNovosProcessos(int idFila, std::list<tipoTupla> *listaDeEspera) {
	tipoTupla exec;
	while(msgrcv(idFila, &exec, sizeof(tipoTupla), 0, IPC_NOWAIT) != -1) {
		printf("Solicitação de execuçao confirmada!\n");
		(*listaDeEspera).push_back(exec);
		
		printf("lista de Espera\n");
		for(tipoTupla t : *listaDeEspera) {
			printf("   %s,%d,%d,%d,%d\n", t.nome, t.delay_sec, t.copias, t.prioridade, t.jobId );	
		}
	}
}


void rotinaDeExecucao(tipoEstruturaExecucao* filasDeExecucao) {
	/*while(1) {
		if (!filasDeExecucao->lista1.empty()) {
			printf("executou 1");
			p_sem();
			filasDeExecucao->lista1.pop_front();;
			v_sem();
		} else if (!filasDeExecucao->lista2.empty()) {
			printf("executou 2");
			p_sem();
			filasDeExecucao->lista1.pop_front();;
			v_sem();
		} else if (!filasDeExecucao->lista3.empty()) {
			printf("executou 3");
			p_sem();
			filasDeExecucao->lista1.pop_front();;
			v_sem();
		}
		printf("ta todando\n");
		
		fflush(stdout);
		sleep(2);
	}*/
	while(1){
		for(tipoTupla t : filasDeExecucao->lista1) {
			printf("   %s,%d,%d,%d,%d\n", t.nome, t.delay_sec, t.copias, t.prioridade, t.jobId );	
		}
	}
}
