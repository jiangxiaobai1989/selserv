/*************************************************************************
	> File Name: selclie.c
	> Author:Jiang Yuhang 
	> Mail: jiangxiaobai1989@gmail.com
	> Created Time: Sat 23 Jul 2016 03:58:23 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>

#define PORTNUM 15001
#define MAXLINE 1024
#define DADDR "115.28.49.209"

static void do_echocli(int);

int main(int ac, char *av[])
{
    int sock_id;
    struct sockaddr_in saddr;
    char sendbuf[1024], recvbuf[1024];


    sock_id = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_id == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    bzero((void *)&saddr, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(PORTNUM);
    saddr.sin_addr.s_addr = inet_addr(DADDR);

    if(connect(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) == -1)
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    printf("type something to server:\n");
    if( fgets(sendbuf, 1024, stdin) == NULL )
    {
        perror("gets input");
        exit(EXIT_FAILURE);
    }
    write(sock_id, sendbuf, 1024);

    do_echocli(sock_id);

    return 0;

}

void do_echocli(int sd)
{
    fd_set rfds;
    int retval;
    int maxfd;
    struct timeval tv;
    int n;
    char recvline[MAXLINE];

    while(1)
    {
        FD_ZERO(&rfds);
        FD_SET(sd, &rfds);
        maxfd = sd;
        tv.tv_sec = 5;
        tv.tv_usec = 0;

        retval = select(maxfd+1, &rfds, NULL, NULL, &tv);
        if(retval == -1)
        {
            return;
        }
        else if(retval == 0)
        {
            printf("client timeout.\n");
            continue;
        }

        if(FD_ISSET(sd, &rfds))
        {
            n = read(sd, recvline, MAXLINE);
            if(n <= 0)
            {
                fprintf(stderr, "client: server is closed.\n");
                close(sd);
                FD_CLR(sd, &rfds);
                return; 
            }
        }

        printf("client recv msg is: %s\n", recvline);
        sleep(5);
        write(sd, recvline, strlen(recvline)+1);
    }

}
