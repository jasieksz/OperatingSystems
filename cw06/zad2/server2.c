#define _XOPEN_SOURCE
#include "server2.h"
#define MAXCLIENTS 20
#define MAXMSGNUM 8


int runner = 1;
int clientID = 0;
mqd_t queueD;
clientT clients[MAXCLIENTS];

int getPidID(pid_t pid);
void sendTo(int id, message* msg);
int queueEmpty(mqd_t qd);
void echo(message msg);
void vers(message msg);
void mtime(message msg);
void login(message msg);



int main(int argc,char **argv) {
  printf("SERVER :  %d\n", getpid());

  struct mq_attr attr;;
	attr.mq_flags = 0;
	attr.mq_maxmsg = MAXMSGNUM;
	attr.mq_msgsize = sizeof(message);
	attr.mq_curmsgs = 0;

  queueD = mq_open(NAME, (O_CREAT | O_RDONLY), 0666, &attr);
  if(queueD == -1) {
		perror("Creating queue failed");
		exit(EXIT_FAILURE);
	}
	printf("SERVER | Queue created\n");

  message myMessage;
	while(runner || !queueEmpty(queueD)) {

		if((mq_receive(queueD, (char *) &myMessage, sizeof(message), 0)) == -1){
			perror("Receiving failed");
			exit(EXIT_FAILURE);
		}

    switch (myMessage.mtype) {
      case Mlogin: //login
        printf("%s\n","LOGIN request");
        login(myMessage);
        break;
      case Mecho: //echo
        printf("%s\n","ECHO request");
        echo(myMessage);
        break;
      case Mtime: //time
        printf("%s\n","TIME request");
        mtime(myMessage);
        break;
      case Mvers: //vers
        printf("%s\n","VERS request");
        vers(myMessage);
        break;
      case Mterm: //terminate
        printf("%s\n","TERMINATE request");
        runner = 0;
        break;
      default:
        perror("Illegal message");
        break;
    }
  }

  	if( mq_close(queueD) < 0) {
  		perror("Closing queue failed");
  		exit(EXIT_FAILURE);
  	}

  	if( mq_unlink(NAME) < 0) {
  		perror("Removing queue failed");
  		exit(EXIT_FAILURE);
  	}

  	printf("Queue deleted.\n");
  	return 0;

}

int getPidID(pid_t pid) {
	int p = -1;
	for(int i=0; i < clientID; i++)
    if(clients[i].pid == pid) {
			p = i;
			break;
		}
	return p;
}

void sendTo(int id, message *msg) {
	if(clients[id].key == -1)
		return;

  if ((mq_send(clients[id].key, (char *) msg, sizeof(message), 0)) < 0) {
    perror("MQ_SEND failed");
    exit(EXIT_FAILURE);
  }
}

int queueEmpty(mqd_t qd) {
	struct mq_attr attr;
	if(mq_getattr(qd, &attr) == -1){
		perror("Accesing server queue failed");
		exit(EXIT_FAILURE);
	}
	return (attr.mq_curmsgs == 0);
}

void mtime(message msg) {
	message tmp;
	tmp.mtype = Mtime;
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
	tmp.mtype = Mvers;
	tmp.sender = getpid();
	strcpy(tmp.mtext, msg.mtext);
	int p = getPidID(msg.sender);

	int j = 0;
	int len = strlen(tmp.mtext);
	while(j < len ) {
		if(tmp.mtext[j] >= 97 && tmp.mtext[j] <= 122)
			tmp.mtext[j] -= 32;
		j++;
	}

	sendTo(p, &tmp);
}

void echo(message msg) {
	message tmp;
	tmp.mtype = Mecho;
	tmp.sender = getpid();
	strcpy(tmp.mtext, msg.mtext);
	int p = getPidID(msg.sender);

	sendTo(p, &tmp);
}


void login(message msg) {
	mqd_t client;
	if((client = mq_open(msg.mtext, O_WRONLY)) == -1) {
		perror("Opening client queue failed");
		exit(EXIT_FAILURE);
	}

	clients[clientID].key = client;
	clients[clientID].pid = msg.sender;
	message tmp;
	tmp.mtype = Mlogin;
	tmp.sender = getpid();
	sprintf(tmp.mtext, "%d", clientID);

	printf("SERVER %d logged\n", msg.sender);
	sendTo(clientID, &tmp);
  clientID++;
}
