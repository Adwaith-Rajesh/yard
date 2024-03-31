#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <IP address> <port number>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *ip_address = argv[1];
    int port = atoi(argv[2]);

    int sock = 0;
    struct sockaddr_in serv_addr;
    char message[BUFFER_SIZE];
    char buffer[BUFFER_SIZE] = {0};

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip_address, &serv_addr.sin_addr) <= 0) {
        perror("\nInvalid address/ Address not supported \n");
        exit(EXIT_FAILURE);
    }
    while (1) {
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("Failed to connect socket...\n");
            exit(EXIT_FAILURE);
        }

        // Connect to server
        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            perror("Could not connect socket...\n");
            exit(EXIT_FAILURE);
        }

        bzero(message, BUFFER_SIZE);
        bzero(buffer, BUFFER_SIZE);
        printf("yard > ");
        if (fgets(message, sizeof(message), stdin) == NULL) {
            continue;
        }

        // Remove newline character from the message
        message[strcspn(message, "\n")] = 0;

        if (strncmp(message, "exit", 4) == 0) {
            printf("Bye...\n");
            break;
        }

        if (send(sock, message, strlen(message), 0) < 0) {
            perror("Could not send to the socket...\n");
            continue;
        }

        if (recv(sock, buffer, BUFFER_SIZE, 0) < 0) {
            perror("Could not read from the socket...\n");
            continue;
        }
        printf("%s\n", buffer);
        close(sock);
    }

    return 0;
}
