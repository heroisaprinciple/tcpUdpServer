#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MYPORT "3490"
#define BACKLOG 10
#define MAXDATASIZE 100

void client() {
    printf("Client starting!\n");

    int client_socket;
    int sent_bytes, received_bytes;
    char msg[] = "Hi, server";
    char buffer[MAXDATASIZE];

    struct addrinfo hints;
    struct addrinfo *servinfo; // will point to results

    // Fills all hints struct with 0 size of hints
    memset(&hints, 0, sizeof(hints)); // Or: struct addrinfo hints = {0};
    // hints.ai_flags = AI_PASSIVE; // fill in my IP for me
    hints.ai_family = AF_UNSPEC; // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP

    // Ip of server in WSL
    // getaddrinfo() returns one or more addrinfo structures, each of which contains an
    // Internet address that can be specified in a call to bind(2) or connect(2)

    //    The getaddrinfo() function allocates and initializes a linked list of addrinfo structures, one for
    //    each network address that matches node and service, subject to any restrictions imposed by hints, and
    //    returns a pointer to the start of the list in servinfo. The items in the linked list are linked by the ai_next field.
    int status = getaddrinfo("172.27.35.246", MYPORT, &hints, &servinfo);

    // If getaddrinfo() fails, it returns a nonzero error code.
    if (status != 0)  {
        fprintf(stderr, "getaddrinfo error is: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    // socket() simply returns to you a socket descriptor that you can use in later system calls, or -1 on error.
    client_socket = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (client_socket == -1) {
        perror("socket()");
        exit(EXIT_FAILURE);
    }

    int conn = connect(client_socket, servinfo->ai_addr, servinfo->ai_addrlen);
    if (conn == -1) {
        perror("connect()");
        exit(EXIT_FAILURE);
    }

    printf("Yay! Connected to the server.");

    sent_bytes = send(client_socket, msg, strlen(msg), 0);
    if (sent_bytes == -1) {
        perror("send()");
        exit(EXIT_FAILURE);
    }

    // Adding a null terminator (\0) as absence of it may cause garbage output
    received_bytes = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (received_bytes == -1) {
        perror("recv()");
        exit(EXIT_FAILURE);
    }

    buffer[received_bytes] = '\0';
    printf("Client received a message: %s\n", buffer);
    close(client_socket);

}

void server() {
    struct addrinfo *res;
    struct addrinfo hints;
    struct sockaddr_storage addr;
    char buffer[MAXDATASIZE];
    char msg[] = "A message received!";

    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(NULL, MYPORT, &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo error is: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    int socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (socket_fd == -1) {
        perror("socket()");
        exit(EXIT_FAILURE);
    }

    // sockfd is the socket file descriptor returned by socket(). my_addr is a pointer to a struct sockaddr
    // that contains information about your address, namely, port and IP address. addrlen is the length in bytes
    // of that address.
    if (bind(socket_fd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("bind()");
        exit(EXIT_FAILURE);
    }

    if (listen(socket_fd, BACKLOG) == -1) {
        perror("listen()");
        exit(EXIT_FAILURE);
    }

    printf("Server: Waiting for connections...\n");

    socklen_t addr_size = sizeof(addr);

    //    It extracts the first connection request on the queue of pending connections for the listening socket, sockfd,
    //    creates a new connected socket, and returns a new file descriptor referring to that socket. The newly created socket
    //    is not in the listening state. The original socket sockfd is unaffected by this call.
    int new_socket = accept(socket_fd, (struct sockaddr *)&addr, &addr_size);
    if (new_socket == -1) {
        perror("The error on accept call.");
        exit(EXIT_FAILURE);
    }

    printf("Server: Client connected!\n");

    int received_bytes = recv(new_socket, buffer, sizeof(buffer) - 1, 0);
    if(received_bytes == -1) {
        perror("recv()");
        exit(EXIT_FAILURE);
    }
    buffer[received_bytes] = '\0';
    printf("Server received: %s\n", buffer);

    int sent_bytes = send(new_socket, msg, strlen(msg), 0);
    if(sent_bytes == -1) {
        perror("send()");
        exit(EXIT_FAILURE);
    }

    close(new_socket);
    close(socket_fd);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s [server|client]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "server") == 0) {
        server();
    } else if (strcmp(argv[1], "client") == 0) {
        sleep(1);  // Ensure the server starts before the client connects
        client();
    } else {
        fprintf(stderr, "Invalid argument. Use 'server' or 'client'.\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}

