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
    int buffer_len = width * height * 5;
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

    bzero(buffer,buffer_len);
    buffer[0] = 'a';
    buffer[1] = 'b';
    buffer[40000] = 'c';
    buffer[100001] = 'd';
    buffer[width * height * 5 - 10] = 'x';
    printf("Sending..., Buffer Length: %d\n", buffer_len);
    n = write(newsockfd,buffer,buffer_len);
    if (n < 0)
        error("ERROR writing to socket");
    
    close(newsockfd);
    close(sockfd);
    return 0;
}