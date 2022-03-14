#include "unp.h"

int main(int argc, char** argv)
{
    int sockfd;
    struct sockaddr_in servaddr;
    char sendline[MAXLINE], receiveline[MAXLINE];
    if (argc != 2)
    {
        printf("Usage: client <IP address>");
        exit(0);
    }
    bzero(&servaddr, sizeof(servaddr));
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);


    socklen_t len = sizeof(servaddr);
    int n;
    while (fgets(sendline, MAXLINE, stdin) != NULL)
    {
        sendto(sockfd, sendline, sizeof(sendline), 0, (SA *) &servaddr, len);
        n = recvfrom(sockfd, receiveline, MAXLINE, 0, (SA *) &servaddr, &len);
        receiveline[n] = 0;
        fputs(receiveline, stdout);
    }


    return 0;
}