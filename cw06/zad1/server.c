#define _XOPEN_SOURCE
#define MAXCLIENTS 20

#include "server.h"
#include <signal.h>

void mirror(message msg);
void login(message msg);
void mtime(message msg);
void calc(message msg);
void sendTo(int id, message *msg);
int getPidID(pid_t pid);

void sigintHandler(int sig);
void removeQueue();

int runner = 1;
char *homedir;
client clients[MAXCLIENTS];
int clientID = 0;
int msgqid;


int main(int argc,char **argv) {
  printf("Server %d\n", getpid());

  if(signal(SIGINT, sigintHandler) == SIG_ERR) {
    perror("SERVER - SIGINT failed");
    exit(EXIT_FAILURE);
  }

  if (atexit(removeQueue) == -1){
		perror("SERVER - atexit() failed");
		exit(EXIT_FAILURE);
	}

  if ((homedir = getenv("HOME")) == NULL) {
      perror("SERVER - Getting homedir failed");
      exit(EXIT_FAILURE);
  }

  key_t key;
  if((key = ftok(homedir, 'p')) < 0){
		perror("SERVER - FTOK failed");
		exit(EXIT_FAILURE);
	}

  int msgqid = msgget(key, IPC_CREAT | 0666);
	if (msgqid < 0) {
		perror("SERVER - MSG queue failed");
		exit(EXIT_FAILURE);
	}

  message myMessage;
  int empty = 0;

  while (runner || !empty){
    struct msqid_ds buf;
    int numMsg;
    msgctl(msgqid, IPC_STAT, &buf);
    numMsg = buf.msg_qnum;

    if(numMsg == 0)
 	 		empty = 1;

    if(msgrcv(msgqid, &myMessage, MSG_SIZE, 0, 0) < -1) {
      perror("MSGRCV failed");
      exit(EXIT_FAILURE);
    }

    switch (myMessage.mtype) {
      case 1: //login
        printf("%s\n","LOGIN request");
        login(myMessage);
        break;
      case 2: //mirror
        printf("%s\n","MIRROR request");
        mirror(myMessage);
        break;
      case 3: //time
        printf("%s\n","TIME request");
        mtime(myMessage);
        break;
      case 4: //calc
        printf("%s\n","CALC request");
        calc(myMessage);
        break;
      case 5: //end
        printf("%s\n","END request");
        runner = 0;
        break;
      default:
        perror("Illegal message");
        break;

    }
  }
  if((msgctl(msgqid, IPC_RMID, NULL)) < -1){
		perror("Queue delete failed");
		exit(EXIT_FAILURE);
	}
}

void sendTo(int id, message *msg) {
  if (msgsnd(clients[id].key, msg, MSG_SIZE, 0) < 0) {
    perror("MSGSND failed");
    exit(EXIT_FAILURE);
  }
}

int getPidID(pid_t pid) {
	int p = -1;
	for(int i=0; i < clientID; i++) {
		if(clients[i].pid == pid) {
			p = i;
			break;
		}
	}
	return p;
}

void login(message msg) {
	printf("SERVER %d | logged\n", msg.sender);
  if (clientID < MAXCLIENTS){
    client ctmp;
    ctmp.key = atoi(msg.mtext);
    ctmp.pid = msg.sender;
    clients[clientID] = ctmp;

    message tmp;
  	tmp.mtype = 1;
  	tmp.sender = getpid();
  	sprintf(tmp.mtext, "%d", clientID);
  	sendTo(clientID, &tmp);
    clientID++;
  } else {
    perror("Too many clients");
    exit(EXIT_FAILURE);
  }
}

void mirror(message msg) {
  int msg_length = (int) strlen(msg.mtext);
  for(int i = 0; i < msg_length / 2; ++i) {
    char buff = msg.mtext[i];
    msg.mtext[i] = msg.mtext[msg_length - i - 1];
    msg.mtext[msg_length - i - 1] = buff;
  }
  message tmp;
	tmp.mtype = 2; //mirror
	tmp.sender = getpid();

	strcpy(tmp.mtext, msg.mtext);
	int p = getPidID(msg.sender);
	sendTo(p, &tmp);
}

void mtime(message msg) {
  message tmp;
	tmp.mtype = 3;
	tmp.sender = getpid();
	int p = getPidID(msg.sender);

	time_t timer;
	struct tm* tm_info;
	time(&timer);
	tm_info = localtime(&timer);
	strftime(tmp.mtext, MMSGLEN, "%Y-%m-%d %H:%M:%S", tm_info);

	sendTo(p, &tmp);
}

void calc(message msg) {
  message tmp;
  tmp.mtype = 4;
  tmp.sender = getpid();
  int p = getPidID(msg.sender);

  char command[4096];
  sprintf(command, "echo '%s' | bc", msg.mtext);
  FILE* calc = popen(command, "r");
  fgets(msg.mtext, MMSGLEN, calc);
  pclose(calc);
  strcpy(tmp.mtext, msg.mtext);
  sendTo(p, &tmp);
}

void sigintHandler(int sig) {
    if (sig == SIGINT) {
        fprintf(stderr, "\nOdebrano sygnal SIGINT\n");
        signal(SIGINT, SIG_DFL);
        raise(SIGINT);
    }
}

void removeQueue() {
  if(msgqid <= -1) 
    return;
  if (msgctl(msgqid, IPC_RMID, NULL) < 0) {
    perror("SERVER - Queue delete failed");
		return;
  }
  printf("SERVER - queue deleted\n");
}