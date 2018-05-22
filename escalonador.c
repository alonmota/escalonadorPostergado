#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "dataTypes.h"

typedef struct estrutura{
    char nome[50], delay[100];
    int copias, prioridade, jobId;
    struct estrutura *prox;
}*vetor[n], lista, *elo;

int main () {
	elo aux, novo;
	int idFila;
	tipoTupla exec;

	/*Cabeça da lista*/
	novo = (elo*) malloc(sizeof(lista));
	novo->nome = NULL;
	novo->delay = NULL;
	novo->copias = NULL;
	novo->prioridade = NULL;
	novo->jobId = NULL;
	novo->prox = NULL;


	if (idFila = msgget(0x1233, 0x124) < 0 ) {
		printf("Erro na criação da fila de mensagens!\n");
		exit(EXIT_FAILURE);
	}

	while(1) {		
		msgrcv(idFila, &exec, sizeof(tipoTupla), 0, 0);
		printf("mensagem recebida\n");
		printf("%s,%s,%d,%d,%d\n", exec.nome, exec.delay, exec.copias, exec.prioridade, exec.jobId );                                                                           

		if(exec.prioridade = 1){
			aux = vetor[0];

			while (aux->prox!=NULL){
	    	aux = aux->prox;                    
	    	}

			novo  = (elo) malloc(sizeof(lista));      
			aux->prox = novo;                             
			novo->prox = NULL;                           
			novo->nome = exec.nome;
			novo->delay = exec.delay;
			novo->copias = exec.copias;
			novo->prioridade = exec.prioridade;
			novo->jobId = exec.jobId;

		}else if(exec.prioridade = 2){
			aux = vetor[1];

			while (aux->prox!=NULL){
	    	aux = aux->prox;                    
	    	}

			novo  = (elo) malloc(sizeof(lista));      
			aux->prox = novo;                             
			novo->prox = NULL;                           
			novo->nome = exec.nome;
			novo->delay = exec.delay;
			novo->copias = exec.copias;
			novo->prioridade = exec.prioridade;
			novo->jobId = exec.jobId;

		}else if(exec.prioridade = 3){
			aux = vetor[2];

			while (aux->prox!=NULL){
	    	aux = aux->prox;                    
	    	}

			novo  = (elo) malloc(sizeof(lista));      
			aux->prox = novo;                             
			novo->prox = NULL;                           
			novo->nome = exec.nome;
			novo->delay = exec.delay;
			novo->copias = exec.copias;
			novo->prioridade = exec.prioridade;
			novo->jobId = exec.jobId;

		}else{
			printf("Prioridade invalida\n");
		}
		
	}
	
}
