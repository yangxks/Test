
#include <iostream>
#include "Crypto.h"
using namespace std;

int err_info(int sock, const char *str)
{
    close(sock);
    printf("%s.\n", str);
     
    return 1;
}

int DownloadPage(const char *Host, const int Port, char **PostData, char *RecvData)
{
    int sockfd;
    size_t n;
    char header[500];
    struct sockaddr_in link;
    
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);    
    if(sockfd < 0)
    {
        printf("Socket error: %s\n", strerror(errno));
        return 0;
    }

    link.sin_family = AF_INET;
    link.sin_port = htons(Port);
    link.sin_addr.s_addr = inet_addr(Host);      
    printf("Waiting for connect %s:%d...........\n", Host, Port);
    
    if(connect(sockfd, (struct sockaddr*)&link, sizeof(struct sockaddr_in)) == -1)
    {          
            return err_info(sockfd,"Connect time out!\n");
    }

    printf("Connect %s:%d OK!\n", Host, Port);
    n = write(sockfd, *PostData, strlen(*PostData));
      
    if(n == strlen(*PostData))
        printf("Send data OK. length:%zu.\n", n);
    else 
    {
        sprintf(header ,"Wrote error: %s\n", strerror(errno));            
        return err_info(sockfd ,header);
    }
      
    printf("Waiting for data from %s.........\n", Host);
    if(read(sockfd, RecvData, 511) == -1) 
    {
        sprintf(header, "Recv error:%s\n", strerror(errno));
        return err_info(sockfd, header);
    }
    
    printf("Recv %zu bytes.\n", strlen(RecvData));
    close(sockfd);
      
    return 0;
}


int test()
{
    char data[] = "{\"protocal\" : 1,\"mcc\" : \"310\"}";
    char *send[512], recv[512], *res[512];
    int n = 511, m = 511;
    
    class Crypto C1;
    memset(recv, 0, sizeof(recv));
    C1.pack(data, strlen(data),  send, n);
    DownloadPage("60.247.77.179", 5555, send, recv);

    n = strlen(recv);
    C1.unpack(recv, n, res, m);
    printf("res:%s.\n", *res);
    
    return 0;
}

int main()
{
    test();
    return 0;
}
