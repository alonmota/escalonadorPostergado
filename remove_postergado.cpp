#include <stdio.h>
#include <stdlib.h>
#include <dataTypes.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

void remover_processos(std::list<tipoTupla> *listaDeEspera);

int main(int argc, char *argv[]){
	
	int pid = fork();	
	if ( pid < 0 ) {
		printf( "Erro no fork!\n" );
		exit (EXIT_FAILURE);
	} else if (pid == 0) {
		remover_processos(&listaDeEspera);
	}

}

void remover_processos(std::list<tipoTupla> *listaDeEspera){



}