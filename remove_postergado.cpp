#include <stdio.h>
#include <stdlib.h>
#include <dataTypes.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


int main(int argc, char *argv[]){
	tipoTupla exec;

	if(!validateParams(argc, argv)) {
		printf("Argumentos invalidos!\n");
		exit(EXIT_FAILURE);
	}

	exec.jobId = getUnicJobId(argv);
	
	for(auto exec : *listaDeEspera) {
		if( exec.delay_sec == argv[1] ) {
			//tirar elemento da estrutura
		}else{
			printf("Nao foi possivel remover processo\n");
		}
	}

}
