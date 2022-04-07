#include "unp.h"
#include "utils.h"
int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Usage <hostname/Ip address> <service/port>\n");
        exit(EXIT_FAILURE);
    }

    int sockfd;
    int connfd;
    struct addrinfo hint, *result, *result_tranverse;
    int n;
    struct sockaddr_storage ss;
    char receiveline[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;


    bzero(&hint,sizeof(hint));
    hint.ai_family = AF_UNSPEC;
    hint.ai_socktype = SOCK_STREAM;
    if ((n = getaddrinfo(argv[1], argv[2], &hint, &result)) < 0)
    {
        printf("getaddrinfo error, hotname: %s, service: %s, error message: %s\n", argv[1], argv[2], gai_strerror(n));
        exit(EXIT_FAILURE);
    }
    for (result_tranverse = result; result_tranverse; result_tranverse = result_tranverse->ai_next)
    {
        sockfd = socket(result_tranverse->ai_family, result_tranverse->ai_socktype, result_tranverse->ai_protocol);
        if (sockfd < 0)
        {
            continue;
        }

        if (connect(sockfd, result_tranverse->ai_addr, result_tranverse->ai_addrlen) == 0)
        {
            memcpy(&servaddr, result_tranverse->ai_addr, sizeof(servaddr));
            len = sizeof(servaddr);
            printf("Connected to %s\n", sock_ntop((SA *) &servaddr, len));
            break;
        }
        close(sockfd);
    }   
    if (result_tranverse == NULL)
    {
        printf("tcp connect error for host %s serivice %s\n", argv[1], argv[2]);
        exit(EXIT_FAILURE);
    }
    freeaddrinfo(result);


    len = sizeof(ss);
    getpeername(sockfd, (SA *)&ss, &len);
    len = sizeof(cliaddr);
    getsockname(sockfd, (SA *) &cliaddr, &len);
    printf("Connected to %s(double check to make sure it's right)\n", sock_ntop((SA *) &ss, len));
    printf("local address is %s\n" ,sock_ntop((SA *) &cliaddr, len));
    while ((n = read(sockfd, receiveline, MAXLINE)))
    {
        receiveline[n] = 0;
        fputs(receiveline, stdout);
    }
    
    return 0;
}