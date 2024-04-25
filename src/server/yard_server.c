// This is the entry point

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include "server.h"

#define BUFFER_SIZE 1024

int new_socket;

void handle_sigint(int signum) {
    printf("closing Socket and exiting...\n");
    close(new_socket);
    exit(signum);
}

void launch(struct Server *server) {
    char buffer[BUFFER_SIZE] = {0};

    while (1) {
        // accepter
        printf(" ==== WAITING ==== \n");
        int addr_len = sizeof(server->address);
        new_socket = accept(server->socket, (struct sockaddr *)&(server->address), (socklen_t *)&addr_len);

        if (recv(new_socket, buffer, BUFFER_SIZE, 0) < 0) {
            perror("Cannot read from socket..\n");
            continue;
            // exit(EXIT_FAILURE);
        }

        // handler
        printf("%s\n", buffer);

        // responder
        if (send(new_socket, buffer, strlen(buffer), 0) < 0) {
            perror("Cannot write to socket...\n");
            continue;
        }
        bzero(buffer, BUFFER_SIZE);
        close(new_socket);
    }
}

int server() {
    signal(SIGINT, handle_sigint);

    struct Server server = server_init(
        AF_INET,      // domain
        SOCK_STREAM,  // service
        0,            // protocol
        INADDR_ANY,   // interface
        8080,         // port
        10,           // backlog
        launch        // launch
    );

    server.launch(&server);
}
