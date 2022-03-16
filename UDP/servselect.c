#include "unp.h"
#include "utl.h"

int main(int argc, char** argv)
{
    struct sockaddr_in servaddr, cliaddr;
    int listenfd, connfd, udpfd;
    socklen_t len;
    fd_set rset;
    int maxfd;
    int nready;
    pid_t pid;
    char buf[MAXLINE];

    void sigchild();

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    len = sizeof(servaddr);
    bind(listenfd, (SA *) &servaddr, len);
    listen(listenfd, LISTENQ);

    udpfd = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(udpfd, (SA *) &servaddr, len);

    FD_ZERO(&rset);
    maxfd = max(listenfd, udpfd);
    signal(SIGCHLD, sigchild);
    for ( ; ;)    
    {
        FD_SET(listenfd, &rset);
        FD_SET(udpfd, &rset);
        if ((nready = select(maxfd+1, &rset, NULL, NULL, NULL)) < 0)
        {
            if (errno == EINTR)
                continue;
            else
            {
                printf("Select error\n");
                exit(0);
            }
        }
        if (FD_ISSET(listenfd, &rset))
        {
            len = sizeof(cliaddr);
            connfd = accept(listenfd, (SA *) &cliaddr, &len);
            if ((pid = fork()) == 0)
            {
                close(listenfd);
                str_echo12(connfd, (SA *) &cliaddr, len);
                exit(0);
            }
            close(connfd);
        }
        if (FD_ISSET(udpfd, &rset))
        {
            len = sizeof(cliaddr);
            recvfrom(udpfd, buf, MAXLINE, 0, (SA *) &cliaddr, &len);
            printf("Receive UDP client message from : %s\n", sock_ntop((SA *) &cliaddr, len));
            sendto(udpfd, buf, sizeof(buf), 0, (SA *) &cliaddr, len);
        }
    }
    return 0;
}
void sigchild()
{
    pid_t pid;
    int stat;

    while((pid = waitpid(-1, &stat, WNOHANG)) > 0)
    {
        printf("child %d terminated\n", pid);
    }
    return;
}