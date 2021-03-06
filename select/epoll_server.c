#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/epoll.h>

#define MAXBUF 1024
#define MAXEPOLLSIZE 10000
static int PORT = 7838;
static int NUM = 10;
/*
 * 设置句柄为非阻塞方式
 */
int setnonblocking(int sockfd)
{
    if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)|O_NONBLOCK) == -1) 
    {
        return -1;
    }
    return 0;
}

int main(int argc, char **argv)
{
    socklen_t len;
    char buf[MAXBUF+1];
    struct sockaddr_in my_addr, their_addr;
    int listenfd, connfd, epfd, sockfd, nfds, n, curfds;
    
        // 声明epoll_event结构体的变量，ev用于注册事件，events数组用于回传要处理的事件
    struct epoll_event ev;
    struct epoll_event events[MAXEPOLLSIZE];    
    
    if ((listenfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) 
    {
        perror("socket");
        exit(1);
    } 
        // 把socket设置为非阻塞方式
    setnonblocking(listenfd);
    bzero(&my_addr, sizeof(my_addr));
    my_addr.sin_family = PF_INET;
    my_addr.sin_port = htons(PORT);    
    my_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) == -1) 
    {
        perror("bind");
        exit(1);
    }
    
    if (listen(listenfd, NUM) == -1) 
    {
        perror("listen");
        exit(1);
    } 
    printf("success start server! listen:%d.\n", PORT);

        // 创建 epoll句柄，把监听 socket 加入到 epoll 集合里 */
    epfd = epoll_create(MAXEPOLLSIZE); 
    len = sizeof(struct sockaddr_in);
    ev.events = EPOLLIN|EPOLLET;
    ev.data.fd = listenfd;
    
        // 将listenfd注册到epoll事件
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev) < 0) 
    {
        fprintf(stderr, "epoll set insertion error: fd=%d\n", listenfd);
        return -1;
    } 
    curfds = 1;
    
    while (1) 
    {
            // 等待有事件发生
        nfds = epoll_wait(epfd, events, curfds, -1);   
        if (nfds == -1)
        {
            perror("epoll_wait");
            break;
        }        
            // 处理所有事件 
        for (n = 0; n < nfds; ++n) 
        {
                // 如果新监测到一个SOCKET用户连接到了绑定的SOCKET端口，建立新的连接
            if (events[n].data.fd == listenfd) 
            {
                len = sizeof(struct sockaddr);
                connfd = accept(listenfd, (struct sockaddr *) &their_addr, &len);
                if (connfd < 0) 
                {
                    perror("accept");
                    continue;
                } 
                else
                    printf("new client: %s:%d，ID:%d.\n", inet_ntoa(their_addr.sin_addr), ntohs(their_addr.sin_port), connfd);

                setnonblocking(connfd);
                    // 设置用于注册的 读操作 事件
                ev.events = EPOLLIN | EPOLLET;
                    // 设置用于读操作的文件描述符
                ev.data.fd = connfd;	   
                epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
                curfds ++;
            } 
            else if (events[n].events & EPOLLIN) // 如果是已经连接的用户，并且收到数据，那么进行读入
            {	
                printf("EPOLLIN\n");
                if ((sockfd = events[n].data.fd) < 0)
                    continue;

                int len;
                bzero(buf, MAXBUF + 1);	   
                
                len = recv(sockfd, buf, MAXBUF, 0);
                if (len > 0)
                    printf("ID:%d, recv:'%s'，%d bytes.\n", sockfd, buf, len);
                else 
                {
                    if (len < 0) 
                    {
                        printf("recv error!code:%d，info'%s'\n", errno, strerror(errno));
                        epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, &ev);
                        curfds--;
                        continue;
                    }
                }

                ev.data.fd = sockfd;
                ev.events = EPOLLOUT | EPOLLET;
                epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev); //修改标识符，等待下一个循环时发送数据，异步处理的精髓!!!!! ?????
            }
            else if (events[n].events & EPOLLOUT) // 如果有数据发送
            {
                printf("EPOLLOUT\n");
                sockfd = events[n].data.fd;
		
                bzero(buf, MAXBUF + 1);
                strcpy(buf, "Server already processes!");

                send(sockfd, buf, strlen(buf), 0);
                ev.data.fd = sockfd;
                ev.events = EPOLLIN | EPOLLET;
                epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
            }
        }
    }   
    close(listenfd);
    
    return 0;
}
