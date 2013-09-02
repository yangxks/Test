//客户端的一个简单的实现，只是为了证实一下，服务器端程序的正确性

//select_client.c
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAXDATASIZE 100
#define MAXLINE 1024

int main(int argc,char *argv[])
{
    int sockfd,sendbytes;
    char send[MAXLINE];
    struct hostent *host;
    struct sockaddr_in serv_addr;

    if(argc < 3){
        fprintf(stderr,"Please enter the server's hostname\n");
        exit(1);
    }

    if((host = gethostbyname(argv[1])) == NULL) {
        perror("gethostbyname");
        exit(1);
    }
    
    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1) {
        perror("socket error \n");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
    serv_addr.sin_addr = *((struct in_addr *)host->h_addr);
    bzero(&(serv_addr.sin_zero),8);

    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) ==-1) {
        perror("connect");
        exit(1);
    }
    
    while(fgets(send,1024,stdin) != NULL)
    {
        if((sendbytes = write(sockfd,send,100)) ==-1) {
            perror("send error \n");
            exit(1);
        }
    }
    close(sockfd);

    return 0;
}
