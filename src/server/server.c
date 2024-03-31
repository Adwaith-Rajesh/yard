#include "server.h"

#include <stdio.h>
#include <stdlib.h>

struct Server server_init(int domain, int service, int protocol,
                          unsigned long interface, int port, int backlog,
                          void (*launch)(struct Server *)) {
    struct Server server;

    server.domain = domain;
    server.service = service;
    server.protocol = protocol;
    server.interface = interface;
    server.port = port;
    server.backlog = backlog;

    server.address.sin_family = domain;
    server.address.sin_port = htons(port);
    server.address.sin_addr.s_addr = htonl(interface);

    server.socket = socket(domain, service, protocol);
    if (server.socket == 0) {
        perror("Failed to connect socket...\n");
        exit(EXIT_FAILURE);
    }

    int option = 1;
    setsockopt(server.socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    // bind the server to the address
    if (bind(server.socket, (struct sockaddr *)&server.address, sizeof(server.address)) < 0) {
        perror("Failed to bind socket...\n");
        exit(EXIT_FAILURE);
    }

    // listen
    if (listen(server.socket, server.backlog) < 0) {
        perror("Failed to start listening...\n");
        exit(EXIT_FAILURE);
    }

    server.launch = launch;

    return server;
}
