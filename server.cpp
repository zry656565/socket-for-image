/***
 * server.cpp
 * A simple server in the internet domain using TCP
 * The port number is passed as an argument
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    short width = 512;
    short height = 424;
    int data_len = width * height * 5;
    char data[data_len];
    int buffer_len = 100000;
    char buffer[buffer_len];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    
    // initialize socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    
    // listen to the client
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd,
                       (struct sockaddr *) &cli_addr,
                       &clilen);
    if (newsockfd < 0)
        error("ERROR on accept");
    
    // read from client
    printf("Start Reading...\n");
    int len = 0;
    bzero(data,sizeof(data));    
    bzero(buffer,sizeof(buffer));
    while ((n = read(newsockfd,buffer,buffer_len)) > 0) {
        memcpy(&data[len], buffer, n);
        len += n;
        printf("Read %d bytes, totally %d bytes...\n", n, len);
        bzero(buffer,sizeof(buffer));
        if (len >= data_len) break;
    }
    if (n < 0) error("ERROR reading from socket");
    
    // show the message from client
    int loc[5] = {0, 1, 40000, 100001, width * height * 5 - 10};
    for (int i = 0; i < 5; i++) {
        printf("Received: The %dth char is: %c, from %d bytes.\n", loc[i], data[loc[i]], len);
    }
    
    // write return message to client
    n = write(newsockfd,"I got your message",18);
    if (n < 0) error("ERROR writing to socket");
    close(newsockfd);
    close(sockfd);
    return 0;
}