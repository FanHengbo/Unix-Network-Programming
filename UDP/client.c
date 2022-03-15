#include "unp.h"
#include "utl.h"
int main(int argc, char** argv)
{
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
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
    socklen_t lencli = sizeof(cliaddr);
    connect(sockfd, (SA *) &servaddr, len);
    getsockname(sockfd, (SA *) &cliaddr, &lencli);
    printf("local address is : %s\n", sock_ntop((SA *) &cliaddr, sizeof(cliaddr)));
    int n;
    while (fgets(sendline, MAXLINE, stdin) != NULL)
    {
        write(sockfd, sendline, sizeof(sendline));

        if ((n = read(sockfd, receiveline, MAXLINE)) < 0)
        {
            printf("read error\n");
            exit(0);
        }
        receiveline[n] = 0;
        printf("receive server echo from %s \n", sock_ntop((SA *) &servaddr, len));
        fputs(receiveline, stdout);

       /* sendto(sockfd, sendline, sizeof(sendline), 0, (SA *) &servaddr, len);
        n = recvfrom(sockfd, receiveline, MAXLINE, 0, (SA *) &servaddr, &len);
        receiveline[n] = 0;
       
        printf("receive server echo from %s \n", sock_ntop((SA *) &servaddr, len));
        fputs(receiveline, stdout);
        */
    }


    return 0;
}