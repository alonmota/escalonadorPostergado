/******************************************
*UnB - Universidade de Brasília
*Sistemas Operacionais
*Prof. Dra Alba Cristina Melo
*Gerente de escalonador postergado de processos
*Alon Mota Lourenço - 130005002
*Rodrigo de Sousa Saldanha - 110139143
*
* Informações adicionais no README
*******************************************/


#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include "dataTypes.h"

struct sembuf operacao[2];
int idsem;
tipoEstruturaExecucao filasDeExecucao;
pid_t running = 0;
int fila = -1;

int p_sem()
{
     operacao[0].sem_num = 0;
     operacao[0].sem_op = 0;
     operacao[0].sem_flg = 0;
     operacao[1].sem_num = 0;
     operacao[1].sem_op = 1;
     operacao[1].sem_flg = 0;
     if ( semop(idsem, operacao, 2) < 0)
       printf("Erro na criação do semaforo\n");
}
int v_sem()
{
     operacao[0].sem_num = 0;
     operacao[0].sem_op = -1;
     operacao[0].sem_flg = 0;
     if ( semop(idsem, operacao, 1) < 0)
       printf("Erro na criação do semaforo\n");
}

int main () {
	//rotinas de tratamento de sinais
	signal(SIGALRM, funcao_sigalarm);
	signal(SIGUSR2, shutdown);
	
	//obtendo mensagens
	int idFila;	
	if (idFila = msgget(0x1233, IPC_CREAT | 0x1FF) < 0 ) {
		printf("Erro na criação da fila de mensagens!\n");
		exit(EXIT_FAILURE);
	}

	//obtendo semaforos
	if ((idsem = semget(0x1223, 1, IPC_CREAT|0x1ff)) < 0) {
		printf("Erro na criação do semaforo\n");
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
	tipoMsgRmv msgRmv;

	//recebe tipo 1 = adicionar na lista de espera
	while(msgrcv(idFila, &msg, sizeof(msg.tupla), 1, IPC_NOWAIT) != -1) {
		//printf("\nSolicitação de execuçao confirmada!\n");
		(*listaDeEspera).push_back(msg.tupla);

		printf("\n%-5.5s %-25.23s %-6.6s %-5.5s %-5.5s\n", "job", "arq_exec", "hh:mm", "cop", "pri");
		for(auto exec : (*listaDeEspera)){
			printf("%-5d %-25.23s %-6.6s %-5d %-5d\n", exec.jobId, exec.nome, exec.hora, exec.copias, exec.prioridade);
		}
		fflush(stdout);		
	}

	//recebe tipo 3 remove fila espera
	while(msgrcv(idFila, &msgRmv, sizeof(msgRmv.jobId), 3, IPC_NOWAIT) != -1) {
		for (std::list<tipoTupla>::iterator t = listaDeEspera->begin(); 
			t != listaDeEspera->end(); ) {
			if (t->jobId == msgRmv.jobId) {
				t = listaDeEspera->erase(t);
			} else {
				t++;
			}
		}
		printf("\n%-5.5s %-25.23s %-6.6s %-5.5s %-5.5s\n", "job", "arq_exec", "hh:mm", "cop", "pri");
		for(auto exec : (*listaDeEspera)){
			printf("%-5d %-25.23s %-6.6s %-5d %-5d\n", exec.jobId, exec.nome, exec.hora, exec.copias, exec.prioridade);
		}
		fflush(stdout);
	}
}


void rotinaDeExecucao (int idFila) {
	tipoMensagem msg;
	while (1) {
		pid_t pids[MAX_COPIES];
		
		//recebe novos programas q devem começar a execucao
		while (msgrcv(idFila, &msg, sizeof(msg.tupla), 2, IPC_NOWAIT) != -1) {
			for (int i = 0; i < msg.tupla.copias; i++) {
				if ((pids[i] = fork()) < 0) {
					printf("Erro na criação do processo!\n");
					exit(EXIT_FAILURE);
				} else if (pids[i] == 0) { //executa codigo do filho, chama o programa
					if (execl(msg.tupla.nome, msg.tupla.nome, (char *) NULL) < 0){
						printf("Erro no execl = %d\n", errno);
					}
				} else {
					tipoProgExec buf;
					buf.status = NONE_DOWN;
					buf.pid = pids[i];
					//força a parada dos programas para que seja feito o controle de apenas um rodar 
					kill(pids[i], SIGSTOP);
					p_sem();
					fila = msg.tupla.prioridade-1;
					filasDeExecucao.fila[msg.tupla.prioridade-1].push_back(buf);
					v_sem();
				}
			}
		}
		funcao_sigalarm(0);
		
		fflush(stdout);
	}
}

void funcao_sigalarm(int signal_number) {
	if(signal_number == SIGALRM){
		tipoProgExec prog;
		//se nenhum processo esta rodando coloca o primeiro viavel a rodar ou espera 
		if (running == 0) {
			if (!filasDeExecucao.fila[0].empty()){
				prog = filasDeExecucao.fila[0].front();
				alarm(5);
				//printf("mandou executar fila 0 prog %d \n", prog.pid);
				kill(prog.pid, SIGCONT);
				p_sem();
				running = prog.pid;
				fila = 0;
				v_sem();
			} else if (!filasDeExecucao.fila[1].empty()){
				prog = filasDeExecucao.fila[1].front();
				alarm(5);
				//printf("mandou executar fila 1 prog %d \n", prog.pid);
				kill(prog.pid, SIGCONT);
				p_sem();
				running = prog.pid;
				fila = 1;
				v_sem();
			} else if (!filasDeExecucao.fila[2].empty()){
				prog = filasDeExecucao.fila[2].front();
				alarm(5);
				//printf("mandou executar fila 2 prog %d \n", prog.pid);
				kill(prog.pid, SIGCONT);
				p_sem();
				running = prog.pid;
				fila = 2;
				v_sem();
			} else {
				alarm(1);
			}
		} else if (signal_number != 0 ) {
			//se o temporizador esgotou para o processo rodando calcula prioridade e esvazia a execução
			tipoProgExec aux;		
			p_sem();
			aux = filasDeExecucao.fila[fila].front();
			kill(aux.pid, SIGSTOP);
			if(aux.status == NONE_DOWN) {
				aux.status = ONE_DOWN;
				filasDeExecucao.fila[fila].push_back(aux);
			} else if (aux.status == ONE_DOWN) {
				if (fila == 2) {
					aux.status = NONE_UP;
					filasDeExecucao.fila[fila-1].push_back(aux);
				} else {
					aux.status = NONE_DOWN;
					filasDeExecucao.fila[fila+1].push_back(aux);
				}
			} else if (aux.status == NONE_UP) {
				aux.status = ONE_UP;
				filasDeExecucao.fila[fila].push_back(aux);
			} else if (aux.status == ONE_UP) {
				if (fila == 0) {
					aux.status = NONE_DOWN;
					filasDeExecucao.fila[fila+1].push_back(aux);
				} else {
					aux.status = NONE_UP;
					filasDeExecucao.fila[fila-1].push_back(aux);
				}
			}
			filasDeExecucao.fila[fila].pop_front();
			running = 0;
			fila = -1;
			v_sem();
			
		} else {
			int status;		
			pid_t buf;
			//remove os processos zumbis 
			if ((buf = waitpid(-1, &status, WNOHANG)) > 0 ) {
				//printf("ecerrou %d\n", buf);
				for(int i = 0; i < 3; i++) {
					for (std::list<tipoProgExec>::iterator t = filasDeExecucao.fila[i].begin(); 
						t != filasDeExecucao.fila[i].end(); ) {
						if (t->pid == buf) {
							t = filasDeExecucao.fila[i].erase(t);
						} else {
							t++;
						}
					}
				}
			}
		}
	}
}


void shutdown(int signal_number) {
	printf("recebi um signal");
	fflush(stdout);

}
