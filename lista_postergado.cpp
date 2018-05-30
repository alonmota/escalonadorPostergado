#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>


#include "dataTypes.h"

int main(){
	int idFila;
	tipoMsgRmv msg;
	
	if (idFila = msgget(0x1233, 0x124) < 0 ) {
		printf("Erro na obtebção da fila de mensagens!\n");
		exit(EXIT_FAILURE);
	}
	
	msg.msgType = 3;
	msg.jobId = -1;
	msgsnd(idFila, &msg, sizeof(msg.jobId), 0);
}
