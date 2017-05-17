#define _XOPEN_SOURCE

#include "server.h"

int server_msgqid;
int msgqid;
char *homedir;
const int lineSize = 100;
int runner = 1;

void sendToServer(message *msg);
void terminate();
void echo();
void gettime();
void vers();

int main(int argc,char **argv) {

  if ((homedir = getenv("HOME")) == NULL) {
      perror("CLIENT - Getting homedir failed");
      exit(EXIT_FAILURE);
  }

  key_t key;
  if((key = ftok(homedir, 'p')) < 0){
    perror("CLIENT - FTOK failed");
    exit(EXIT_FAILURE);
  }

  server_msgqid = msgget(key, 0);
	if (server_msgqid < 0) {
		perror("CLIENT - MSGQID failed");
		exit(EXIT_FAILURE);
	}

  printf("CLIENT %d | Server queue %d accesed\n", getpid(), server_msgqid);

  msgqid = msgget(IPC_PRIVATE, IPC_CREAT | 0666); //msgflg = IPC_CREAT | 0666 -- create queue and make it read and appendable by all.
	if (msgqid < 0) {
		perror("CLIENT - Queue create failed");
		exit(EXIT_FAILURE);
	}
	printf("CLIENT %d | Queue %d created\n", getpid(), msgqid);

  //
  //login
  //
  message tmp;
	tmp.mtype = 1;
	tmp.sender = getpid();
	sprintf(tmp.mtext, "%d", msgqid);
	sendToServer(&tmp);

  message msg;
	if(msgrcv(msgqid, &msg, MSG_SIZE, 0, 0) < -1) {
  	 	perror("CLIENT - MSGRCV failed");
      exit(EXIT_FAILURE);
  }
  printf("CLIENT %d\n", atoi(msg.mtext));

	while(runner) {
    char c;
		scanf(" %c", &c);
		switch(c) {
      case 'q':
				runner = 0;
				break;
			case 'e':
				echo();
				break;
			case 'c':
				terminate();
				break;
      case 't':
        gettime();
        break;
      case 'v':
        vers();
        break;
      default:
        perror("Invalid argument");
        break;
		}
	}
 //
 // quit
 //
  if((msgctl(msgqid, IPC_RMID, NULL)) < -1) {
    perror("CLIENT - Queue delete failed");
    exit(EXIT_FAILURE);
  }
  printf("CLIENT %d | Queue %d deleted\n", getpid(), msgqid);
  return 0;
}

void sendToServer(message *msg) {
  if (msgsnd(server_msgqid, msg, MSG_SIZE, 0) < 0) {
    perror("CLIENT - MSGSND failed");
    exit(EXIT_FAILURE);
  }
}

void terminate() {
	message tmp;
	tmp.mtype = 5;
	tmp.sender = getpid();
	sendToServer(&tmp);
}

void echo() {
	char c;
	printf("Enter the line:\n");
	while ((c = getchar()) != '\n' && c != EOF) {}

	char *line;
	line = (char *) malloc(lineSize * sizeof(char));

	fgets(line, lineSize, stdin);
	line[strlen(line)-1] = '\0';

	message tmp;
	tmp.mtype = 2;
	tmp.sender = getpid();
	strcpy(tmp.mtext, line);

	sendToServer(&tmp);
	free(line);

	message msg;
	msgrcv(msgqid, &msg, MSG_SIZE, 0, 0);
  printf("%s\n", msg.mtext);
}

void gettime() {
  message tmp;
	tmp.mtype = 3;
	tmp.sender = getpid();
	sendToServer(&tmp);

	message msg;
	printf("TIME : ");
	msgrcv(msgqid, &msg, MSG_SIZE, 0, 0);
  printf("%s\n", msg.mtext);
}

void vers() {
  char c;
	printf("Enter line:\n");
	while ((c = getchar()) != '\n' && c != EOF) {}

	char* line;
	line = (char *) malloc(lineSize * sizeof(char));
	fgets(line, lineSize, stdin);
	line[strlen(line)-1] = '\0';

	message tmp;
	tmp.mtype = 4;
	tmp.sender = getpid();
	strcpy(tmp.mtext, line);

	sendToServer(&tmp);
	free(line);

	message msg;
	msgrcv(msgqid, &msg, MSG_SIZE, 0, 0);
  printf("%s\n", msg.mtext);

}
