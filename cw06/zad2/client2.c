#define _XOPEN_SOURCE
#include "server2.h"
#define MAXCLIENTS 20
#define MAXMSGNUM 8
#define NAMESIZE 6

mqd_t serverD;
mqd_t queue;
char *name;
int runner = 1;
const int lineSize = 100;

void sendToServer(message *msg);
void deleteQ();
void echo();
void vers();
void gettime();
void login();
void terminate();

int main(int argc,char **argv){
  if((serverD = mq_open(NAME, O_RDWR)) == -1) {
		perror("CLIENT - Opening server queue failed");
		exit(EXIT_FAILURE);
	}
	printf("CLIENT %d | Server queue opened\n", serverD);

  struct mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = MAXMSGNUM;
	attr.mq_msgsize = sizeof(message);
	attr.mq_curmsgs = 0;

	name = (char *) malloc(NAMESIZE * sizeof(char));
  name[0] = '/';
  for (int i = 1; i < NAMESIZE-1; i++) {
    name[i] = (char) (rand()%22 + 97);
  }
  name[NAMESIZE-1] = '\0';
  printf("Client %d name: %s\n", getpid(), name);

  if((queue = mq_open(name, (O_CREAT | O_RDONLY), 0666, &attr)) == -1 ) {
		perror("CLINET - Creating queue failed");
		exit(EXIT_FAILURE);
	}
  printf("CLIENT | Queue created.\n");
  //
  // Loign
  //
  message tmp;
	tmp.mtype = Mlogin;
	tmp.sender = getpid();
	sprintf(tmp.mtext, "%s", name);
	sendToServer(&tmp);
	message msg;
	mq_receive(queue, (char *) &msg, sizeof(message), 0);
  printf("CLIENT %d | logged\n", atoi(msg.mtext));

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
  deleteQ();
}



void sendToServer(message *msg) {
  if (mq_send(serverD, (char *) msg, sizeof(message), 0) < 0) {
    perror("CLINET - Sending failed");
    exit(EXIT_FAILURE);
  }
}

void deleteQ(){
  mq_close(serverD);
  mq_close(queue);
  mq_unlink(name);
}

void echo() {
	char c;
	printf("Enter line:\n");
	while ((c = getchar()) != '\n' && c != EOF) {}
	char* line;
	line = (char *) malloc(lineSize * sizeof(char));
	fgets(line, lineSize, stdin);
	line[strlen(line)-1] = '\0';

	message tmp;
	tmp.mtype = Mecho;
	tmp.sender = getpid();
	strcpy(tmp.mtext, line);
	sendToServer(&tmp);
	free(line);
	message msg;
	mq_receive(queue, (char *) &msg, sizeof(message), 0);
  printf("%s\n", msg.mtext);
}

void vers() {
	char c;
	printf("Enter line:\n");
	while ((c = getchar()) != '\n' && c != EOF) {}
	char *line = (char *) malloc(lineSize * sizeof(char));
	fgets(line, lineSize, stdin);
	line[strlen(line)-1] = '\0';

	message tmp;
	tmp.mtype = Mvers;
	tmp.sender = getpid();
	strcpy(tmp.mtext, line);

	sendToServer(&tmp);
	free(line);

	message msg;
	mq_receive(queue, (char *) &msg, sizeof(message), 0);
  printf("%s\n", msg.mtext);
}

void gettime() {
	message tmp;
	tmp.mtype = Mtime;
	tmp.sender = getpid();
	sendToServer(&tmp);

	message msg;
	mq_receive(queue, (char *) &msg, sizeof(message), 0);
  printf("%s\n", msg.mtext);
}

void terminate() {
	message tmp;
	tmp.mtype = Mterm;
	tmp.sender = getpid();
	sendToServer(&tmp);
}
