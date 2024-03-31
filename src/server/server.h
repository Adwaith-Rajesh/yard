#ifndef YARD_SERVER_H
#define YARD_SERVER_H

#include <netinet/in.h>
#include <sys/socket.h>

struct Server {
    int domain;
    int service;
    int protocol;
    unsigned long interface;
    int port;
    int backlog;

    struct sockaddr_in address;
    int socket;

    void (*launch)(struct Server *);
};

struct Server server_init(int domain, int service, int protocol,
                          unsigned long interface, int port, int backlog,
                          void (*launch)(struct Server *));

#endif
