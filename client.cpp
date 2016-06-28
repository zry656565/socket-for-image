/***
 * client.cpp
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    short width = 960;
    short height = 540;
    int data_len = width * height * 5;
    char data[data_len];
    int buffer_len = 50000;
    char buffer[buffer_len];
    
    if (argc < 3) {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(0);
    }
    
    // initialize socket
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    printf("Host found.\n");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    printf("Connect succeeded.\n");
    
    // receive images from server
    int len;
    while (true) {
        len = 0;
        n = 0;
        bzero(data, sizeof(data));
        bzero(buffer, sizeof(buffer));
        
        while ((n = read(sockfd, buffer, buffer_len)) > 0) {
            memcpy(&data[len], buffer, n);
            len += n;
            printf("Read %d bytes, totally %d bytes...\n", n, len);
            bzero(buffer,sizeof(buffer));
            if (len >= data_len) break;
        }
        if (n < 0) error("ERROR reading from socket");
        
        // show the message from server
        int loc[5] = {0, 1, 40000, 100001, width * height * 5 - 10};
        for (int i = 0; i < 5; i++) {
            printf("Received: The %dth char is: %c, from %d bytes.\n", loc[i], data[loc[i]], len);
        }
        
        n = write(sockfd,"DONE",4);
        if (n < 0) error("ERROR writing to socket");
        printf("send `DONE` message to server\n");
    }
    
    close(sockfd);
    return 0;
}