#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "chat.h"
#include <stdlib.h>
#include <sys/select.h>

#define MAX_CLIENT 5
#define MAX_ID 32
#define MAX_BUF 256

typedef struct {
    int sockfd;
    int inUse;
    char uid[MAX_ID];
} ClientType;

int Sockfd;
fd_set allset, rset;
int maxfd;

ClientType Client[MAX_CLIENT];

void SendToOtherClients(int id, char *buf) {
    int i;
    char msg[MAX_BUF + MAX_ID];

    sprintf(msg, "%s> %s", Client[id].uid, buf);
    printf("%s", msg);

    for (i = 0; i < MAX_CLIENT; i++) {
        if (Client[i].inUse && (i != id)) {
            if (send(Client[i].sockfd, msg, strlen(msg) + 1, 0) < 0) {
                perror("send");
                exit(1);
            }
        }
    }
}

void ProcessClient(int id) {
    char buf[MAX_BUF];
    int n;

    if ((n = recv(Client[id].sockfd, Client[id].uid, MAX_ID, 0)) < 0) {
        perror("recv");
        exit(1);
    }
    printf("Client %d log-in(ID: %s).....\n", id, Client[id].uid);

    while (1) {
        if ((n = recv(Client[id].sockfd, buf, MAX_BUF, 0)) < 0) {
            perror("recv");
            exit(1);
        }
        if (n == 0) {
            printf("Client %d log-out(ID: %s).....\n", id, Client[id].uid);

            close(Client[id].sockfd);
            Client[id].inUse = 0;

            strcpy(buf, "log-out.....\n");
            SendToOtherClients(id, buf);

            FD_CLR(Client[id].sockfd, &allset);

            break;
        }

        SendToOtherClients(id, buf);
    }
}

void CloseServer(int signo) {
    int i;

    close(Sockfd);

    for (i = 0; i < MAX_CLIENT; i++) {
        if (Client[i].inUse) {
            close(Client[i].sockfd);
        }
    }

    printf("\nChat server terminated.....\n");

    exit(0);
}

int GetID() {
    int i;

    for (i = 0; i < MAX_CLIENT; i++) {
        if (!Client[i].inUse) {
            Client[i].inUse = 1;
            return i;
        }
    }
}

int main(int argc, char *argv[]) {
    int newSockfd, id, one = 1;
    socklen_t cliAddrLen;
    struct sockaddr_in cliAddr, servAddr;
    int n;

    signal(SIGINT, CloseServer);

    if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    if (setsockopt(Sockfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)) < 0) {
        perror("setsockopt");
        exit(1);
    }

    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = PF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(SERV_TCP_PORT);

    if (bind(Sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        perror("bind");
        exit(1);
    }

    listen(Sockfd, 5);

    printf("Chat server started.....\n");

    FD_ZERO(&allset);
    FD_SET(Sockfd, &allset);
    maxfd = Sockfd;

    cliAddrLen = sizeof(cliAddr);
    while (1) {
        rset = allset;

        if (select(maxfd + 1, &rset, NULL, NULL, NULL) < 0) {
            perror("select");
            exit(1);
        }

        if (FD_ISSET(Sockfd, &rset)) {
            newSockfd = accept(Sockfd, (struct sockaddr *)&cliAddr, &cliAddrLen);
            if (newSockfd < 0) {
                perror("accept");
                exit(1);
            }

            id = GetID();
            Client[id].sockfd = newSockfd;
            FD_SET(newSockfd, &allset);
            if (newSockfd > maxfd) {
                maxfd = newSockfd;
            }

            if ((n = recv(Client[id].sockfd, Client[id].uid, MAX_ID, 0)) < 0) {
                perror("recv");
                exit(1);
            }
            printf("Client %d log-in(ID: %s).....\n", id, Client[id].uid);
        }

        for (id = 0; id < MAX_CLIENT; id++) {
            if (Client[id].inUse && FD_ISSET(Client[id].sockfd, &rset)) {
                ProcessClient(id);
            }
        }
    }
    return 0;
}