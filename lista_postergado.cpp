#include <stdio.h>
#include <stdlib.h>
#include <dataTypes.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

void listar_processos(std::list<tipoTupla> *listaDeEspera);

int main(int argc, char *argv[]){

	int pid = fork();	
	if ( pid < 0 ) {
		printf( "Erro no fork!\n" );
		exit (EXIT_FAILURE);
	} else if (pid == 0) {
		listar_processos(&listaDeEspera);
	}

}

void listar_processos(std::list<tipoTupla> *listaDeEspera){
	int i;	

	printf("job  arq_exec  hhmm  copias  pri\n");
	while(1){
		for (int i = 0; i<3; i++) {
			printf("fila %d\n", i+1);
			for(auto exec : filasDeExecucao.fila[i]) {
				printf("%d  %s  %d  %d  %d\n", tupla.jobId, tupla.nome, tupla.delay_sec, tupla.copias, tupla.prioridade);
			}
		}
	}
}