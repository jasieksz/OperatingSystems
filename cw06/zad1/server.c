#define _XOPEN_SOURCE
#define MAXCLIENTS 20

#include "server.h"

void echo(message msg);
void login(message msg);
void mtime(message msg);
void vers(message msg);
void sendTo(int id, message *msg);
int getPidID(pid_t pid);

int runner = 1;
char *homedir;
client clients[MAXCLIENTS];
int clientID = 0;
int msgqid;


int main(int argc,char **argv) {
  printf("Server %d\n", getpid());

  if ((homedir = getenv("HOME")) == NULL) {
      perror("Getting homedir failed");
      exit(EXIT_FAILURE);
  }

  key_t key;
  if((key = ftok(homedir, 'p')) < 0){
		perror("FTOK failed");
		exit(EXIT_FAILURE);
	}

  int msgqid = msgget(key, IPC_CREAT | 0666);
	if (msgqid < 0) {
		perror("MSG queue failed");
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
      case 2: //echo
        printf("%s\n","ECHO request");
        echo(myMessage);
        break;
      case 3: //time
        printf("%s\n","TIME request");
        mtime(myMessage);
        break;
      case 4: //vers
        printf("%s\n","VERS request");
        vers(myMessage);
        break;
      case 5: //terminate
        printf("%s\n","TERMINATE request");
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

void echo(message msg) {
	message tmp;
	tmp.mtype = 2; //echo
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

void vers(message msg) {
  message tmp;
  tmp.mtype = 4;
  tmp.sender = getpid();
  strcpy(tmp.mtext, msg.mtext);
  int p = getPidID(msg.sender);

  int j = 0;
  int len = strlen(tmp.mtext);
  while (j < len ) {
    if(tmp.mtext[j] >= 97 && tmp.mtext[j] <= 122)
      tmp.mtext[j] -= 32;
    j++;
  }
  sendTo(p, &tmp);
}
