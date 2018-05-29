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
    
    idshm = shmget(PAI, sizeof(int), IPC_CREAT|0600);
    pid = (int *) shmat(idshm, NULL, 0);
    
    kill(*pid, SIGUSR1);
    
    return 0;
}