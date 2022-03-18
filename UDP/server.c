#include "unp.h"
#include "utl.h"
int main(int argc, char** argv)
{
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(sockfd, (SA *) &servaddr, sizeof(servaddr));

    int n;
    socklen_t len;
    char recieveline[MAXLINE];

    for (; ;)
    {
        len = sizeof(cliaddr);
        n = recvfrom(sockfd, recieveline, MAXLINE, 0, (SA *) &cliaddr, &len);
        printf("receive %d bytes from %s \n", n ,sock_ntop((SA *) &cliaddr, len));
        sendto(sockfd, recieveline, n, 0, (SA *) &cliaddr, len);
    }
    return 0;
}