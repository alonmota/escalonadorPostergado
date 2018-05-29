#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(int argc, char *argv[]){
	int idFila;
	tipoMsgRmv msg;
	
	if (idFila = msgget(0x1233, 0x124) < 0 ) {
		printf("Erro na obtebção da fila de mensagens!\n");
		exit(EXIT_FAILURE);
	}
	msg.msgType = 3;
	msg.jobId = (int) strtol(argv[1], (char **)NULL, 10);;
	msgsnd(idFila, &msg, sizeof(msg.jobId), 0);
}
