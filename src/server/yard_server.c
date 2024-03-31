// This is the entry point

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "server.h"

#define BUFFER_SIZE 1024

void launch(struct Server *server) {
    char buffer[BUFFER_SIZE];

    while (1) {
        // accepter
        printf(" ==== WAITING ==== \n");
        int addr_len = sizeof(server->address);
        int new_socket = accept(server->socket, (struct sockaddr *)&(server->address), (socklen_t *)&addr_len);

        if (read(new_socket, buffer, BUFFER_SIZE) < 0) {
            perror("Cannot read from socket..\n");
            // exit(EXIT_FAILURE);
        }

        // handler
        printf("%s\n", buffer);

        // responder
        char *hello = "Hello World";

        if (write(new_socket, hello, strlen(hello)) < 0) {
            perror("Cannot write to socket...\n");
        }

        close(new_socket);
    }
}

int main() {
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
