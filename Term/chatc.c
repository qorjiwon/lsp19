#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include "chat.h"
#include <stdlib.h>

#define MAX_BUF 256

int Sockfd;

void *ChatClient(void *arg) {
    char buf[MAX_BUF];
    int n;

    while (1) {
        if ((n = recv(Sockfd, buf, MAX_BUF, 0)) < 0) {
            perror("recv");
            exit(1);
        }
        if (n == 0) {
            fprintf(stderr, "Server terminated.....\n");
            close(Sockfd);
            exit(1);
        }
        printf("%s", buf);
    }
}

void CloseClient(int signo) {
    close(Sockfd);
    printf("\nChat client terminated.....\n");

    exit(0);
}

int main(int argc, char *argv[]) {
    struct sockaddr_in servAddr;
    struct hostent *hp;
    pthread_t tid;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s ServerIPaddress\n", argv[0]);
        exit(1);
    }

    if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = PF_INET;
    servAddr.sin_port = htons(SERV_TCP_PORT);

    if (isdigit(argv[1][0])) {
        servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    } else {
        if ((hp = gethostbyname(argv[1])) == NULL) {
            fprintf(stderr, "Unknown host: %s\n", argv[1]);
            exit(1);
        }
        memcpy(&servAddr.sin_addr, hp->h_addr_list, hp->h_length);
    }

    if (connect(Sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        perror("connect");
        exit(1);
    }

    signal(SIGINT, CloseClient);

    // Create a thread for receiving messages from the server
    if (pthread_create(&tid, NULL, ChatClient, NULL) < 0) {
        perror("pthread_create");
        exit(1);
    }

    char input[MAX_BUF];
    while (1) {
        fgets(input, MAX_BUF, stdin);

        if (send(Sockfd, input, strlen(input) + 1, 0) < 0) {
            perror("send");
            exit(1);
        }
    }

    return 0;
}

