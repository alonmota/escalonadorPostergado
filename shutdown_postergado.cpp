#include <stdio.h>
#include <stdlib.h>
#include <dataTypes.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <sys/shm.h>

int main(int argc, char *argv[]){
	int idshm, *pid;
	tipoTupla exec;

	if(!validateParams(argc, argv)) {
		printf("Argumentos invalidos!\n");
		exit(EXIT_FAILURE);
	}
    
    idshm = shmget(PAI, sizeof(exec), IPC_CREAT|0600);
    pid = (int *) shmat(idshm, NULL, 0);
    
    kill(*pid, SIGUSR1);
    
    return 0;
}