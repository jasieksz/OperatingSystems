
#define _XOPEN_SOURCE

#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

int interval = 1;

void sigintHandler(int sig);
void sigtstpHandler(int sig);
void infLoop();

int main(void) {

    struct sigaction saction;
    saction.sa_handler = sigtstpHandler;

    if(signal(SIGINT, sigintHandler) == SIG_ERR) {
        perror("SIGINT failed");
        exit(EXIT_FAILURE);
    }

    if(sigaction(SIGTSTP, &saction, NULL) == -1) {
        perror("SIGTSTP failed!");
        exit(EXIT_FAILURE);
    }

    infLoop();
    return 0;
}

void infLoop(){
    while (1) {
        int counter = 0;
        while (counter < 26) {
            printf("%c\n", 'a' + counter);
            sleep(1);
            counter += interval;
            if (counter == -1)
              counter = 25;
        }
    }
}

void sigtstpHandler(int sig) {
    if (sig == SIGTSTP) {
        interval *= (-1);
    }
}

void sigintHandler(int sig) {
    if (sig == SIGINT) {
        fprintf(stderr, "\nOdebrano sygnal SIGINT\n");
        signal(SIGINT, SIG_DFL);
        raise(SIGINT);
    }
}
